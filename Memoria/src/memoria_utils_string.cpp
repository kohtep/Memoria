#include "memoria_utils_string.hpp"

#include <stdarg.h>
#include <stdint.h>

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

int CompareMemory(const void *lpBlock1, const void *lpBlock2, size_t dwSize)
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

void *CopyMemory(void *lpDestination, const void *lpSource, size_t dwSize)
{
	unsigned char *dest = static_cast<unsigned char *>(lpDestination);
	const unsigned char *src = static_cast<const unsigned char *>(lpSource);

	for (size_t i = 0; i < dwSize; ++i)
	{
		dest[i] = src[i];
	}

	return lpDestination;
}

void *MoveMemory(void *lpDestination, const void *lpSource, size_t dwSize)
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

void *FillMemory(void *lpDestination, int nValue, size_t dwSize)
{
	unsigned char *dest = static_cast<unsigned char *>(lpDestination);

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

int FormatBufSafeV(char *lpBuffer, size_t dwMaxSize, const char *lpFormat, va_list args)
{
	if (lpBuffer == nullptr || lpFormat == nullptr || dwMaxSize == 0)
		return 0;

	char *pOut = lpBuffer;
	size_t remaining = (dwMaxSize == (size_t)-1) ? (size_t)-1 : dwMaxSize - 1;

	while (*lpFormat && remaining > 0)
	{
		if (*lpFormat == '%')
		{
			++lpFormat;
			bool isLongLong = false;

			if (*lpFormat == 'l' && *(lpFormat + 1) == 'l')
			{
				isLongLong = true;
				lpFormat += 2;
			}

			if (!*lpFormat)
				break;

			switch (*lpFormat)
			{
			case 'd':
				if (isLongLong)
				{
					long long val = va_arg(args, long long);
					char temp[21];
					int len = Int64ToStr(temp, val);
					for (int i = 0; i < len && remaining > 0; ++i, --remaining)
						*pOut++ = temp[i];
				}
				else
				{
					int val = va_arg(args, int);
					char temp[12];
					int len = IntToStr(temp, val);
					for (int i = 0; i < len && remaining > 0; ++i, --remaining)
						*pOut++ = temp[i];
				}
				break;

			case 'u':
				if (isLongLong)
				{
					unsigned long long val = va_arg(args, unsigned long long);
					char temp[21];
					int len = UInt64ToStr(temp, val);
					for (int i = 0; i < len && remaining > 0; ++i, --remaining)
						*pOut++ = temp[i];
				}
				else
				{
					unsigned int val = va_arg(args, unsigned int);
					char temp[11];
					int len = UIntToStr(temp, val);
					for (int i = 0; i < len && remaining > 0; ++i, --remaining)
						*pOut++ = temp[i];
				}
				break;

			case 'x':
				if (isLongLong)
				{
					unsigned long long val = va_arg(args, unsigned long long);
					char temp[17];
					int len = UInt64ToHex(temp, val, false);
					for (int i = 0; i < len && remaining > 0; ++i, --remaining)
						*pOut++ = temp[i];
				}
				else
				{
					unsigned int val = va_arg(args, unsigned int);
					char temp[9];
					int len = UInt64ToHex(temp, val, false);
					for (int i = 0; i < len && remaining > 0; ++i, --remaining)
						*pOut++ = temp[i];
				}
				break;

			case 'X':
				if (isLongLong)
				{
					unsigned long long val = va_arg(args, unsigned long long);
					char temp[17];
					int len = UInt64ToHex(temp, val, true);
					for (int i = 0; i < len && remaining > 0; ++i, --remaining)
						*pOut++ = temp[i];
				}
				else
				{
					unsigned int val = va_arg(args, unsigned int);
					char temp[9];
					int len = UInt64ToHex(temp, val, true);
					for (int i = 0; i < len && remaining > 0; ++i, --remaining)
						*pOut++ = temp[i];
				}
				break;

			case 's':
			{
				const char *str = va_arg(args, const char *);
				if (!str) str = "(null)";
				while (*str && remaining > 0)
				{
					*pOut++ = *str++;
					--remaining;
				}
				break;
			}
			case 'c':
			{
				char ch = static_cast<char>(va_arg(args, int));
				if (remaining > 0)
				{
					*pOut++ = ch;
					--remaining;
				}
				break;
			}
			case 'p':
			{
				uintptr_t ptr = reinterpret_cast<uintptr_t>(va_arg(args, void *));
				char temp[2 + sizeof(uintptr_t) * 2];
				int len = UInt64ToHex(temp, ptr, false);
				for (int i = 0; i < len && remaining > 0; ++i, --remaining)
					*pOut++ = temp[i];
				break;
			}
			case 'f':
			{
				double val = va_arg(args, double);
				char temp[64];
				int len = DoubleToStr(temp, val, 6); // 6 знаков после запятой
				for (int i = 0; i < len && remaining > 0; ++i, --remaining)
					*pOut++ = temp[i];
				break;
			}

			default:
				if (remaining > 0) { *pOut++ = '%'; --remaining; }
				if (remaining > 0) { *pOut++ = *lpFormat; --remaining; }
				break;
			}
		}
		else
		{
			*pOut++ = *lpFormat;
			--remaining;
		}

		++lpFormat;
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