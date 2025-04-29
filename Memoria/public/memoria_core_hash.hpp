#pragma once

#include "memoria_common.hpp"

#include <stdint.h>
#include <type_traits>

MEMORIA_BEGIN

constexpr uint64_t FNV1A_PRIME = 0x00000100000001B3ull;
constexpr uint64_t FNV1A_BASIS = 0xcbf29ce484222325ull;

template <typename CharT, size_t N, bool CaseInsensitive = true>
constexpr uint64_t FNV1aConstexpr(const CharT(&text)[N])
{
    static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>);

    auto tolower_constexpr = [](char ch) constexpr -> char {
        return (ch >= 'A' && ch <= 'Z') ? (ch + 'a' - 'A') : ch;
        };

    auto towlower_constexpr = [](wchar_t ch) constexpr -> wchar_t {
        return (ch >= L'A' && ch <= L'Z') ? (ch + L'a' - L'A') : ch;
        };

    uint64_t hash = FNV1A_BASIS;

    for (size_t i = 0; i < N - 1; ++i)
    {
        CharT ch = text[i];
        if constexpr (CaseInsensitive)
        {
            if constexpr (std::is_same_v<CharT, char>)
                ch = tolower_constexpr(static_cast<unsigned char>(ch));
            else if constexpr (std::is_same_v<CharT, wchar_t>)
                ch = towlower_constexpr(ch);
        }
        hash ^= static_cast<uint64_t>(ch);
        hash *= FNV1A_PRIME;
    }

    return hash;
}

template <typename CharT>
uint64_t FNV1aRuntime(const CharT *text, bool case_insensitive = true)
{
    static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>);

    auto tolower_runtime = [](char ch) -> char {
        return (ch >= 'A' && ch <= 'Z') ? (ch + 'a' - 'A') : ch;
        };

    auto towlower_runtime = [](wchar_t ch) -> wchar_t {
        return (ch >= L'A' && ch <= L'Z') ? (ch + L'a' - L'A') : ch;
        };

    uint64_t hash = FNV1A_BASIS;

    while (*text)
    {
        CharT ch = *text;
        if (case_insensitive)
        {
            if constexpr (std::is_same_v<CharT, char>)
                ch = tolower_runtime(static_cast<unsigned char>(ch));
            else if constexpr (std::is_same_v<CharT, wchar_t>)
                ch = towlower_runtime(ch);
        }
        hash ^= static_cast<uint64_t>(ch);
        hash *= FNV1A_PRIME;
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

    template <typename CharT, size_t N, bool CaseInsensitive = true>
    constexpr fnv1a_t(const CharT(&str)[N])
        : _value(Memoria::FNV1aConstexpr<CharT, N, CaseInsensitive>(str))
    {
    }

    template <typename CharT>
    fnv1a_t(const CharT *str, bool case_insensitive = true)
        : _value(Memoria::FNV1aRuntime(str, case_insensitive))
    {
    }

    constexpr operator uint64_t() const { return _value; }
};

MEMORIA_END