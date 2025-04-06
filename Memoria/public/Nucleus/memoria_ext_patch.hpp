#pragma once

#include "memoria_common.hpp"

#include <stdint.h>
#include <string_view>
#include <vector>
#include <memory>

MEMORIA_BEGIN

class CPatch
{
private:
	CPatch(const CPatch &) = delete;
	CPatch &operator=(const CPatch &) = delete;

	// The '_patches' variable is intentionally declared outside of 'GetPatches',
	// because all static variables by default behave as TLS (Thread-Local Storage)
	// to ensure thread safety. I did not want each thread to have its own instance
	// of the patch container, so I moved the variable out and made it 'static inline'.
	//
	// In general, this issue could be resolved using '/Zc:threadSafeInit-' (for MSVC),
	// but I would prefer not to make such a global change just for a single container.
	static inline std::vector<CPatch *> *_patches;

	static std::vector<CPatch *> &GetPatches()
	{
		if (!_patches)
		{
			_patches = new std::vector<CPatch *>();
			std::atexit([]() { delete _patches; });
		}

		return *_patches;
	}

	friend extern bool FreeAllPatches();

private:
	void *_dest_address;

	std::vector<uint8_t> _data_origin;
	std::vector<uint8_t> _data_patch;

	std::vector<void *> _stack_backtrace;
	bool _active;

	void Toggle(bool state);

public:
	bool IsActive() const;
	bool IsValid() const;

	void Apply();
	void Restore();

	void *GetAddress() const { return _dest_address; }

	const auto &GetDataOrigin() const { return _data_origin; }
	const auto &GetDataPatch() const { return _data_patch; }

	CPatch(void *dest_address, const void *source_address, size_t size, bool instant_deploy);
	~CPatch();
};

extern CPatch *PatchU8(void *addr, uint8_t value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchU16(void *addr, uint16_t value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchU24(void *addr, uint32_t value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchU24(void *addr, uint8_t value[3], bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchU32(void *addr, uint32_t value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchU64(void *addr, uint64_t value, bool instant_deploy = true, ptrdiff_t offset = 0);

extern CPatch *PatchI8(void *addr, int8_t value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchI16(void *addr, int16_t value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchI24(void *addr, int32_t value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchI24(void *addr, int8_t value[3], bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchI32(void *addr, int32_t value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchI64(void *addr, int64_t value, bool instant_deploy = true, ptrdiff_t offset = 0);

extern CPatch *PatchFloat(void *addr, float value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchDouble(void *addr, double value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchPointer(void *addr, const void *value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchRelative(void *addr, const void *value, bool instant_deploy = true, ptrdiff_t offset = 0);

extern CPatch *PatchAStr(void *addr, const std::string_view &value, bool instant_deploy = true, ptrdiff_t offset = 0);
extern CPatch *PatchWStr(void *addr, const std::wstring_view &value, bool instant_deploy = true, ptrdiff_t offset = 0);

extern bool FreePatch(CPatch *patch);
extern bool FreeAllPatches();

MEMORIA_END