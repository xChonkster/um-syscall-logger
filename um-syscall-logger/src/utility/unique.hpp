/*
*	unique.hpp
* 
*	smart pointers
*/

#pragma once

#include <windows.h>
#include <memory>

// holds numbers
template <auto T>
using constant_t = std::integral_constant<decltype(T), T>;

// unique_ptr for handles
using smart_handle = std::unique_ptr<std::remove_pointer_t<HANDLE>, constant_t<CloseHandle>>;

