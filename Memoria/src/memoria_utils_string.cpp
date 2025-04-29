#include "memoria_utils_string.hpp"

#include <stdarg.h>
#include <stdint.h>
#include <Windows.h>

#include "memoria_utils_assert.hpp"

MEMORIA_BEGIN

char *FindCharA(const char *lpString, int ch)
{
	while (*lpString != '\0')
	{
		if (*lpString == ch)
			return const_cast<char *>(lpString);

		++lpString;
	}

	if (ch == '\0')
		return const_cast<char *>(lpString);

	return nullptr;
}

char *FindLastCharA(const char *lpString, int ch)
{
	const char *lpResult = nullptr;

	while (*lpString != '\0')
	{
		if (*lpString == ch)
			lpResult = lpString;

		++lpString;
	}

	return const_cast<char *>(lpResult);
}

int MemCompare(const void *lpBlock1, const void *lpBlock2, size_t dwSize)
{
	const unsigned char *p1 = static_cast<const unsigned char *>(lpBlock1);
	const unsigned char *p2 = static_cast<const unsigned char *>(lpBlock2);

	for (size_t i = 0; i < dwSize; ++i)
	{
		if (p1[i] != p2[i])
			return static_cast<int>(p1[i]) - static_cast<int>(p2[i]);
	}

	return 0;
}

void *MemCopy(void *lpDestination, const void *lpSource, size_t dwSize)
{
	unsigned char *dest = static_cast<unsigned char *>(lpDestination);
	const unsigned char *src = static_cast<const unsigned char *>(lpSource);

	for (size_t i = 0; i < dwSize; ++i)
	{
		dest[i] = src[i];
	}

	return lpDestination;
}

void *MemMove(void *lpDestination, const void *lpSource, size_t dwSize)
{
	unsigned char *dest = static_cast<unsigned char *>(lpDestination);
	const unsigned char *src = static_cast<const unsigned char *>(lpSource);

	if (src < dest && dest < src + dwSize)
	{
		for (size_t i = dwSize; i-- > 0;)
		{
			dest[i] = src[i];
		}
	}
	else
	{
		for (size_t i = 0; i < dwSize; ++i)
		{
			dest[i] = src[i];
		}
	}

	return lpDestination;
}

void *MemFill(void *lpDestination, int nValue, size_t dwSize)
{
	// `volatile` helps prevent insertion of a `memset` call from the CRT library.
	volatile unsigned char *dest = static_cast<unsigned char *>(lpDestination);

	for (size_t i = 0; i < dwSize; ++i)
	{
		dest[i] = static_cast<unsigned char>(nValue);
	}

	return lpDestination;
}

char *StrCopyA(char *lpDest, const char *lpSrc)
{
	char *lpResult = lpDest;

	while ((*lpDest++ = *lpSrc++) != '\0')
	{
		;
	}
	
	return lpResult;
}

char *StrNCopyA(char *lpDest, const char *lpSrc, size_t dwMaxLen)
{
	char *lpResult = lpDest;
	size_t len = 0;

	while (len < dwMaxLen && lpSrc[len] != '\0')
	{
		lpDest[len] = lpSrc[len];
		++len;
	}

	lpDest[len] = '\0';
	return lpResult;
}

int StrCompA(const char *lpStr1, const char *lpStr2)
{
	while (*lpStr1 && (*lpStr1 == *lpStr2))
	{
		++lpStr1;
		++lpStr2;
	}
	return static_cast<unsigned char>(*lpStr1) - static_cast<unsigned char>(*lpStr2);
}

int StrNCompA(const char *lpStr1, const char *lpStr2, size_t dwCount)
{
	while (dwCount && *lpStr1 && (*lpStr1 == *lpStr2))
	{
		++lpStr1;
		++lpStr2;
		--dwCount;
	}

	if (dwCount == 0)
		return 0;

	return static_cast<unsigned char>(*lpStr1) - static_cast<unsigned char>(*lpStr2);
}

int StrICompA(const char *lpStr1, const char *lpStr2)
{
	while (*lpStr1 && *lpStr2)
	{
		char ch1 = (*lpStr1 >= 'A' && *lpStr1 <= 'Z') ? *lpStr1 + 32 : *lpStr1;
		char ch2 = (*lpStr2 >= 'A' && *lpStr2 <= 'Z') ? *lpStr2 + 32 : *lpStr2;

		if (ch1 != ch2)
			return static_cast<unsigned char>(ch1) - static_cast<unsigned char>(ch2);

		++lpStr1;
		++lpStr2;
	}
	return static_cast<unsigned char>(*lpStr1) - static_cast<unsigned char>(*lpStr2);
}

wchar_t *StrCopyW(wchar_t *lpDest, const wchar_t *lpSrc)
{
	wchar_t *lpResult = lpDest;

	while ((*lpDest++ = *lpSrc++) != L'\0')
	{
		;
	}

	return lpResult;
}

wchar_t *StrNCopyW(wchar_t *lpDest, const wchar_t *lpSrc, size_t dwMaxLen)
{
	wchar_t *lpResult = lpDest;
	size_t len = 0;

	while (len < dwMaxLen && lpSrc[len] != L'\0')
	{
		lpDest[len] = lpSrc[len];
		++len;
	}

	lpDest[len] = L'\0';
	return lpResult;
}

int StrCopySafeA(char *lpDest, size_t dwDestSize, const char *lpSrc)
{
	size_t i = 0;

	while (i + 1 < dwDestSize && lpSrc[i] != '\0')
	{
		lpDest[i] = lpSrc[i];
		++i;
	}

	if (lpSrc[i] != '\0')
		return 1;

	lpDest[i] = '\0';
	return 0;
}

int StrCopySafeW(wchar_t *lpDest, size_t dwDestSize, const wchar_t *lpSrc)
{
	size_t i = 0;

	while (i + 1 < dwDestSize && lpSrc[i] != L'\0')
	{
		lpDest[i] = lpSrc[i];
		++i;
	}

	if (lpSrc[i] != L'\0')
		return 1;

	lpDest[i] = L'\0';
	return 0;
}

int StrNCopySafeA(char *lpDest, size_t dwDestSize, const char *lpSrc, size_t dwMaxLen)
{
	size_t i = 0;

	while (i < dwMaxLen && i + 1 < dwDestSize && lpSrc[i] != '\0')
	{
		lpDest[i] = lpSrc[i];
		++i;
	}

	if (i == dwDestSize)
		return 1;

	lpDest[i] = '\0';
	return 0;
}

int StrNCopySafeW(wchar_t *lpDest, size_t dwDestSize, const wchar_t *lpSrc, size_t dwMaxLen)
{
	size_t i = 0;

	while (i < dwMaxLen && i + 1 < dwDestSize && lpSrc[i] != L'\0')
	{
		lpDest[i] = lpSrc[i];
		++i;
	}

	if (i == dwDestSize)
		return 1;

	lpDest[i] = L'\0';
	return 0;
}

char *StrCatA(char *lpDest, const char *lpSrc)
{
	char *lpResult = lpDest;

	while (*lpDest)
		lpDest++;

	while ((*lpDest++ = *lpSrc++) != '\0')
	{
		;
	}

	return lpResult;
}

wchar_t *StrCatW(wchar_t *lpDest, const wchar_t *lpSrc)
{
	wchar_t *lpResult = lpDest;

	while (*lpDest)
		lpDest++;

	while ((*lpDest++ = *lpSrc++) != L'\0')
	{
		;
	}

	return lpResult;
}

char *StrNCatA(char *lpDest, const char *lpSrc, size_t dwCount)
{
	char *lpResult = lpDest;

	while (*lpDest)
		lpDest++;

	size_t i = 0;
	while (i < dwCount && lpSrc[i] != '\0')
	{
		lpDest[i] = lpSrc[i];
		++i;
	}

	lpDest[i] = '\0';
	return lpResult;
}

wchar_t *StrNCatW(wchar_t *lpDest, const wchar_t *lpSrc, size_t dwCount)
{
	wchar_t *lpResult = lpDest;

	while (*lpDest)
		lpDest++;

	size_t i = 0;
	while (i < dwCount && lpSrc[i] != L'\0')
	{
		lpDest[i] = lpSrc[i];
		++i;
	}

	lpDest[i] = L'\0';
	return lpResult;
}

int StrCatSafeA(char *lpDest, size_t dwDestSize, const char *lpSrc)
{
	size_t destLen = 0;

	while (destLen < dwDestSize && lpDest[destLen] != '\0')
		destLen++;

	if (destLen == dwDestSize)
		return 1;

	size_t i = 0;
	while ((destLen + i + 1) < dwDestSize && lpSrc[i] != '\0')
	{
		lpDest[destLen + i] = lpSrc[i];
		++i;
	}

	if ((destLen + i) >= dwDestSize)
		return 1;

	lpDest[destLen + i] = '\0';
	return 0;
}

int StrCatSafeW(wchar_t *lpDest, size_t dwDestSize, const wchar_t *lpSrc)
{
	size_t destLen = 0;

	while (destLen < dwDestSize && lpDest[destLen] != L'\0')
		destLen++;

	if (destLen == dwDestSize)
		return 1;

	size_t i = 0;
	while ((destLen + i + 1) < dwDestSize && lpSrc[i] != L'\0')
	{
		lpDest[destLen + i] = lpSrc[i];
		++i;
	}

	if ((destLen + i) >= dwDestSize)
		return 1;

	lpDest[destLen + i] = L'\0';
	return 0;
}

int StrNCatSafeA(char *lpDest, size_t dwDestSize, const char *lpSrc, size_t dwCount)
{
	size_t destLen = 0;

	while (destLen < dwDestSize && lpDest[destLen] != '\0')
		destLen++;

	if (destLen == dwDestSize)
		return 1;

	size_t i = 0;
	while (i < dwCount && (destLen + i + 1) < dwDestSize && lpSrc[i] != '\0')
	{
		lpDest[destLen + i] = lpSrc[i];
		++i;
	}

	if ((destLen + i) >= dwDestSize)
		return 1;

	lpDest[destLen + i] = '\0';
	return 0;
}

int StrNCatSafeW(wchar_t *lpDest, size_t dwDestSize, const wchar_t *lpSrc, size_t dwCount)
{
	size_t destLen = 0;

	while (destLen < dwDestSize && lpDest[destLen] != L'\0')
		destLen++;

	if (destLen == dwDestSize)
		return 1;

	size_t i = 0;
	while (i < dwCount && (destLen + i + 1) < dwDestSize && lpSrc[i] != L'\0')
	{
		lpDest[destLen + i] = lpSrc[i];
		++i;
	}

	if ((destLen + i) >= dwDestSize)
		return 1;

	lpDest[destLen + i] = L'\0';
	return 0;
}

int StrCompW(const wchar_t *lpStr1, const wchar_t *lpStr2)
{
	while (*lpStr1 && (*lpStr1 == *lpStr2))
	{
		++lpStr1;
		++lpStr2;
	}

	return static_cast<unsigned int>(*lpStr1) - static_cast<unsigned int>(*lpStr2);
}

int StrNCompW(const wchar_t *lpStr1, const wchar_t *lpStr2, size_t dwCount)
{
	while (dwCount && *lpStr1 && (*lpStr1 == *lpStr2))
	{
		++lpStr1;
		++lpStr2;
		--dwCount;
	}

	if (dwCount == 0)
		return 0;

	return static_cast<unsigned int>(*lpStr1) - static_cast<unsigned int>(*lpStr2);
}

int StrICompW(const wchar_t *lpStr1, const wchar_t *lpStr2)
{
	while (*lpStr1 && *lpStr2)
	{
		wchar_t ch1 = (*lpStr1 >= L'A' && *lpStr1 <= L'Z') ? *lpStr1 + 32 : *lpStr1;
		wchar_t ch2 = (*lpStr2 >= L'A' && *lpStr2 <= L'Z') ? *lpStr2 + 32 : *lpStr2;

		if (ch1 != ch2)
			return static_cast<unsigned int>(ch1) - static_cast<unsigned int>(ch2);

		++lpStr1;
		++lpStr2;
	}

	return static_cast<unsigned int>(*lpStr1) - static_cast<unsigned int>(*lpStr2);
}

struct FormatModifiers
{
	bool isShort      : 1; // %h
	bool isChar       : 1; // %hh
	bool isLong       : 1; // %l
	bool isLongLong   : 1; // %ll
	bool isIntMax     : 1; // %j
	bool isSizeT      : 1; // %z
	bool isPtrdiff    : 1; // %t
	bool isLongDouble : 1; // %L
};

using FormatHandler = int (*)(char *&pOut, size_t &remaining, va_list &args, const FormatModifiers &modifiers);

struct FormatSpecHandler
{
	char specifier;
	FormatHandler handler;
};

static int HandleDecimal(char *&pOut, size_t &remaining, va_list &args, const FormatModifiers &modifiers)
{
	char temp[21];

	if (modifiers.isLongLong) {
		long long val = va_arg(args, long long);
		int len = Int64ToStr(temp, val);
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}
	else if (modifiers.isLong) {
		long val = va_arg(args, long);
		int len = Int64ToStr(temp, static_cast<long long>(val));
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}
	else if (modifiers.isShort) {
		short val = static_cast<short>(va_arg(args, int));
		int len = IntToStr(temp, val);
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}
	else if (modifiers.isChar) {
		signed char val = static_cast<signed char>(va_arg(args, int));
		int len = IntToStr(temp, val);
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}
	else {
		int val = va_arg(args, int);
		int len = IntToStr(temp, val);
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}

	return 0;
}

static int HandleUnsigned(char *&pOut, size_t &remaining, va_list &args, const FormatModifiers &modifiers)
{
	char temp[21];

	if (modifiers.isLongLong) {
		unsigned long long val = va_arg(args, unsigned long long);
		int len = UInt64ToStr(temp, val);
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}
	else if (modifiers.isLong) {
		unsigned long val = va_arg(args, unsigned long);
		int len = UInt64ToStr(temp, static_cast<uint64_t>(val));
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}
	else if (modifiers.isShort) {
		unsigned short val = static_cast<unsigned short>(va_arg(args, unsigned int));
		int len = UIntToStr(temp, val);
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}
	else if (modifiers.isChar) {
		unsigned char val = static_cast<unsigned char>(va_arg(args, unsigned int));
		int len = UIntToStr(temp, val);
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}
	else {
		unsigned int val = va_arg(args, unsigned int);
		int len = UIntToStr(temp, val);
		for (int i = 0; i < len && remaining > 0; ++i, --remaining)
			*pOut++ = temp[i];
	}

	return 0;
}

static int HandleHexLower(char *&pOut, size_t &remaining, va_list &args, const FormatModifiers &modifiers)
{
	char temp[17];

	uint64_t value = 0;
	if (modifiers.isLongLong) {
		value = va_arg(args, unsigned long long);
	}
	else if (modifiers.isLong) {
		value = static_cast<uint64_t>(va_arg(args, unsigned long));
	}
	else if (modifiers.isShort) {
		value = static_cast<unsigned short>(va_arg(args, unsigned int));
	}
	else if (modifiers.isChar) {
		value = static_cast<unsigned char>(va_arg(args, unsigned int));
	}
	else {
		value = va_arg(args, unsigned int);
	}

	int len = UInt64ToHex(temp, value, false);
	for (int i = 0; i < len && remaining > 0; ++i, --remaining)
		*pOut++ = temp[i];

	return 0;
}

static int HandleHexUpper(char *&pOut, size_t &remaining, va_list &args, const FormatModifiers &modifiers)
{
	char temp[17];

	uint64_t value = 0;
	if (modifiers.isLongLong) {
		value = va_arg(args, unsigned long long);
	}
	else if (modifiers.isLong) {
		value = static_cast<uint64_t>(va_arg(args, unsigned long));
	}
	else if (modifiers.isShort) {
		value = static_cast<unsigned short>(va_arg(args, unsigned int));
	}
	else if (modifiers.isChar) {
		value = static_cast<unsigned char>(va_arg(args, unsigned int));
	}
	else {
		value = va_arg(args, unsigned int);
	}

	int len = UInt64ToHex(temp, value, true);
	for (int i = 0; i < len && remaining > 0; ++i, --remaining)
		*pOut++ = temp[i];

	return 0;
}

static int HandleString(char *&pOut, size_t &remaining, va_list &args, const FormatModifiers &)
{
	const char *str = va_arg(args, const char *);
	if (!str) str = "(null)";

	while (*str && remaining > 0)
		*pOut++ = *str++, --remaining;

	return 0;
}

static int HandleChar(char *&pOut, size_t &remaining, va_list &args, const FormatModifiers &)
{
	if (remaining > 0) {
		*pOut++ = static_cast<char>(va_arg(args, int));
		--remaining;
	}
	return 0;
}

static int HandlePointer(char *&pOut, size_t &remaining, va_list &args, const FormatModifiers &)
{
	uintptr_t ptr = reinterpret_cast<uintptr_t>(va_arg(args, void *));
	char temp[2 + sizeof(uintptr_t) * 2];

	int len = UInt64ToHex(temp, ptr, false);
	for (int i = 0; i < len && remaining > 0; ++i, --remaining)
		*pOut++ = temp[i];

	return 0;
}

static int HandleFloat(char *&pOut, size_t &remaining, va_list &args, const FormatModifiers &modifiers)
{
	char temp[64];
	int len = 0;

	if (modifiers.isLongDouble) {
		long double val = va_arg(args, long double);
		len = LongDoubleToStr(temp, val, 6);
	}
	else {
		double val = va_arg(args, double);
		len = DoubleToStr(temp, val, 6);
	}

	for (int i = 0; i < len && remaining > 0; ++i, --remaining)
		*pOut++ = temp[i];

	return 0;
}

static FormatSpecHandler g_formatHandlers[] =
{
	{ 'd', HandleDecimal },
	{ 'i', HandleDecimal },
	{ 'u', HandleUnsigned },
	{ 'x', HandleHexLower },
	{ 'X', HandleHexUpper },
	{ 's', HandleString },
	{ 'c', HandleChar },
	{ 'p', HandlePointer },
	{ 'f', HandleFloat },
	{ 0, nullptr }
};

static FormatHandler FindHandler(char spec)
{
	for (int i = 0; g_formatHandlers[i].specifier != 0; ++i)
	{
		if (g_formatHandlers[i].specifier == spec)
		{
			return g_formatHandlers[i].handler;
		}
	}

	return nullptr;
}

static FormatModifiers ParseFormatModifiers(const char *&format)
{
	FormatModifiers modifiers = {};

	while (true)
	{
		if (*format == 'h')
		{
			if (*(format + 1) == 'h')
			{
				modifiers.isChar = true;
				format += 2;
			}
			else
			{
				modifiers.isShort = true;
				++format;
			}
		}
		else if (*format == 'l')
		{
			if (*(format + 1) == 'l')
			{
				modifiers.isLongLong = true;
				format += 2;
			}
			else
			{
				modifiers.isLong = true;
				++format;
			}
		}
		else if (*format == 'j')
		{
			modifiers.isIntMax = true;
			++format;
		}
		else if (*format == 'z')
		{
			modifiers.isSizeT = true;
			++format;
		}
		else if (*format == 't')
		{
			modifiers.isPtrdiff = true;
			++format;
		}
		else if (*format == 'L')
		{
			modifiers.isLongDouble = true;
			++format;
		}
		else
		{
			break;
		}
	}

	return modifiers;
}

static int HandleTime(char *&pOut, size_t &remaining, va_list &, const FormatModifiers &)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	char sep[4] = ":";
	GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_STIME, sep, sizeof(sep));

	char timeFmt[2] = "1";
	GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_ITIME, timeFmt, sizeof(timeFmt));
	bool is24Hour = (timeFmt[0] == '1');

	int hour = st.wHour;
	bool isPM = false;

	if (!is24Hour)
	{
		if (hour == 0)
		{
			hour = 12;
		}
		else if (hour >= 12)
		{
			if (hour > 12)
				hour -= 12;

			isPM = true;
		}
	}

	char temp[16];
	int i = 0;

	temp[i++] = '0' + ((hour / 10) % 10);
	temp[i++] = '0' + (hour % 10);
	temp[i++] = sep[0];
	temp[i++] = '0' + ((st.wMinute / 10) % 10);
	temp[i++] = '0' + (st.wMinute % 10);
	temp[i++] = sep[0];
	temp[i++] = '0' + ((st.wSecond / 10) % 10);
	temp[i++] = '0' + (st.wSecond % 10);

	if (!is24Hour) {
		temp[i++] = ' ';
		temp[i++] = isPM ? 'P' : 'A';
		temp[i++] = 'M';
	}

	temp[i] = '\0';

	size_t len = StrLenA(temp);
	if (len > remaining)
		len = remaining;

	MemCopy(pOut, temp, len);
	pOut += len;
	remaining -= len;

	return 0;
}

static int HandleDate(char *&pOut, size_t &remaining, va_list &, const FormatModifiers &)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	char sep[4] = "/";
	GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SDATE, sep, sizeof(sep));

	char orderStr[2] = "0";
	GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_IDATE, orderStr, sizeof(orderStr));
	int order = orderStr[0] - '0'; // 0 = MDY, 1 = DMY, 2 = YMD

	char temp[16];
	int i = 0;

	char m1 = '0' + ((st.wMonth / 10) % 10);
	char m2 = '0' + (st.wMonth % 10);
	char d1 = '0' + ((st.wDay / 10) % 10);
	char d2 = '0' + (st.wDay % 10);
	char y1 = '0' + ((st.wYear / 1000) % 10);
	char y2 = '0' + ((st.wYear / 100) % 10);
	char y3 = '0' + ((st.wYear / 10) % 10);
	char y4 = '0' + (st.wYear % 10);

	if (order == 0) { // MDY
		temp[i++] = m1; temp[i++] = m2;
		temp[i++] = sep[0];
		temp[i++] = d1; temp[i++] = d2;
		temp[i++] = sep[0];
		temp[i++] = y1; temp[i++] = y2;
		temp[i++] = y3; temp[i++] = y4;
	}
	else if (order == 1) { // DMY
		temp[i++] = d1; temp[i++] = d2;
		temp[i++] = sep[0];
		temp[i++] = m1; temp[i++] = m2;
		temp[i++] = sep[0];
		temp[i++] = y1; temp[i++] = y2;
		temp[i++] = y3; temp[i++] = y4;
	}
	else { // YMD
		temp[i++] = y1; temp[i++] = y2;
		temp[i++] = y3; temp[i++] = y4;
		temp[i++] = sep[0];
		temp[i++] = m1; temp[i++] = m2;
		temp[i++] = sep[0];
		temp[i++] = d1; temp[i++] = d2;
	}

	temp[i] = '\0';

	size_t len = StrLenA(temp);
	if (len > remaining)
		len = remaining;

	MemCopy(pOut, temp, len);
	pOut += len;
	remaining -= len;

	return 0;
}

static FormatSpecHandler g_extendedFormatHandlers[] = {
	{ 't', HandleTime },
	{ 'd', HandleDate },
	{  0 , nullptr     }
};

static FormatHandler FindExtendedHandler(char spec)
{
	for (int i = 0; g_extendedFormatHandlers[i].specifier != 0; ++i)
	{
		if (g_extendedFormatHandlers[i].specifier == spec)
			return g_extendedFormatHandlers[i].handler;
	}

	return nullptr;
}

int FormatBufSafeV(char *lpBuffer, size_t dwMaxSize, const char *lpFormat, va_list args)
{
	if (lpBuffer == nullptr || lpFormat == nullptr || dwMaxSize == 0)
		return 0;

	char *pOut = lpBuffer;
	size_t remaining = (dwMaxSize == static_cast<size_t>(-1)) ? static_cast<size_t>(-1) : dwMaxSize - 1;

	while (*lpFormat && remaining > 0)
	{
		if (*lpFormat == '%')
		{
			++lpFormat;

			if (*lpFormat == '#')
			{
				++lpFormat;

				if (!*lpFormat)
					break;

				FormatHandler extHandler = FindExtendedHandler(*lpFormat);
				if (extHandler)
				{
					FormatModifiers emptyModifiers = {};
					extHandler(pOut, remaining, args, emptyModifiers);
				}
				else
				{
					if (remaining > 0)
					{
						*pOut++ = '%';
						--remaining;
					}
					if (remaining > 0)
					{
						*pOut++ = '#';
						--remaining;
					}
					if (remaining > 0)
					{
						*pOut++ = *lpFormat;
						--remaining;
					}
				}

				++lpFormat;
				continue;
			}

			FormatModifiers modifiers = ParseFormatModifiers(lpFormat);

			if (!*lpFormat)
				break;

			if (*lpFormat == '%')
			{
				*pOut++ = '%';
				--remaining;
			}
			else if (*lpFormat == 'n')
			{
				AssertMsg(false, "%%n is disabled for safety reasons");
			}
			else
			{
				FormatHandler handler = FindHandler(*lpFormat);
				if (handler)
				{
					handler(pOut, remaining, args, modifiers);
				}
				else
				{
					AssertMsg(false, "Unknown format specifier");

					if (remaining > 0)
					{
						*pOut++ = '%';
						--remaining;
					}
					if (remaining > 0)
					{
						*pOut++ = *lpFormat;
						--remaining;
					}
				}
			}

			++lpFormat;
		}
		else
		{
			*pOut++ = *lpFormat++;
			--remaining;
		}
	}

	*pOut = '\0';
	return static_cast<int>(pOut - lpBuffer);
}

int FormatBufSafe(char *lpBuffer, size_t dwMaxSize, const char *lpFormat, ...)
{
	va_list args;
	va_start(args, lpFormat);
	int result = FormatBufSafeV(lpBuffer, dwMaxSize, lpFormat, args);
	va_end(args);
	return result;
}

int IntToStr(char *lpBuffer, int nValue)
{
	char *p = lpBuffer;
	bool bNegative = false;

	if (nValue < 0)
	{
		bNegative = true;
		nValue = -nValue;
	}

	char temp[12];
	int i = 0;

	do
	{
		temp[i++] = '0' + (nValue % 10);
		nValue /= 10;
	} while (nValue > 0);

	if (bNegative)
		temp[i++] = '-';

	while (i-- > 0)
		*p++ = temp[i];

	*p = '\0';
	return static_cast<int>(p - lpBuffer);
}

int UIntToStr(char *lpBuffer, unsigned int uValue)
{
	char *p = lpBuffer;
	char temp[11];
	int i = 0;

	do
	{
		temp[i++] = '0' + (uValue % 10);
		uValue /= 10;
	} while (uValue > 0);

	while (i-- > 0)
		*p++ = temp[i];

	*p = '\0';
	return static_cast<int>(p - lpBuffer);
}

int UIntToHex(char *lpBuffer, unsigned int uValue)
{
	volatile char hexDigits[17];
	*(uint32_t *)&hexDigits[ 0] = '3210';
	*(uint32_t *)&hexDigits[ 4] = '7654';
	*(uint32_t *)&hexDigits[ 8] = 'ba98';
	*(uint32_t *)&hexDigits[12] = 'fedc';
	hexDigits[16] = '\0';

	char *p = lpBuffer;
	char temp[9];
	int i = 0;

	do
	{
		temp[i++] = hexDigits[uValue % 16];
		uValue /= 16;
	} while (uValue > 0);

	while (i-- > 0)
		*p++ = temp[i];

	*p = '\0';
	return static_cast<int>(p - lpBuffer);
}

int Int64ToStr(char *lpBuffer, long long nValue)
{
	char *p = lpBuffer;
	bool bNegative = false;

	if (nValue < 0)
	{
		bNegative = true;
		nValue = -nValue;
	}

	char temp[21];
	int i = 0;

	do {
		temp[i++] = '0' + (nValue % 10);
		nValue /= 10;
	} while (nValue > 0);

	if (bNegative)
		temp[i++] = '-';

	while (i-- > 0)
		*p++ = temp[i];

	*p = '\0';
	return static_cast<int>(p - lpBuffer);
}

int UInt64ToStr(char *lpBuffer, unsigned long long uValue)
{
	char *p = lpBuffer;
	char temp[21];
	int i = 0;

	do {
		temp[i++] = '0' + (uValue % 10);
		uValue /= 10;
	} while (uValue > 0);

	while (i-- > 0)
		*p++ = temp[i];

	*p = '\0';
	return static_cast<int>(p - lpBuffer);
}

int UInt64ToHex(char *lpBuffer, unsigned long long uValue, bool uppercase)
{
	volatile char hexDigits[17];

	if (uppercase)
	{
		*(uint32_t *)&hexDigits[0] = '3210';
		*(uint32_t *)&hexDigits[4] = '7654';
		*(uint32_t *)&hexDigits[8] = 'BA98';
		*(uint32_t *)&hexDigits[12] = 'FEDC';
	}
	else
	{
		*(uint32_t *)&hexDigits[0] = '3210';
		*(uint32_t *)&hexDigits[4] = '7654';
		*(uint32_t *)&hexDigits[8] = 'ba98';
		*(uint32_t *)&hexDigits[12] = 'fedc';
	}

	char *p = lpBuffer;
	char temp[17];
	int i = 0;

	do
	{
		unsigned digit = uValue % 16;
		uValue /= 16;

		if (digit < 10)
			temp[i++] = hexDigits[digit];
		else
			temp[i++] = hexDigits[digit];
	} while (uValue > 0);

	while (i-- > 0)
		*p++ = temp[i];

	*p = '\0';
	return static_cast<int>(p - lpBuffer);
}

int DoubleToStr(char *lpBuffer, double value, int precision)
{
	char *p = lpBuffer;

	if (IsNan(value))
	{
		*p++ = 'n'; *p++ = 'a'; *p++ = 'n';
		*p = '\0';
		return 3;
	}

	if (IsInf(value))
	{
		if (value < 0) *p++ = '-';
		*p++ = 'i'; *p++ = 'n'; *p++ = 'f';
		*p = '\0';
		return static_cast<int>(p - lpBuffer);
	}

	if (value < 0)
	{
		*p++ = '-';
		value = -value;
	}

	long long intPart = static_cast<long long>(value);
	p += Int64ToStr(p, intPart);

	*p++ = '.';

	double frac = value - intPart;

	for (int i = 0; i < precision; ++i)
	{
		frac *= 10.0;
		int digit = static_cast<int>(frac);
		*p++ = '0' + digit;
		frac -= digit;
	}

	*p = '\0';
	return static_cast<int>(p - lpBuffer);
}

int LongDoubleToStr(char *lpBuffer, long double value, int precision)
{
	char *p = lpBuffer;

	if (IsNan(value)) {
		*p++ = 'n'; *p++ = 'a'; *p++ = 'n';
		*p = '\0';
		return 3;
	}

	if (IsInf(value)) {
		if (value < 0) *p++ = '-';
		*p++ = 'i'; *p++ = 'n'; *p++ = 'f';
		*p = '\0';
		return static_cast<int>(p - lpBuffer);
	}

	if (value < 0) {
		*p++ = '-';
		value = -value;
	}

	long long intPart = static_cast<long long>(value);
	p += Int64ToStr(p, intPart);

	*p++ = '.';

	long double frac = value - intPart;

	for (int i = 0; i < precision; ++i) {
		frac *= 10.0;
		int digit = static_cast<int>(frac);
		*p++ = '0' + digit;
		frac -= digit;
	}

	*p = '\0';
	return static_cast<int>(p - lpBuffer);
}

bool IsNan(double val)
{
	union
	{
		double d;
		uint64_t u;
	} db;

	db.d = val;
	uint64_t exp = (db.u >> 52) & 0x7FF;
	uint64_t frac = db.u & 0xFFFFFFFFFFFFFull;
	return (exp == 0x7FF) && (frac != 0);
}

bool IsInf(double val)
{
	union
	{
		double d;
		uint64_t u;
	} db;

	db.d = val;
	uint64_t exp = (db.u >> 52) & 0x7FF;
	uint64_t frac = db.u & 0xFFFFFFFFFFFFFull;
	return (exp == 0x7FF) && (frac == 0);
}

bool IsNan(long double val)
{
	union
	{
		long double ld;
		struct
		{
			uint64_t mantissa;
			uint16_t exponent;
			uint16_t pad[3];
		} parts;
	} ld_union = {};

	ld_union.ld = val;

	uint16_t exp = ld_union.parts.exponent & 0x7FFF;
	uint64_t frac = ld_union.parts.mantissa & 0x7FFFFFFFFFFFFFFF;

	return (exp == 0x7FFF) && (frac != 0);
}

bool IsInf(long double val)
{
	union
	{
		long double ld;
		struct
		{
			uint64_t mantissa;
			uint16_t exponent;
			uint16_t pad[3];
		} parts;
	} ld_union = {};

	ld_union.ld = val;

	uint16_t exp = ld_union.parts.exponent & 0x7FFF;
	uint64_t frac = ld_union.parts.mantissa & 0x7FFFFFFFFFFFFFFF;

	return (exp == 0x7FFF) && (frac == 0);
}

size_t StrLenA(const char *lpString)
{
	const char *p = lpString;
	while (*p) ++p;
	return static_cast<size_t>(p - lpString);
}

size_t StrLenW(const wchar_t *lpString)
{
	const wchar_t *p = lpString;
	while (*p) ++p;
	return static_cast<size_t>(p - lpString);
}

MEMORIA_END