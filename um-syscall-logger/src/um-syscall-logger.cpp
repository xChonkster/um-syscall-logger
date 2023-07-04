/*
*   um-syscall-logger.cpp
* 
*   ...
*/

#include <windows.h>
#include <iostream>
#include <cassert>

typedef struct _INSTRUMENTATION_CALLBACK_INFORMATION
{
    //ULONG Version; // cant not be zero bc it gets checked against reserved...
    //ULONG Reserved; // always zero
    PVOID Callback; // https://cdn.discordapp.com/attachments/765576637265739789/1125805954894680094/ida64_rEHu1Dzm9a.png
} INSTRUMENTATION_CALLBACK_INFORMATION, * PINSTRUMENTATION_CALLBACK_INFORMATION;// https://cdn.discordapp.com/attachments/765576637265739789/1125805460050694235/ida64_6dOZkqVu4q.png 

typedef NTSTATUS( __stdcall* NtSetInformationProcess )
(
    _In_ HANDLE hProcess,
    _In_ PROCESS_INFORMATION_CLASS ProcessInformationClass,
    LPVOID ProcessInformation,
    _In_ DWORD ProcessInformationSize
);

extern "C" __int64 ON_SYSCALL_RETURN_IMPL(__int64 return_address, __int64 return_value)
{
    int a = 5;
    
    a += 10;

    return a;
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

    std::printf( "error: %X", Status );
}

