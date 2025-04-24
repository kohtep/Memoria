#include "memoria_core_misc.hpp"

#ifndef MEMORIA_DISABLE_CORE_MISC

#include "memoria_utils_string.hpp"

#include <Windows.h>
#include <inttypes.h>

#undef max

MEMORIA_BEGIN

void *RelToAbsEx(const void *addr, ptrdiff_t pre_offset, ptrdiff_t post_offset)
{
	return RelToAbs(reinterpret_cast<void *>(ptrdiff_t(addr) + pre_offset), post_offset);
}

void *RelToAbs(const void *addr, ptrdiff_t offset)
{
	return PtrOffset(addr, *static_cast<const int32_t *>(addr) + offset);
}

int32_t CalcRel(const void *base, void *addr, size_t imm_size)
{
	return reinterpret_cast<int32_t>(PtrOffset(base, -reinterpret_cast<intptr_t>(addr) - imm_size));
}

void *PtrOffset(const void *addr, ptrdiff_t offset, bool dereference)
{
	void *result = reinterpret_cast<void *>(reinterpret_cast<intptr_t>(addr) + offset);

	if (dereference)
		result = *reinterpret_cast<void **>(result);

	return result;
}

void *PtrAdvance(const void *addr, size_t offset, bool dereference)
{
	void *result = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset);

	if (dereference)
		result = *reinterpret_cast<void **>(result);

	return result;
}

void *PtrRewind(const void *addr, size_t offset, bool dereference)
{
	void *result = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) - offset);

	if (dereference)
		result = *reinterpret_cast<void **>(result);

	return result;
}

bool IsInBounds(uintptr_t addr, uintptr_t addr_lower, uintptr_t addr_upper)
{
	if (addr < addr_lower)
		return false;

	if (addr >= addr_upper)
		return false;

	return true;
}

bool IsInBounds(const void *addr, const void *addr_lower, const void *addr_upper)
{
	auto naddr = reinterpret_cast<uintptr_t>(addr);
	auto lower = reinterpret_cast<uintptr_t>(addr_lower);
	auto upper = reinterpret_cast<uintptr_t>(addr_upper);

	return IsInBounds(naddr, lower, upper);
}

bool IsIn32BitRange(uintptr_t addr1, uintptr_t addr2, ptrdiff_t offset)
{
	uintptr_t adjusted_addr2 = static_cast<uintptr_t>(static_cast<ptrdiff_t>(addr2) + offset);
	uintptr_t diff = (addr1 > adjusted_addr2) ? (addr1 - adjusted_addr2) : (adjusted_addr2 - addr1);

	return diff <= std::numeric_limits<uint32_t>::max();
}

bool IsIn32BitRange(const void *addr1, const void *addr2, ptrdiff_t offset)
{
	uintptr_t ptr1 = reinterpret_cast<uintptr_t>(addr1);
	uintptr_t ptr2 = reinterpret_cast<uintptr_t>(addr2);
	uintptr_t adjusted_ptr2 = static_cast<uintptr_t>(static_cast<ptrdiff_t>(ptr2) + offset);
	uintptr_t diff = (ptr1 > adjusted_ptr2) ? (ptr1 - adjusted_ptr2) : (adjusted_ptr2 - ptr1);

	return diff <= std::numeric_limits<uint32_t>::max();
}

bool IsMemoryValid(const void *addr, ptrdiff_t offset)
{
	if (offset != 0)
		addr = (void *)(uintptr_t(addr) + offset);

	if (!addr)
		return false;

	MEMORY_BASIC_INFORMATION memInfo;

	if (VirtualQuery(addr, &memInfo, sizeof(memInfo)) == 0)
		return false;

	return !(memInfo.Protect == 0 || memInfo.Protect == PAGE_NOACCESS);
}

bool IsMemoryExecutable(const void *addr, ptrdiff_t offset)
{
	if (offset != 0)
		addr = (void *)(uintptr_t(addr) + offset);

	if (!addr)
		return false;

	MEMORY_BASIC_INFORMATION memInfo;

	if (VirtualQuery(addr, &memInfo, sizeof(memInfo)) == 0)
		return false;

	if (memInfo.Protect == 0 || memInfo.Protect == PAGE_NOACCESS)
		return false;

	return memInfo.Protect == PAGE_EXECUTE ||
		memInfo.Protect == PAGE_EXECUTE_READ ||
		memInfo.Protect == PAGE_EXECUTE_READWRITE ||
		memInfo.Protect == PAGE_EXECUTE_WRITECOPY;
}

bool MakeWritable(void *addr)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
		return false;

	DWORD protect = mbi.Protect;
	DWORD newProtect = protect;

	switch (protect)
	{
	case PAGE_EXECUTE:
		newProtect = PAGE_EXECUTE_READWRITE;
		break;
	case PAGE_EXECUTE_READ:
		newProtect = PAGE_EXECUTE_READWRITE;
		break;
	case PAGE_READONLY:
		newProtect = PAGE_READWRITE;
		break;
	case PAGE_NOACCESS:
		newProtect = PAGE_READWRITE;
		break;
	}

	if (newProtect == protect)
		return false;

	DWORD oldProtect;
	return VirtualProtect(addr, mbi.RegionSize, newProtect, &oldProtect) != 0;
}

bool MakeReadable(void *addr)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
		return false;

	DWORD protect = mbi.Protect;
	DWORD newProtect = protect;

	switch (protect)
	{
	case PAGE_EXECUTE:
		newProtect = PAGE_EXECUTE_READ;
		break;
	case PAGE_NOACCESS:
		newProtect = PAGE_READONLY;
		break;
	}

	if (newProtect == protect)
		return false;

	DWORD oldProtect;
	return VirtualProtect(addr, mbi.RegionSize, newProtect, &oldProtect) != 0;
}

bool MakeExecutable(void *addr)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
		return false;

	DWORD protect = mbi.Protect;
	DWORD newProtect = protect;

	switch (protect)
	{
	case PAGE_READONLY:
		newProtect = PAGE_EXECUTE_READ;
		break;
	case PAGE_READWRITE:
		newProtect = PAGE_EXECUTE_READWRITE;
		break;
	case PAGE_NOACCESS:
		newProtect = PAGE_EXECUTE;
		break;
	}

	if (newProtect == protect)
		return false;

	DWORD oldProtect;
	return VirtualProtect(addr, mbi.RegionSize, newProtect, &oldProtect) != 0;
}

bool RemoveWritable(void *addr)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
		return false;

	DWORD protect = mbi.Protect;
	DWORD newProtect = protect;

	switch (protect) {
	case PAGE_READWRITE:
		newProtect = PAGE_READONLY;
		break;
	case PAGE_EXECUTE_READWRITE:
		newProtect = PAGE_EXECUTE_READ;
		break;
	}

	if (newProtect == protect)
		return false;

	DWORD oldProtect;
	return VirtualProtect(addr, mbi.RegionSize, newProtect, &oldProtect) != 0;
}

bool RemoveReadable(void *addr)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
		return false;

	DWORD protect = mbi.Protect;
	DWORD newProtect = protect;

	switch (protect)
	{
	case PAGE_READONLY:
		newProtect = PAGE_NOACCESS;
		break;
	case PAGE_READWRITE:
		newProtect = PAGE_NOACCESS;
		break;
	case PAGE_EXECUTE_READ:
		newProtect = PAGE_EXECUTE;
		break;
	case PAGE_EXECUTE_READWRITE:
		newProtect = PAGE_EXECUTE;
		break;
	}

	if (newProtect == protect)
		return false;

	DWORD oldProtect;
	return VirtualProtect(addr, mbi.RegionSize, newProtect, &oldProtect) != 0;
}

bool RemoveExecutable(void *addr)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
		return false;

	DWORD protect = mbi.Protect;
	DWORD newProtect = protect;

	switch (protect)
	{
	case PAGE_EXECUTE:
		newProtect = PAGE_NOACCESS;
		break;
	case PAGE_EXECUTE_READ:
		newProtect = PAGE_READONLY;
		break;
	case PAGE_EXECUTE_READWRITE:
		newProtect = PAGE_READWRITE;
		break;
	}

	if (newProtect == protect)
		return false;

	DWORD oldProtect;
	return VirtualProtect(addr, mbi.RegionSize, newProtect, &oldProtect) != 0;
}

void *GetBaseAddress(const void *addr)
{
	HMODULE result;

	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCSTR>(addr), &result))
	{
		return nullptr;
	}

	return result;
}

bool GetModuleName(HMODULE hModule, char *out, size_t max_size)
{
	if (!out || max_size == 0)
		return false;

	char buffer[MAX_PATH];
	if (GetModuleFileNameA(hModule, buffer, MAX_PATH) == 0)
	{
		out[0] = '\0';
		return false;
	}

	const char *filename = FindLastCharA(buffer, '\\');
	filename = filename ? filename + 1 : buffer;

	StrNCopyA(out, filename, max_size);
	return true;
}

bool GetModuleNameForAddress(const void *address, char *out, size_t max_size)
{
	if (!out || max_size == 0)
		return false;

	HMODULE base = (HMODULE)GetBaseAddress(address);
	if (!base)
	{
		out[0] = '\0';
		return false;
	}

	return GetModuleName(base, out, max_size);
}

bool BeautifyPointer(const void *addr, char *out, size_t max_size)
{
	if (!out || max_size == 0)
		return false;

	if (!addr)
	{
		StrNCopyA(out, "null", max_size);
		return true;
	}

	void *base = GetBaseAddress(addr);
	if (!base)
	{
		FormatBufSafe(out, max_size, "%p", addr);
		return true;
	}

	char modname[64];
	if (!GetModuleName((HMODULE)base, modname, sizeof(modname)))
	{
		FormatBufSafe(out, max_size, "%p", addr);
		return true;
	}

	char *last_dot = FindLastCharA(modname, '.');
	if (last_dot)
		*last_dot = '\0';

	uintptr_t offset = (uintptr_t)addr - (uintptr_t)base;
	FormatBuf(out, "%s.%llx", modname, (unsigned long long)offset);
	return true;
}

size_t Align(size_t value, int alignment)
{
	//Assert(alignment != 0);

	if (alignment == 0)
		return value;

	size_t mask = alignment - 1;

	if (value > SIZE_MAX - mask)
		return 0;

	return (value + mask) & ~mask;
}

void *Align(const void *value, int alignment)
{
	if (alignment == 0)
		return const_cast<void *>(value);

	uintptr_t addr = reinterpret_cast<uintptr_t>(value);
	uintptr_t mask = static_cast<uintptr_t>(alignment - 1);

	if (addr > SIZE_MAX - mask)
		return nullptr;

	addr = (addr + mask) & ~mask;
	return reinterpret_cast<void *>(addr);
}

void *GetSelfAddress()
{
	return GetBaseAddress(_ReturnAddress());
}

DWORD BeginThread(void (*fnFunction)(LPVOID), LPVOID param)
{
	DWORD nThreadId;

	HANDLE hThread = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(fnFunction), param, 0, &nThreadId);
	if (hThread == NULL)
		return 0;

	CloseHandle(hThread);
	return nThreadId;	
}

DWORD BeginThread(void (*fnFunction)())
{
	DWORD nThreadId;

	HANDLE hThread = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(fnFunction), nullptr, 0, &nThreadId);
	if (hThread == NULL)
		return 0;

	CloseHandle(hThread);
	return nThreadId;
}

typedef struct _PEB_LDR_DATA
{
	UINT8 _PADDING_[12];
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB
{
#ifdef _WIN64
	UINT8 _PADDING_[24];
#else
	UINT8 _PADDING_[12];
#endif
	PEB_LDR_DATA *Ldr;
} PEB, *PPEB;

struct UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
};

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

HMODULE GetModuleHandleDirect(fnv1a_t module_name_hash)
{
#ifdef _WIN64
	PPEB pPeb = (PPEB)__readgsqword(0x60);
#elif _WIN32
	PPEB pPeb = (PPEB)__readfsdword(0x30);
#else
	Assert(false);
	return 0;
#endif

	PLIST_ENTRY pListHead = &(pPeb->Ldr->InMemoryOrderModuleList);
	PLIST_ENTRY pListEntry = pListHead->Flink;

	while (pListEntry != pListHead)
	{
		PLDR_DATA_TABLE_ENTRY pModule = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (FNV1a(pModule->BaseDllName.Buffer) == module_name_hash)
		{
			return (HMODULE)pModule->DllBase;
		}

		pListEntry = pListEntry->Flink;
	}

	return NULL;
}

void *GetProcAddressDirect(fnv1a_t module_name_hash, fnv1a_t function_name_hash)
{
	HMODULE hModule = GetModuleHandleDirect(module_name_hash);
	if (!hModule)
		return NULL;

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((PBYTE)hModule + dosHeader->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY exportDir = (PIMAGE_EXPORT_DIRECTORY)((PBYTE)hModule + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	DWORD exportSize = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

	if (!exportDir || exportSize == 0)
		return NULL;

	PDWORD functions = (PDWORD)((PBYTE)hModule + exportDir->AddressOfFunctions);
	PDWORD names = (PDWORD)((PBYTE)hModule + exportDir->AddressOfNames);
	PWORD ordinals = (PWORD)((PBYTE)hModule + exportDir->AddressOfNameOrdinals);

	for (DWORD i = 0; i < exportDir->NumberOfNames; i++)
	{
		char *function_name = (char *)(reinterpret_cast<uintptr_t>(hModule) + names[i]);

		if (FNV1a(function_name) == function_name_hash)
			return (void *)((PBYTE)hModule + functions[ordinals[i]]);
	}

	return NULL;
}

using CreateInterfaceFn_t = void *(*)(const char *name, int *returnCode);

void *GetInterfaceAddress(HMODULE handle, const char *interface_name)
{
	if (!handle)
		handle = GetModuleHandleA(nullptr);

	if (!handle || !interface_name || !*interface_name)
		return nullptr;

	volatile char createInterfaceName[17]{};

	// Crea -> aerC
	// teIn -> nIet
	// terf -> fret
	// ace0 -> eca
	*(uint32_t *)&createInterfaceName[0] = 'aerC';
	*(uint32_t *)&createInterfaceName[4] = 'nIet';
	*(uint32_t *)&createInterfaceName[8] = 'fret';
	*(uint32_t *)&createInterfaceName[12] = 'eca';

	auto pfnGetInterface = reinterpret_cast<CreateInterfaceFn_t>(
		GetProcAddress(handle, const_cast<LPCSTR>(createInterfaceName))
		);

	if (!pfnGetInterface)
		return nullptr;

	return pfnGetInterface(interface_name, nullptr);
}

void *GetInterfaceAddress(const char *module_name, const char *interface_name)
{
	if (!module_name || !*module_name || !interface_name || !*interface_name)
		return nullptr;

	HMODULE handle = GetModuleHandleA(module_name);
	if (!handle)
		return nullptr;

	return GetInterfaceAddress(handle, interface_name);
}

MEMORIA_END

#endif