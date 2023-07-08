/*
*	concept.hpp
* 
*	reusable concepts
*/

#pragma once

#include <concepts>

template <typename T>
concept integral_or_pointer = requires
{
	{ std::is_integral_v<T> || std::is_pointer_v<T> };
};

