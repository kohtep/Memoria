#include "memoria_ext_patch.hpp"

#ifndef MEMORIA_DISABLE_EXT_PATCH

#include <Windows.h>
#include <string_view>

#include "memoria_core_debug.hpp"
#include "memoria_core_write.hpp"
#include "memoria_core_misc.hpp"

#include "memoria_utils_assert.hpp"
#include "memoria_utils_string.hpp"

MEMORIA_BEGIN

bool CPatch::IsActive() const
{
	if (!_dest_address)
		return false;

	return _active;
}

bool CPatch::IsValid() const
{
	if (!_dest_address)
		return false;

	bool equal;

	if (_active)
		equal = CompareMemory(_dest_address, _data_patch.data(), _data_patch.size()) == 0;
	else
		equal = CompareMemory(_dest_address, _data_origin.data(), _data_origin.size()) == 0;

	return equal;
}

void CPatch::Apply()
{
	if (!_dest_address)
		return;

	if (_data_origin.empty() || _data_patch.empty())
		return;

	if (!IsActive())
		Toggle(true);
}

void CPatch::Restore()
{
	if (!_dest_address)
		return;

	if (_data_origin.empty() || _data_patch.empty())
		return;

	if (IsActive())
		Toggle(false);
}

void CPatch::Toggle(bool state)
{
	if (!_dest_address)
		return;

	if (_data_origin.empty() || _data_patch.empty())
		return;

	if (IsMemoryValid(_dest_address))
	{
		if (state)
			WriteMemory(_dest_address, _data_patch.data(), _data_patch.size());
		else
			WriteMemory(_dest_address, _data_origin.data(), _data_origin.size());
	}

	_active = state;
}

CPatch::CPatch(void *dest_address, const void *source_address, size_t size, bool instant_deploy)
	: _dest_address(dest_address)
	, _active(false)
	, _stack_backtrace(GetStackBacktrace())
{
	Assert(dest_address && source_address && size);

	if (dest_address && source_address && size > 0)
	{
		_data_origin.resize(size);
		_data_patch.resize(size);

		CopyMemory(_data_origin.data(), dest_address, size);
		CopyMemory(_data_patch.data(), source_address, size);
	}
	else
	{
		_dest_address = {};

		_data_origin.clear();
		_data_patch.clear();
	}

	GetPatches().emplace_back(this);

	if (instant_deploy)
		Apply();
}

CPatch::~CPatch()
{
	if (IsActive())
		Toggle(false);

	_dest_address = {};

	_data_origin.clear();
	_data_patch.clear();

	_stack_backtrace.clear();

	auto &patches = GetPatches();

	for (auto it = patches.begin(); it != patches.end(); ++it)
	{
		if (*it == this)
		{
			patches.erase(it);
			break;
		}
	}
}

CPatch *PatchU8(void *addr, uint8_t value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchU16(void *addr, uint16_t value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchU24(void *addr, uint32_t value, bool instant_deploy, ptrdiff_t offset)
{
	uint8_t bytes[3] =
	{
		static_cast<uint8_t>(value & 0xFF),
		static_cast<uint8_t>((value >> 8) & 0xFF),
		static_cast<uint8_t>((value >> 16) & 0xFF)
	};

	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &bytes, 3, instant_deploy);
}

CPatch *PatchU24(void *addr, uint8_t value[3], bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, 3, instant_deploy);
}

CPatch *PatchU32(void *addr, uint32_t value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchU64(void *addr, uint64_t value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchI8(void *addr, int8_t value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchI16(void *addr, int16_t value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchI24(void *addr, int32_t value, bool instant_deploy, ptrdiff_t offset)
{
	int8_t bytes[3] =
	{
		static_cast<int8_t>(value & 0xFF),
		static_cast<int8_t>((value >> 8) & 0xFF),
		static_cast<int8_t>((value >> 16) & 0xFF)
	};

	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &bytes, 3, instant_deploy);
}

CPatch *PatchI24(void *addr, int8_t value[3], bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, 3, instant_deploy);
}

CPatch *PatchI32(void *addr, int32_t value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchI64(void *addr, int64_t value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchFloat(void *addr, float value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchDouble(void *addr, double value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchPointer(void *addr, const void *value, bool instant_deploy, ptrdiff_t offset)
{
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &value, sizeof(value), instant_deploy);
}

CPatch *PatchRelative(void *addr, const void *value, bool instant_deploy, ptrdiff_t offset)
{
	if (!IsIn32BitRange(addr, value))
		return nullptr;

#pragma warning(suppress : 4244) // conversion from '__int64' to 'uint32_t', possible loss of data
	uint32_t rel = (uint8_t *)value - ((uint8_t *)addr + sizeof(int32_t));

	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), &rel, sizeof(rel), instant_deploy);
}

CPatch *PatchAStr(void *addr, const char *value, bool instant_deploy, ptrdiff_t offset)
{
	if (!value)
		return nullptr;

	size_t len = StrLenA(value);
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), value,
		(len + 1) * sizeof(char), instant_deploy);
}

CPatch *PatchWStr(void *addr, const wchar_t *value, bool instant_deploy, ptrdiff_t offset)
{
	if (!value)
		return nullptr;

	size_t len = StrLenW(value);
	return new CPatch(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(addr) + offset), value,
		(len + 1) * sizeof(wchar_t), instant_deploy);
}

bool FreePatch(CPatch *patch)
{
	if (!patch)
		return false;

	delete patch;
	return true;
}

bool FreeAllPatches()
{
	for (auto patch : CPatch::GetPatches())
	{
		delete patch;
	}

	Assert(CPatch::GetPatches().empty());
	return true;
}

MEMORIA_END

#endif