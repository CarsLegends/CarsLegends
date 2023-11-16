#ifndef MACROS_HASH_HPP
#define MACROS_HASH_HPP

#include <cstdint>
#include <cstddef>

namespace Macros
{
	constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
	{
		return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
	}

	constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
	{
		return fnv1a_32(s, count);
	}
}
#endif