#pragma once

#include "memoria_common.hpp"

#include "memoria_core_signature.hpp"

#include <stdint.h>
#include <string_view>
#include <string>

MEMORIA_BEGIN

/**
 * @brief Compares a byte at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckU8(const void *addr, uint8_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares an unsigned word (2 bytes) at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckU16(const void *addr, uint16_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares 3 bytes at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @note Although the function accepts a double word (4 bytes), only the lower
 *       24 bits (00FFFFFFh) will be checked.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckU24(const void *addr, uint32_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares 3 bytes at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @note The byte order in the array must conform to Little-endian.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckU24(const void *addr, const uint8_t value[3], ptrdiff_t offset = 0);

/**
 * @brief Compares an unsigned double word (4 bytes) at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckU32(const void *addr, uint32_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares an unsigned quad word (8 bytes) at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckU64(const void *addr, uint64_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares a character (1 signed byte) at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckI8(const void *addr, int8_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares a signed word (2 bytes) at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckI16(const void *addr, int16_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares 3 bytes at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @note Although the function accepts a double word (4 bytes), only the lower
 *       24 bits (00FFFFFFh) will be checked.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckI24(const void *addr, int32_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares 3 signed bytes at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @note The byte order in the array must conform to Little-endian.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckI24(const void *addr, const int8_t value[3], ptrdiff_t offset = 0);

/**
 * @brief Compares a signed double word (4 bytes) at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckI32(const void *addr, int32_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares a signed quad word (8 bytes) at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckI64(const void *addr, int64_t value, ptrdiff_t offset = 0);

/**
 * @brief Compares a single-precision floating-point number (4 bytes) at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckFloat(const void *addr, float value, ptrdiff_t offset = 0);

/**
 * @brief Compares a double-precision floating-point number (8 bytes) at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value Value to compare against.
 * @param offset Preliminary offset relative to 'addr'.
 *
 * @return true if the value at the specified address is identical to 'value', false otherwise.
 */
extern bool CheckDouble(const void *addr, double value, ptrdiff_t offset = 0);

/**
 * @brief Compares an ANSI string at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value The string value to compare with.
 * @param offset Preliminary offset relative to 'addr' before comparison.
 *
 * @note The null-terminator is not considered in comparison, as std::string_view
 *       stores a pointer and size, not a null-terminated string. This means the
 *       comparison length is strictly defined by value.size().
 *
 *       Examples:
 *         std::string_view{"test"}       // size() == 4, does NOT include '\0'
 *         std::string_view{"test\0abc", 7} // size() == 7, includes null and part of "abc"
 *
 * @return true if the memory content at the given address matches 'value', false otherwise.
 */
extern bool CheckAStr(const void *addr, const std::string_view &value, ptrdiff_t offset = 0);

/**
 * @brief Compares a wide-character (UTF-16/UTF-32) string at the specified memory address.
 *
 * @param addr Memory address to check.
 * @param value The wide string value to compare with.
 * @param offset Optional offset relative to 'addr' before comparison (in wchar_t).
 *
 * @note The null-terminator is not considered in comparison, as std::wstring_view
 *       holds a pointer and a length (in wchar_t elements), not a null-terminated string.
 *       This means that comparison is strictly limited to value.size() * sizeof(wchar_t) bytes.
 *
 *       Examples:
 *         std::wstring_view{L"test"}              // size() == 4, does NOT include L'\0'
 *         std::wstring_view{L"test\0xyz", 8}       // size() == 8, includes null and part of "xyz"

 * @return true if the memory content at the given address matches 'value', false otherwise.
 */
extern bool CheckWStr(const void *addr, const std::wstring_view &value, ptrdiff_t offset = 0);

extern bool CheckSignature(const void *addr, const CSignature &value, ptrdiff_t offset = 0);
extern bool CheckSignature(const void *addr, const std::string_view &value, ptrdiff_t offset = 0);

MEMORIA_END