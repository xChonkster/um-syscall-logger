/*
*	token.hpp
*
*	functions for managing the current access token
*/

#pragma once

#include <windows.h>
#include <stdint.h>

namespace token
{

bool adjust_token_privilege( _In_ const char* privelege, _In_ bool enabled );

} // namespace token

