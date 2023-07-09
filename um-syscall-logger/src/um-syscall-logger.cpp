/*
*   um-syscall-logger.cpp
* 
*   ...
*/

#include "./process/process.hpp"
#include "./token/token.hpp"

#include "./syscall-table.hpp"

#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

struct call_info_t
{
    uint64_t return_address;
    uint32_t return_value;
    uint32_t syscall_id;
    uint32_t thread_id;
    uint32_t process_id;
};

struct function_info_t
{
    const char* _fmt;
    HANDLE _process_handle;
    void* _syscall_id_name_table;
    void* _printf;
    void* _ReadProcessMemory;
    void* _VirtualFreeEx;
    void* _TerminateThread;
};

// https://cdn.discordapp.com/attachments/765576637265739789/1127602040810516551/ida64_OPigRVderV.png
// https://cdn.discordapp.com/attachments/765576637265739789/1127604045079650454/devenv_CwN4GouNvw.png
// cant defeat the optimizer
// this shit is a failed project it only runs on debug mode smfh

// callback function
void on_syscall_return( uint64_t call_info_address )
{
    // this is a horrible solution for a problem i legit didnt think about, gg
    const volatile function_info_t* memory = reinterpret_cast<function_info_t*>(0x1234567878563412); // volatile bc fuck you optimizer

    const HANDLE _process_handle = memory->_process_handle;

    call_info_t call_info{ 0 };

    reinterpret_cast<decltype(&ReadProcessMemory)>(memory->_ReadProcessMemory)( _process_handle, reinterpret_cast<void*>(call_info_address), &call_info, sizeof( call_info_t ), nullptr );
    reinterpret_cast<decltype(&VirtualFreeEx)>(memory->_VirtualFreeEx)( _process_handle, reinterpret_cast<void*>(call_info_address), NULL, MEM_RELEASE );
    
    if (call_info.syscall_id < syscall_id_name_table_size ) // this table only contains nt syscall numbers, if u really went all out u need to do symbol parsing & disassembly to figure out the right syscalls, which requires alot more work. this is just a PoC anyway
        reinterpret_cast<decltype(&printf)>(memory->_printf)(memory->_fmt, reinterpret_cast<char**>(memory->_syscall_id_name_table)[call_info.syscall_id], call_info.return_address, call_info.return_value, call_info.thread_id, call_info.process_id);

    reinterpret_cast<decltype(&TerminateThread)>(memory->_TerminateThread)( reinterpret_cast<HANDLE>(-2)/*GetCurrentThread()*/, EXIT_SUCCESS );
}

void on_syscall_return_end() {};

// bounds
extern "C" __int64 asm_block_start; // marks start of asm stub
extern "C" __int64 asm_block_end; // marks end of asm stub

// variables
extern "C" HANDLE host_handle_value; // asm handle value
extern "C" uintptr_t host_callback_address;
extern "C" uintptr_t NtCreateThreadEx;
extern "C" uintptr_t NtAllocateVirtualMemory;

// function
extern "C" void* instrumentation_callback_stub(); // asm stub

int main( _In_ int argc, _In_ char* argv[] )
{
    // instrumented return (this is too op...) https://cdn.discordapp.com/attachments/765576637265739789/1125172825184018512/ida64_lQyI481Ke1.png
    // ApcState.Process has xrefs to NtSetInformationProcess https://cdn.discordapp.com/attachments/765576637265739789/1125362349818253312/ida64_S7qh5I6nMU.png

    // when you do a syscall, the syscall instruction saves the RIP of the next instruction in rcx, this is why we do `mov r10, rcx`
    // this is why KiFastSystemCall does
    // push rcx ; save return address
    // mov rcx, r10 ; restore arg 
    // https://cdn.discordapp.com/attachments/765576637265739789/1125499604847300689/ida64_iohb4B3w8n.png

    // https://cdn.discordapp.com/attachments/765576637265739789/1125499978563981383/ida64_l73WJRaigt.png

    // https://cdn.discordapp.com/attachments/765576637265739789/1125501946938265831/msedge_JAoQdMMkI9.png
    // -> https://cdn.discordapp.com/attachments/765576637265739789/1125502212743909407/ida64_QvZ1ejapBx.png

    // idk if i should keep these comments

    // app running as admin?
    if ( !token::adjust_token_privilege( SE_DEBUG_NAME, true ) )
    {
        std::printf( "app must run as administrator\n" );

        std::cin.get();
        return FALSE;
    }

    // user specified a process name?
    if ( argc != 2 )
    {
        std::printf( "please specify a process file name, e.g. notepad.exe\n" );
        
        std::cin.get();
        return FALSE;
    }

    // get ntdll
    static HMODULE ntdll = GetModuleHandleA( "ntdll.dll" );

    assert( ntdll );

    // get exported funcs
    static FARPROC _NtCreateThreadEx = GetProcAddress( ntdll, "NtCreateThreadEx" );
    static FARPROC _NtAllocateVirtualMemory = GetProcAddress( ntdll, "NtAllocateVirtualMemory" );

    // thread storage
    std::vector<std::thread> thread_storage{};

    // when we unlock this mutex, the threads will end
    std::mutex destroy_threads{};

    // lock it
    destroy_threads.lock();

    // load debugger
    process app{ argv[1] };

    for ( const process::instance_t& instance : app.get_instances() )
    {
        thread_storage.emplace_back( [&]() -> void
        {
            // duplicate handle to current process into target process
            HANDLE dup_handle_value{ 0 };

            DuplicateHandle( GetCurrentProcess(), GetCurrentProcess(), instance.process_handle, &dup_handle_value, NULL, FALSE, DUPLICATE_SAME_ACCESS );

            // calculate callback size
            const size_t callback_size = reinterpret_cast<uintptr_t>(&on_syscall_return_end) - reinterpret_cast<uintptr_t>(&on_syscall_return);

            // allocate memory for callback
            auto callback = VirtualAlloc( NULL, callback_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE );

            assert( callback );

            // copy function
            std::memcpy( callback, &on_syscall_return, callback_size );

            function_info_t function_info{ 
                "syscall: %s, return address: 0x%llX, return value: 0x%X, thread id: %u, process id: %u\n",
                instance.process_handle,
                &syscall_id_name_table,
                &printf,
                &ReadProcessMemory,
                &VirtualFreeEx,
                &TerminateThread
            }; // slightly better

            // look for -1
            uint8_t* current = reinterpret_cast<uint8_t*>(callback);
            
            while ( *reinterpret_cast<uint64_t*>(current) != static_cast<uint64_t>(0x1234567878563412) ) // this kind of sucks
                current++;

            // write handle (this shit is horrible)
            *reinterpret_cast<function_info_t**>(current) = &function_info;

            // copy handle into stub
            *reinterpret_cast<HANDLE*>(&host_handle_value) = dup_handle_value;

            // copy callback
            *reinterpret_cast<uintptr_t*>(&host_callback_address) = reinterpret_cast<uintptr_t>(callback);

            // copy exports
            *reinterpret_cast<uintptr_t*>(&NtCreateThreadEx) = reinterpret_cast<uintptr_t>(_NtCreateThreadEx);
            *reinterpret_cast<uintptr_t*>(&NtAllocateVirtualMemory) = reinterpret_cast<uintptr_t>(_NtAllocateVirtualMemory);

            // calculate block size
            const size_t asm_block_size = reinterpret_cast<uintptr_t>(&asm_block_end) - reinterpret_cast<uintptr_t>(&asm_block_start);

            // copy stub into target process
            auto memory = instance.allocate( asm_block_size, PAGE_EXECUTE_READWRITE );

            instance.write_process_memory( memory.get(), &asm_block_start, asm_block_size );

            // set instrumentation ptr to memory
            instance.set_instrumentation_callback( reinterpret_cast<char*>(memory.get()) + (reinterpret_cast<uintptr_t>(&instrumentation_callback_stub) - reinterpret_cast<uintptr_t>(&asm_block_start)) );

            // mutex is currently locked, so the current thread was to wait for it to unlock before we continue
            destroy_threads.lock();
            
            instance.set_instrumentation_callback( static_cast<__int64>(0) ); // unload hook

            std::printf( "unloading: %d\n",  instance.process_id );

            // everything in here is smart pointers, but icba to make one for virtual memory
            VirtualFree( callback, NULL, MEM_RELEASE );

            // kill next thread
            destroy_threads.unlock();
        } );
    }

    std::cin.get();

    // kill threads
    destroy_threads.unlock();

    // wait for threads to finish
    for ( std::thread& thread : thread_storage )
        thread.join();
}

// im probably not looking at this again

