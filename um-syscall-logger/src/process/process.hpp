/*
*	process.hpp
* 
*	class for managing processes
*/

#pragma once

#include "../utility/unique.hpp"
#include "../utility/concept.hpp"

#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

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

class process
{
public:
	process( _In_ const std::string& process_name );

	~process() = default;

public:
	// struct (kind of class) for process instance
	struct instance_t
	{
	public:
		const DWORD process_id;
		const HANDLE process_handle;

	public:
		instance_t( _In_ DWORD _process_id, _In_ HANDLE _process_handle )
			: process_id( _process_id ), process_handle( _process_handle )
		{}

		~instance_t()
		{};

	public:
		inline auto allocate( _In_ size_t size, _In_ DWORD protection ) const
		{
			const auto deleter = [&]( void* memory ) -> void
			{
				VirtualFreeEx(process_handle, memory, NULL, MEM_RELEASE );
			};

			void* memory = VirtualAllocEx( process_handle, NULL, size, MEM_RESERVE | MEM_COMMIT, protection );

			return std::unique_ptr<std::remove_pointer_t<void*>, decltype(deleter)>{ memory, deleter };
		}

		template <typename T>
		constexpr int read_process_memory( _In_ const integral_or_pointer auto address, _Out_ T* buffer, _In_ size_t size ) const
		{
			return ReadProcessMemory( process_handle, reinterpret_cast<const void*>(static_cast<__int64>(address)), reinterpret_cast<void*>(buffer), size, nullptr );
		}

		template <typename T>
		constexpr int write_process_memory( _In_ integral_or_pointer auto address, _In_ const T* buffer, _In_ size_t size ) const
		{
			return WriteProcessMemory( process_handle, reinterpret_cast<void*>(address), reinterpret_cast<const void*>(buffer), size, nullptr );
		}

		constexpr NTSTATUS set_instrumentation_callback( _In_ integral_or_pointer auto address ) const
		{
			// get handle to ntdll
			static const HMODULE ntdll = GetModuleHandleA( "ntdll.dll" );

			assert( ntdll );

			// get address of export
			static const NtSetInformationProcess _NtSetInformationProcess = reinterpret_cast<NtSetInformationProcess>(GetProcAddress( ntdll, "NtSetInformationProcess" ));

			// set up & do the call
			INSTRUMENTATION_CALLBACK_INFORMATION ICI{ reinterpret_cast<PVOID>(address) };

			return _NtSetInformationProcess( process_handle, static_cast<PROCESS_INFORMATION_CLASS>(40), &ICI, sizeof( INSTRUMENTATION_CALLBACK_INFORMATION ) );
		}
	};

	// returns instance_t[]
	const std::vector<instance_t>& get_instances() const
	{
		return instances;
	}

private:
	std::vector<instance_t> instances{};
};
