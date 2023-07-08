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
    uint32_t return_value;
    uint32_t syscall_id;
    uint32_t thread_id;
    uint32_t process_id;
};

struct function_info_t
{
    HANDLE _process_handle;
    uintptr_t _syscall_id_name_table;
    const char* _format_str;
    uintptr_t _printf;
    uintptr_t _rpm;
};

// callback function
void ON_SYSCALL_RETURN_CALLBACK( uint64_t call_info_address )
{
    const volatile function_info_t* memory = reinterpret_cast<function_info_t*>(0x1234567878563412); // volatile bc fuck you optimizer

    const HANDLE _process_handle = memory->_process_handle;
    const uintptr_t _syscall_id_name_table = memory->_syscall_id_name_table;
    const char* _format_str = memory->_format_str; // this shit SUCKS
    const uintptr_t _printf = memory->_printf; 
    const uintptr_t _rpm = memory->_rpm;
    // this is a horrible solution for a problem i legit didnt think about, gg

    call_info_t call_info;

    reinterpret_cast<decltype(&ReadProcessMemory)>(_rpm)( _process_handle, reinterpret_cast<void*>(call_info_address), &call_info, sizeof( call_info_t ), nullptr );
    
    reinterpret_cast<decltype(&printf)>(_printf)(_format_str, reinterpret_cast<char**>(_syscall_id_name_table)[call_info.syscall_id], call_info.return_value, call_info.thread_id, call_info.process_id);
}

void ON_SYSCALL_RETURN_CALLBACK_END() {};

// bounds
extern "C" __int64 ASM_BLOCK_START; // marks start of asm stub
extern "C" __int64 ASM_BLOCK_END; // marks end of asm stub

// variables
extern "C" HANDLE HOST_HANDLE_VALUE; // asm handle value
extern "C" uintptr_t HOST_CALLBACK_ADDRESS;
extern "C" uintptr_t NtCreateThreadEx;
extern "C" uintptr_t NtWaitForSingleObject;

// function
extern "C" void* ON_SYSCALL_RETURN(); // asm stub

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
    static FARPROC _NtWaitForSingleObject = GetProcAddress( ntdll, "NtWaitForSingleObject" );

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
            const size_t callback_size = reinterpret_cast<uintptr_t>(&ON_SYSCALL_RETURN_CALLBACK_END) - reinterpret_cast<uintptr_t>(&ON_SYSCALL_RETURN_CALLBACK);

            // allocate memory for callback
            auto callback = VirtualAlloc( NULL, callback_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE );

            assert( callback );

            // copy function
            std::memcpy( callback, &ON_SYSCALL_RETURN_CALLBACK, callback_size );

            function_info_t function_info{ instance.process_handle, reinterpret_cast<uintptr_t>(&syscall_id_name_table), "syscall: %s, return value: %u, thread id: %u, process id: %u\n", reinterpret_cast<uintptr_t>(&printf), reinterpret_cast<uintptr_t>(&ReadProcessMemory) }; // this is a disgrace

            // look for -1
            uint8_t* current = reinterpret_cast<uint8_t*>(callback);
            
            while ( *reinterpret_cast<uint64_t*>(current) != static_cast<uint64_t>(0x1234567878563412) ) // this kind of sucks
                current++;

            // write handle (this shit is horrible)
            *reinterpret_cast<function_info_t**>(current) = &function_info;

            // copy handle into stub
            *reinterpret_cast<HANDLE*>(&HOST_HANDLE_VALUE) = dup_handle_value;

            // copy callback
            *reinterpret_cast<uintptr_t*>(&HOST_CALLBACK_ADDRESS) = reinterpret_cast<uintptr_t>(callback);

            // copy exports
            *reinterpret_cast<uintptr_t*>(&NtCreateThreadEx) = reinterpret_cast<uintptr_t>(_NtCreateThreadEx);
            *reinterpret_cast<uintptr_t*>(&NtWaitForSingleObject) = reinterpret_cast<uintptr_t>(_NtWaitForSingleObject);

            // calculate block size
            const size_t asm_block_size = reinterpret_cast<uintptr_t>(&ASM_BLOCK_END) - reinterpret_cast<uintptr_t>(&ASM_BLOCK_START);

            // copy stub into target process
            auto memory = instance.allocate( asm_block_size, PAGE_EXECUTE_READWRITE );

            instance.write_process_memory( memory.get(), &ASM_BLOCK_START, asm_block_size );

            // set instrumentation ptr to memory
            instance.set_instrumentation_callback( reinterpret_cast<char*>(memory.get()) + (reinterpret_cast<uintptr_t>(&ON_SYSCALL_RETURN) - reinterpret_cast<uintptr_t>(&ASM_BLOCK_START)) );

            // mutex is currently locked, so the current thread was to wait for it to unlock before we continue
            destroy_threads.lock();
            
            instance.set_instrumentation_callback( static_cast<__int64>(0) ); // unload hook

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

