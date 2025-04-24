//
// memoria_utils_string.hpp
// 
// Basic memory and string helpers to eliminate dependency on CRT.
//
// Some functions attempt to avoid using string pointers
// to enhance the confidentiality of their own implementation,
// placing strings directly in IMM values of assembly instructions.
//
// The behavior of the functions may differ from the original — 
// certain features may be implemented in their own way.
//

#pragma once

#include "memoria_common.hpp"

#include <vadefs.h>

MEMORIA_BEGIN

#undef CopyMemory
#undef MoveMemory
#undef FillMemory

// strchr
extern char *FindCharA(const char *lpString, int ch);

// strrchr
extern char *FindLastCharA(const char *lpString, int ch);

// memcmp
extern int CompareMemory(const void *lpBlock1, const void *lpBlock2, size_t dwSize);

// memcpy
extern void *CopyMemory(void *lpDestination, const void *lpSource, size_t dwSize);

// memmove
extern void *MoveMemory(void *lpDestination, const void *lpSource, size_t dwSize);

// memset
extern void *FillMemory(void *lpDestination, int nValue, size_t dwSize);

// strcpy (Ansi)
extern char *StrCopyA(char *lpDest, const char *lpSrc);

// strcpy (Wide)
extern wchar_t *StrCopyW(wchar_t *lpDest, const wchar_t *lpSrc);

// strncpy (Ansi)
extern char *StrNCopyA(char *lpDest, const char *lpSrc, size_t dwCount);

// strncpy (Wide)
extern wchar_t *StrNCopyW(wchar_t *lpDest, const wchar_t *lpSrc, size_t dwCount);

// strcpy_s (Ansi)
extern int StrCopySafeA(char *lpDest, size_t dwDestSize, const char *lpSrc);

// strcpy_s (Wide)
extern int StrCopySafeW(wchar_t *lpDest, size_t dwDestSize, const wchar_t *lpSrc);

// strncpy_s (Ansi)
extern int StrNCopySafeA(char *lpDest, size_t dwDestSize, const char *lpSrc, size_t dwMaxLen);

// strncpy_s (Wide)
extern int StrNCopySafeW(wchar_t *lpDest, size_t dwDestSize, const wchar_t *lpSrc, size_t dwMaxLen);

// strcmp (Ansi)
extern int StrCompA(const char *lpStr1, const char *lpStr2);

// strcmp (Wide)
extern int StrCompW(const wchar_t *lpStr1, const wchar_t *lpStr2);

// strncmp (Ansi)
extern int StrNCompA(const char *lpStr1, const char *lpStr2, size_t dwCount);

// strncmp (Wide)
extern int StrNCompW(const wchar_t *lpStr1, const wchar_t *lpStr2, size_t dwCount);

// stricmp (Ansi)
extern int StrICompA(const char *lpStr1, const char *lpStr2);

// stricmp (Wide)
extern int StrICompW(const wchar_t *lpStr1, const wchar_t *lpStr2);

// strcat (Ansi)
extern char *StrCatA(char *lpDest, const char *lpSrc);

// strcat (Wide)
extern wchar_t *StrCatW(wchar_t *lpDest, const wchar_t *lpSrc);

// strncat (Ansi)
extern char *StrNCatA(char *lpDest, const char *lpSrc, size_t dwCount);

// strncat (Wide)
extern wchar_t *StrNCatW(wchar_t *lpDest, const wchar_t *lpSrc, size_t dwCount);

// strcat_s (Ansi)
extern int StrCatSafeA(char *lpDest, size_t dwDestSize, const char *lpSrc);

// strcat_s (Wide)
extern int StrCatSafeW(wchar_t *lpDest, size_t dwDestSize, const wchar_t *lpSrc);

// strncat_s (Ansi)
extern int StrNCatSafeA(char *lpDest, size_t dwDestSize, const char *lpSrc, size_t dwCount);

// strncat_s (Wide)
extern int StrNCatSafeW(wchar_t *lpDest, size_t dwDestSize, const wchar_t *lpSrc, size_t dwCount);

// vsprintf_s
extern int FormatBufSafeV(char *lpBuffer, size_t dwMaxSize, const char *lpFormat, va_list args);

// sprintf_s
extern int FormatBufSafe(char *lpBuffer, size_t dwMaxSize, const char *lpFormat, ...);

// std::to_string(int)
extern int IntToStr(char *lpBuffer, int nValue);

// std::to_string(unsigned int)
extern int UIntToStr(char *lpBuffer, unsigned int uValue);

// std::stringstream & std::hex
extern int UIntToHex(char *lpBuffer, unsigned int uValue);

// std::to_string(long long)
extern int Int64ToStr(char *lpBuffer, long long nValue);

// std::to_string(unsigned long long)
extern int UInt64ToStr(char *lpBuffer, unsigned long long uValue);

// std::stringstream & std::hex
extern int UInt64ToHex(char *lpBuffer, unsigned long long uValue, bool uppercase);

// std::to_string(double)
extern int DoubleToStr(char *lpBuffer, double fValue, int nPrecision);

// std::isnan
extern bool IsNan(double val);

// std::isinf
extern bool IsInf(double val);

// strlen
extern size_t StrLenA(const char *lpString);

// wcslen
extern size_t StrLenW(const wchar_t *lpString);

MEMORIA_END

// sprintf
#define FormatBuf(buffer, fmt, ...) Memoria::FormatBufSafe(buffer, static_cast<size_t>(-1), fmt, __VA_ARGS__)

MEMORIA_BEGIN



MEMORIA_END