#pragma once

#include "memoria_common.hpp"
#include "memoria_core_signature.hpp"

#include <stdint.h>
#include <string>
#include <vcruntime.h>

MEMORIA_BEGIN

class CMemoryBlock;

class CSigHandle
{
private:
	void **_output;

	const void *_mem_begin;
	const void *_mem_end;

#ifdef _DEBUG
	void *_output_debug;
#endif

	// when constructor 'output' is nullptr then '_internal_data' will be used as output
	void *_internal_data;

private:
	void SetOutputInternally(const void *value, bool deref);

public:
	CSigHandle() = delete;
	CSigHandle(CMemoryBlock *block, void *output = nullptr);
	CSigHandle(const void *mem_begin, const void *mem_end, void *output = nullptr);
	~CSigHandle();

	//
	// Readers
	//

	std::optional<uint8_t> ReadU8(ptrdiff_t offset = 0);
	std::optional<uint16_t> ReadU16(ptrdiff_t offset = 0);
	std::optional<uint32_t> ReadU24(ptrdiff_t offset = 0);
	std::optional<uint32_t> ReadU32(ptrdiff_t offset = 0);
	std::optional<uint64_t> ReadU64(ptrdiff_t offset = 0);

	std::optional<int8_t> ReadI8(ptrdiff_t offset = 0);
	std::optional<int16_t> ReadI16(ptrdiff_t offset = 0);
	std::optional<int32_t> ReadI24(ptrdiff_t offset = 0);
	std::optional<int32_t> ReadI32(ptrdiff_t offset = 0);
	std::optional<int64_t> ReadI64(ptrdiff_t offset = 0);

	std::optional<float> ReadFloat(ptrdiff_t offset = 0);
	std::optional<double> ReadDouble(ptrdiff_t offset = 0);

	std::string ReadAStr(ptrdiff_t offset = 0);
	std::wstring ReadWStr(ptrdiff_t offset = 0);

	//
	// Search
	//

	CSigHandle &FindU8(uint8_t value, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindU16(uint16_t value, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindU24(uint8_t value[3], bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindU24(uint32_t value, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindU32(uint32_t value, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindU64(uint64_t value, bool backward = false, ptrdiff_t offset = 0);

	CSigHandle &FindI8(int8_t value, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindI16(int16_t value, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindI24(int8_t value[3], bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindI24(int32_t value, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindI32(int32_t value, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindI64(int64_t value, bool backward = false, ptrdiff_t offset = 0);

	CSigHandle &FindFloat(float value, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindDouble(double value, bool backward = false, ptrdiff_t offset = 0);

	CSigHandle &FindBlock(const void *data, size_t size, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindSignature(const CSignature &sig, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindSignature(const std::string_view &sig, bool backward = false, ptrdiff_t offset = 0);

	CSigHandle &FindReference(const void *data, uint16_t opcode, bool search_absolute, bool search_relative,
		bool backward = false, ptrdiff_t pre_offset = sizeof(int32_t), ptrdiff_t offset = 0);

	CSigHandle &FindAStr(const char *data, bool backward = false, ptrdiff_t offset = 0);
	CSigHandle &FindWStr(const wchar_t *data, bool backward = false, ptrdiff_t offset = 0);

	CSigHandle &FindRelative(uint16_t opcode = 0, size_t index = 0, bool backward = false, ptrdiff_t offset = 0);

	CSigHandle &Deref();

	CSigHandle &Rip();
	CSigHandle &Rip(ptrdiff_t offset);
	CSigHandle &Rip(ptrdiff_t pre_offset, ptrdiff_t post_offset);

	CSigHandle &PtrOffset(ptrdiff_t value);
	CSigHandle &Add(size_t value);
	CSigHandle &Sub(size_t value);

	CSigHandle &Align(size_t nValue = 0x10);

	//
	// Check
	//

	bool CheckU8(uint8_t value, ptrdiff_t offset = 0) const;
	bool CheckU16(uint16_t value, ptrdiff_t offset = 0) const;
	bool CheckU24(uint32_t value, ptrdiff_t offset = 0) const;
	bool CheckU24(uint8_t value[3], ptrdiff_t offset = 0) const;
	bool CheckU32(uint32_t value, ptrdiff_t offset = 0) const;
	bool CheckU64(uint64_t value, ptrdiff_t offset = 0) const;

	bool CheckI8(int8_t value, ptrdiff_t offset = 0) const;
	bool CheckI16(int16_t value, ptrdiff_t offset = 0) const;
	bool CheckI24(int32_t value, ptrdiff_t offset = 0) const;
	bool CheckI24(int8_t value[3], ptrdiff_t offset = 0) const;
	bool CheckI32(int32_t value, ptrdiff_t offset = 0) const;
	bool CheckI64(int64_t value, ptrdiff_t offset = 0) const;

	bool CheckFloat(float value, ptrdiff_t offset = 0) const;
	bool CheckDouble(double value, ptrdiff_t offset = 0) const;

	bool CheckAStr(const std::string_view &value, ptrdiff_t offset = 0) const;
	bool CheckWStr(const std::wstring_view &value, ptrdiff_t offset = 0) const;

	bool CheckSignature(const CSignature &value, ptrdiff_t offset = 0) const;
	bool CheckSignature(const std::string_view &value, ptrdiff_t offset = 0) const;

	//
	// Utility
	//

	CSigHandle Clone() const { return *this; }

	ptrdiff_t GetOffset() const;

	void Reset();
	void Invalidate();
	void ForceOutput(const void *value);
	void *CurrentOutput() const;

	bool IsValid() const;

	void *GetPointer() const { return (_output ? *_output : nullptr); }

	template <typename T> T As() { return reinterpret_cast<T>(GetPointer()); }
};

MEMORIA_END