/*
*   um-syscall-logger.cpp
* 
*   ...
*/

#include <windows.h>
#include <iostream>
#include <cassert>
#include <deque>
#include <mutex>
#include <thread>

// instrumentation callback info struct
typedef struct _INSTRUMENTATION_CALLBACK_INFORMATION
{
    //ULONG Version; // cant not be zero bc it gets checked against reserved...
    //ULONG Reserved; // always zero
    PVOID Callback; // https://cdn.discordapp.com/attachments/765576637265739789/1125805954894680094/ida64_rEHu1Dzm9a.png
} INSTRUMENTATION_CALLBACK_INFORMATION, * PINSTRUMENTATION_CALLBACK_INFORMATION;// https://cdn.discordapp.com/attachments/765576637265739789/1125805460050694235/ida64_6dOZkqVu4q.png 

// NtSetInformationProcess typedef
typedef NTSTATUS( __stdcall* NtSetInformationProcess )
(
    _In_ HANDLE hProcess,
    _In_ PROCESS_INFORMATION_CLASS ProcessInformationClass,
    LPVOID ProcessInformation,
    _In_ DWORD ProcessInformationSize
);

bool debounce = false;

// callback function
extern "C" void ON_SYSCALL_RETURN_IMPL( _In_ uint8_t* return_address,
                                        _In_ uint64_t return_value )
{
    // so at this point we have to look for the syscall id, now we'll just be looking for `mov eax, ...` or `mov rax, ...` (even tho this already doesnt happen)
    // your target app might implement manual syscalls, and might implement them differently in which case this function will fail
    // we'll also be using a static array of syscalls, which may not work for your version of kernel (you could use nt symbols and disassemble each "Nt" stub in ntdll etc etc but this is just a small project)

    if ( debounce )
        return;

    uint32_t syscall_id = 0u;

    for ( uint32_t count = 0u; count <= 100; count++, return_address-- ) // furthest we go back is 100 bytes (which is already far)
    {
        if ( return_address[0] == 0xB8 )
        {
            syscall_id = *reinterpret_cast<uint32_t*>(&return_address[1]);

            break;
        } // 0xB8 = mov eax, ...
        else if ( return_address[0] == 0x48 && return_address[1] == 0xC7 && return_address[2] == 0xC0 )
        {
            syscall_id = *reinterpret_cast<uint32_t*>(&return_address[3]);

            break;
        } // 0x48 0xC7 0xC0 = mov rax, ...
    }

    // do a syscall lookup (https://github.com/j00ru/windows-syscalls/blob/master/x64/json/nt-per-system.json)
    if ( syscall_id )
    {
        debounce = true;
        // k fuck this we need threads
    }
}

extern "C" void* ON_SYSCALL_RETURN(); // asm stub

int main()
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

    INSTRUMENTATION_CALLBACK_INFORMATION ICI{ &ON_SYSCALL_RETURN };

    const HMODULE ntdll = LoadLibraryA( "ntdll.dll" );

    assert( ntdll );

    const NtSetInformationProcess _NtSetInformationProcess = reinterpret_cast<NtSetInformationProcess>(GetProcAddress( ntdll, "NtSetInformationProcess" ));

    NTSTATUS Status = _NtSetInformationProcess( GetCurrentProcess(), static_cast<PROCESS_INFORMATION_CLASS>(40), &ICI, sizeof( INSTRUMENTATION_CALLBACK_INFORMATION ) );

    //std::printf( "error: %X", Status );
    
    std::free( std::malloc( 0x1000 ) );

    std::this_thread::sleep_for( std::chrono::years( 1 ) ); // that'll do i think
}

