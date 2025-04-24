#pragma once

#include "memoria_common.hpp"

#include <stdint.h>
#include <type_traits>

MEMORIA_BEGIN

template <typename CharT>
constexpr uint64_t FNV1a(const CharT *text, bool case_insensitive = true)
{
	static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>,
		"FNV1a function supports only char and wchar_t types.");

	const uint64_t prime = 0x00000100000001B3;
	uint64_t hash = 0xcbf29ce484222325;

	constexpr auto tolower_constexpr = [](char ch) -> char
		{
			return (ch >= 'A' && ch <= 'Z') ? (ch + 'a' - 'A') : ch;
		};

	constexpr auto towlower_constexpr = [](wchar_t ch) -> wchar_t
		{
			return (ch >= L'A' && ch <= L'Z') ? (ch + L'a' - L'A') : ch;
		};

	while (*text)
	{
		CharT ch = *text;
		if (case_insensitive)
		{
			if constexpr (std::is_same_v<CharT, char>)
			{
				ch = static_cast<CharT>(tolower_constexpr(static_cast<unsigned char>(ch)));
			}
			else if constexpr (std::is_same_v<CharT, wchar_t>)
			{
				ch = towlower_constexpr(ch);
			}
		}
		hash ^= static_cast<uint64_t>(ch);
		hash *= prime;
		++text;
	}

	return hash;
}

class fnv1a_t
{
private:
	uint64_t _value;

public:
	constexpr fnv1a_t() : _value(0) {}

	constexpr fnv1a_t(uint64_t val) : _value(val) {}

	template <typename CharT, size_t N>
	constexpr fnv1a_t(const CharT(&str)[N], bool case_insensitive = true)
		: _value(FNV1a(str, case_insensitive))
	{
	}

	constexpr operator uint64_t() const { return _value; }
};

MEMORIA_END