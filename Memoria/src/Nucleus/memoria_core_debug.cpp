#include "memoria_core_debug.hpp"
#include "memoria_core_misc.hpp"

#include <Windows.h>
#include <dbghelp.h>
#include <stacktrace>

#pragma comment(lib, "dbghelp.lib")

MEMORIA_BEGIN

static bool SymInited = false;

static int SymShutdown()
{
	if (SymInited)
		SymCleanup(GetCurrentProcess());

	return 0;
}

static void SymInit()
{
	SymInited = SymInitialize(GetCurrentProcess(), NULL, TRUE);
	onexit(SymShutdown);
}

std::string GetSymbolName(const void *address)
{
	static bool ensure_sym_init = (SymInit(), true);

	if (!SymInited)
		return "";

	DWORD64 displacement = 0;
	char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = MAX_SYM_NAME;

	if (!SymFromAddr(GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &displacement, symbol))
		return "";

	return symbol->Name ? symbol->Name : "";
}

std::string GetBeautyFunctionAddress(const void *address, bool concat_module_name, bool memory_beautify_on_error)
{
	auto pBaseAddress = GetFunctionBaseAddressFromItsCode(address);
	if (!pBaseAddress)
	{
		if (memory_beautify_on_error)
			return BeautifyPointer(address);

		return "";
	}

	auto sName = GetSymbolName(pBaseAddress);
	if (sName.empty())
	{
		if (memory_beautify_on_error)
			return BeautifyPointer(address);

		return "";
	}

	auto offset = reinterpret_cast<uintptr_t>(address) - reinterpret_cast<uintptr_t>(pBaseAddress);

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "+0x%llX", static_cast<unsigned long long>(offset));

	std::string result{};

	if (concat_module_name)
	{
		HMODULE hModule = reinterpret_cast<HMODULE>(GetBaseAddress(address));

		if (hModule != nullptr)
			result += GetModuleName(hModule) + "!";
	}

	result += sName + buffer;
	result += " [" + BeautifyPointer(address) + "]";

	return result;
}

std::vector<std::tuple<void *, void *>> GetFunctionEntries(HMODULE handle)
{
	if (!handle)
		handle = GetModuleHandleA(0);

	if (!handle)
		return {};

	ULONG size;
	PIMAGE_RUNTIME_FUNCTION_ENTRY pFunc = reinterpret_cast<PIMAGE_RUNTIME_FUNCTION_ENTRY>(
		ImageDirectoryEntryToData(handle, TRUE, IMAGE_DIRECTORY_ENTRY_EXCEPTION, &size));

	if (!pFunc)
		return {};

	DWORD count = size / sizeof(IMAGE_RUNTIME_FUNCTION_ENTRY);

	std::vector<std::tuple<void *, void *>> result{};
	result.reserve(count);

	for (size_t i = 0u; i < count; ++i)
	{
		if (!pFunc[i].BeginAddress || !pFunc[i].EndAddress)
			continue;

		void *addr_begin = PtrOffset(handle, pFunc[i].BeginAddress);
		void *addr_end = PtrOffset(handle, pFunc[i].EndAddress);

		result.emplace_back(addr_begin, addr_end);
	}

	return result;
}

void *GetFunctionBaseAddressFromItsCode(const void *address)
{
	HMODULE hModule = static_cast<HMODULE>(GetBaseAddress(address));
	if (!hModule)
		return nullptr;

	ULONG size;
	PIMAGE_RUNTIME_FUNCTION_ENTRY pFunc = reinterpret_cast<PIMAGE_RUNTIME_FUNCTION_ENTRY>(
		ImageDirectoryEntryToData(hModule, TRUE, IMAGE_DIRECTORY_ENTRY_EXCEPTION, &size));

	if (!pFunc)
		return nullptr;

	DWORD count = size / sizeof(IMAGE_RUNTIME_FUNCTION_ENTRY);

	for (size_t i = 0u; i < count; ++i)
	{
		if (!pFunc[i].BeginAddress || !pFunc[i].EndAddress)
			continue;

		uintptr_t addr_begin = reinterpret_cast<uintptr_t>(hModule) + pFunc[i].BeginAddress;
		uintptr_t addr_end = reinterpret_cast<uintptr_t>(hModule) + pFunc[i].EndAddress;

		uintptr_t addr_needed = reinterpret_cast<uintptr_t>(address);

		if (IsInBounds(addr_needed, addr_begin, addr_end))
			return reinterpret_cast<void *>(addr_begin);
	}

	return nullptr;
}

std::vector<void *> GetStackBacktrace()
{
	std::vector<void *> result{};

	auto trace = std::stacktrace::current();

	for (const auto &frame : trace)
		result.emplace_back(frame.native_handle());

	//void *callers[128];
	//int count = RtlCaptureStackBackTrace(0, _countof(callers), callers, NULL);
	//
	//for (int i = 2; i < count; i++)
	//	result.push_back(callers[i]);

	return result;
}

std::vector<std::string> GetBeautyStackBacktrace()
{
	std::vector<std::string> result{};

	auto backtrace = GetStackBacktrace();
	if (backtrace.empty())
		return result;

	result.reserve(backtrace.size());

	for (auto &&addr : backtrace)
	{
		auto sName = GetBeautyFunctionAddress(addr);
		result.emplace_back(sName);
	}

	return result;
}

MEMORIA_END