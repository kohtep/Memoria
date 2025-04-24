#pragma once

#include "memoria_common.hpp"

MEMORIA_BEGIN

class Boolean
{
private:
    bool value;

public:
    explicit constexpr Boolean(bool val) noexcept
        : value(val)
    {
    }

    constexpr Boolean(const Boolean &) noexcept = default;
    constexpr Boolean &operator=(const Boolean &) noexcept = default;

    explicit constexpr operator bool() const noexcept
    {
        return value;
    }

    constexpr Boolean operator!() const noexcept
    {
        return Boolean(!value);
    }

    friend constexpr bool operator==(Boolean lhs, Boolean rhs) noexcept
    {
        return lhs.value == rhs.value;
    }

    friend constexpr bool operator!=(Boolean lhs, Boolean rhs) noexcept
    {
        return lhs.value != rhs.value;
    }

    friend constexpr Boolean operator&&(Boolean lhs, Boolean rhs) noexcept
    {
        return Boolean(lhs.value && rhs.value);
    }

    friend constexpr Boolean operator||(Boolean lhs, Boolean rhs) noexcept
    {
        return Boolean(lhs.value || rhs.value);
    }

    Boolean operator&(const Boolean &) const = delete;
    Boolean operator|(const Boolean &) const = delete;
    Boolean operator^(const Boolean &) const = delete;
    Boolean operator~() const = delete;
    Boolean operator<<(int) const = delete;
    Boolean operator>>(int) const = delete;
};

static_Assert(sizeof(Boolean) == sizeof(bool));

MEMORIA_END

#define bool Memoria::Boolean
#define true Memoria::Boolean(true)
#define false Memoria::Boolean(false)