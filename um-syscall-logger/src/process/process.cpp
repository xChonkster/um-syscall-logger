#include "./process.hpp"

#include "../utility/unique.hpp"

#include <TlHelp32.h>
#include <string>

process::process( _In_ const std::string& process_name )
{
	const auto snapshot = smart_handle{ CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL ) };

	PROCESSENTRY32 entry{ 0 };
	entry.dwSize = sizeof( PROCESSENTRY32 );

	Process32First( snapshot.get(), &entry );

	do
	{
		if ( !_strnicmp( entry.szExeFile, process_name.c_str(), process_name.length() ) )
		{
			// open handle
			const HANDLE process_handle = OpenProcess( PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID );

			// create new process instance
			instances.emplace_back( entry.th32ProcessID, process_handle );
		}
	} while ( Process32Next( snapshot.get(), &entry ) );
}

