#include "memoria_utils_format.hpp"

#include "memoria_utils_assert.hpp"

#include <stdarg.h>
#include <stdint.h>
#include <Windows.h>

#include "memoria_utils_secure.hpp"

#ifdef MEMORIA_USE_LAZYIMPORT
	#define GetLocaleInfoA     LI_FN(GetLocaleInfoA)
	#define GetLastError       LI_FN(GetLastError)
	#define GetLocalTime       LI_FN(GetLocalTime)
	#define FormatMessageA     LI_FN(FormatMessageA)
#endif

MEMORIA_BEGIN

int FormatBufSafe(char *lpBuffer, size_t dwMaxSize, const char *lpFormat, ...)
{
	va_list args;
	va_start(args, lpFormat);
	int result = vsprintf_s(lpBuffer, dwMaxSize, lpFormat, args);
	va_end(args);
	return result;
}

static int va_current = 0;
static char va_string[16][1024];

char *VFormatBufSafe(const char *lpFormat, ...)
{
	va_list argptr;
	va_current = (va_current + 1) % 16;

	va_start(argptr, lpFormat);
	FormatBufSafe(va_string[va_current], _countof(va_string[va_current]), lpFormat, argptr);
	va_end(argptr);

	return va_string[va_current];
}

MEMORIA_END