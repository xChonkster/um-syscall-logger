#include "./token.hpp"

namespace token
{

bool adjust_token_privilege( _In_ const char* privelege, _In_ bool enabled )
{
	HANDLE access_token{ 0 }; // need to open our own token because GetCurrentProcessToken() doesnt have required priv
	TOKEN_PRIVILEGES new_privelege{ 0 };

	bool success = false;

	if ( OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &access_token ) && LookupPrivilegeValueA( NULL, privelege, &new_privelege.Privileges[0].Luid ) )
	{
		// set up the new state of the token
		new_privelege.PrivilegeCount = 1;
		new_privelege.Privileges[0].Attributes = enabled ? SE_PRIVILEGE_ENABLED : NULL;

		// adjust the priv
		success = AdjustTokenPrivileges( access_token, FALSE, &new_privelege, NULL, nullptr, nullptr );
	}

	CloseHandle( access_token );

	return success;
}

} // namespace token

