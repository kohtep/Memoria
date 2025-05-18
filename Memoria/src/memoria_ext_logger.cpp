#include "memoria_ext_logger.hpp"

#include "memoria_utils_string.hpp"
#include "memoria_utils_format.hpp"
#include "memoria_utils_vector.hpp"
#include "memoria_utils_optional.hpp"
#include "memoria_utils_assert.hpp"

#include <stdarg.h>
#include <Windows.h>

#include "memoria_utils_secure.hpp"

#ifdef MEMORIA_USE_LAZYIMPORT
	#define AllocConsole       LI_FN(AllocConsole)
	#define FreeConsole        LI_FN(FreeConsole)
	#define GetConsoleWindow   LI_FN(GetConsoleWindow)
	#define GetStdHandle       LI_FN(GetStdHandle)
	#define WriteConsoleA      LI_FN(WriteConsoleA)
#endif

MEMORIA_BEGIN

static Memoria::FixedVector<LoggerCallback_t, 32> LoggerFns;

static bool IsConsoleAvailable()
{
	return GetConsoleWindow() != NULL;
}

static Memoria::Optional<bool> gbIsConsoleWasCreated;
static HANDLE ghConsoleHandle = INVALID_HANDLE_VALUE;

static void LogToConsole(const char *text)
{
	if (AssertIf(ghConsoleHandle != INVALID_HANDLE_VALUE))
	{
		DWORD written = 0;
		WriteConsoleA(ghConsoleHandle, text, (DWORD)StrLenA(text), &written, nullptr);

		char newline = '\n';
		WriteConsoleA(ghConsoleHandle, &newline, (DWORD)sizeof(newline), &written, nullptr);
	}
}

bool RegisterLogger(LoggerCallback_t cb)
{
	if (LoggerFns.full())
		return false;

	LoggerFns.emplace_back(cb);
	return true;
}

bool UnregisterLogger(LoggerCallback_t cb)
{
	if (auto it = LoggerFns.find(cb); it != LoggerFns.end())
	{
		LoggerFns.erase(it);
		return true;
	}

	return false;
}

size_t GetLoggerCount()
{
	return LoggerFns.size();
}

bool InitializeConsoleLogging()
{
	if (gbIsConsoleWasCreated.has_value())
	{
		AssertMsg(false, "The console has already been initialized. Terminate the previous session.");
		return false;
	}

	if (!IsConsoleAvailable())
	{
		if (!AllocConsole())
		{
			AssertMsg(false, "Failed to initialize the console. Error code '%d'.", GetLastError());
			return false;
		}

		gbIsConsoleWasCreated = true;
	}
	else
	{
		gbIsConsoleWasCreated = false;
	}

	ghConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	RegisterLogger(LogToConsole);

	return true;
}

bool FinalizeConsoleLogging()
{
	if (!gbIsConsoleWasCreated.has_value())
	{
		AssertMsg(false, "The console has not yet been initialized for logging.");
		return false;
	}

	if (gbIsConsoleWasCreated.value())
	{
		if (!FreeConsole())
		{
			AssertMsg(false, "Failed to free the console. Error code '%d'.", GetLastError());
			return false;
		}
	}

	gbIsConsoleWasCreated.reset();
	ghConsoleHandle = INVALID_HANDLE_VALUE;

	UnregisterLogger(LogToConsole);
	return true;
}

void DispatchLog(const char *fmt, ...)
{
	Assert(fmt);

	if (LoggerFns.empty())
		return;

	if (!fmt || !*fmt)
		return;

	char msgBuf[2048];
	va_list args;
	va_start(args, fmt);
	Memoria::FormatBufSafeV(msgBuf, sizeof(msgBuf), fmt, args);
	va_end(args);

	for (auto fn : LoggerFns)
		fn(msgBuf);
}

MEMORIA_END