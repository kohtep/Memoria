#include "memoria_ext_sig.hpp"

#include "memoria_core_search.hpp"
#include "memoria_core_check.hpp"
#include "memoria_core_read.hpp"
#include "memoria_core_misc.hpp"
#include "memoria_ext_sig.hpp"
#include "memoria_ext_module.hpp"

#include <assert.h>

MEMORIA_BEGIN

CSigHandle::CSigHandle(const void *mem_begin, const void *mem_end, void *output)
	: _internal_data{}
{
	assert(mem_begin && mem_end);

	_mem_begin = mem_begin;
	_mem_end = mem_end;

	if (output != nullptr)
	{
		_output = (void **)output;
	}
	else
	{
		_output = &_internal_data;
	}

	*_output = const_cast<void *>(_mem_begin);

#ifdef _DEBUG
	_output_debug = *_output;
#endif
}

CSigHandle::CSigHandle(CMemoryBlock *block, void *output)
	: CSigHandle(block->GetBase(), block->GetLastByte(), output)
{

}

CSigHandle::~CSigHandle()
{

}

void CSigHandle::SetOutputInternally(const void *value, bool deref)
{
	if (!_output)
		return;

	__try
	{
		if (deref)
			*_output = *reinterpret_cast<void **>(const_cast<void *>(value));
		else
			*_output = reinterpret_cast<void **>(const_cast<void *>(value));
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		assert(false);
		*_output = {};
	}

#ifdef _DEBUG
	_output_debug = *_output;
#endif
}

std::optional<uint8_t> CSigHandle::ReadU8(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadU8(*_output, offset);
}

std::optional<uint16_t> CSigHandle::ReadU16(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadU16(*_output, offset);
}


std::optional<uint32_t> CSigHandle::ReadU24(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadU24(*_output, offset);
}

std::optional<uint32_t> CSigHandle::ReadU32(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadU32(*_output, offset);
}

std::optional<uint64_t> CSigHandle::ReadU64(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadU64(*_output, offset);
}

std::optional<int8_t> CSigHandle::ReadI8(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadI8(*_output, offset);
}

std::optional<int16_t> CSigHandle::ReadI16(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadI16(*_output, offset);
}

std::optional<int32_t> CSigHandle::ReadI24(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadI24(*_output, offset);
}

std::optional<int32_t> CSigHandle::ReadI32(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadI32(*_output, offset);
}

std::optional<int64_t> CSigHandle::ReadI64(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadI64(*_output, offset);
}

std::optional<float> CSigHandle::ReadFloat(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadFloat(*_output, offset);
}

std::optional<double> CSigHandle::ReadDouble(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return std::nullopt;

	return Memoria::ReadDouble(*_output, offset);
}

std::string CSigHandle::ReadAStr(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return {};

	return Memoria::ReadAStr(*_output, offset);
}

std::wstring CSigHandle::ReadWStr(ptrdiff_t offset)
{
	if (*_output == nullptr)
		return {};

	return Memoria::ReadWStr(*_output, offset);
}

CSigHandle &CSigHandle::FindU8(uint8_t value, bool backward, ptrdiff_t offset)
{
    if (*_output == nullptr)
        return *this;

    auto result = Memoria::FindU8(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
    return *this;
}

CSigHandle &CSigHandle::FindU16(uint16_t value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindU16(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindU24(uint8_t value[3], bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindU24(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindU24(uint32_t value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindU24(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindU32(uint32_t value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindU32(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindU64(uint64_t value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindU64(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindI8(int8_t value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindI8(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindI16(int16_t value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindI16(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindI24(int8_t value[3], bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindI24(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindI24(int32_t value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindI24(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindI32(int32_t value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindI32(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindI64(int64_t value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindI64(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindFloat(float value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindFloat(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindDouble(double value, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindDouble(*_output, _mem_begin, _mem_end, value, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindBlock(const void *data, size_t size, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindBlock(*_output, _mem_begin, _mem_end, data, size, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindSignature(const CSignature &sig, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindSignature(*_output, _mem_begin, _mem_end, sig, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindSignature(const std::string_view &sig, bool backward, ptrdiff_t offset)
{
	CSignature s(sig);
	return FindSignature(s, backward, offset);
}

CSigHandle &CSigHandle::FindReference(const void *data, uint16_t opcode, bool search_absolute, bool search_relative, bool backward, ptrdiff_t pre_offset, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindReference(*_output, _mem_begin, _mem_end, data, opcode, search_absolute, search_relative, 
		backward, offset, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindAStr(const char *data, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindAStr(*_output, _mem_begin, _mem_end, data, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindWStr(const wchar_t *data, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindWStr(*_output, _mem_begin, _mem_end, data, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::FindRelative(uint16_t opcode, size_t index, bool backward, ptrdiff_t offset)
{
	if (*_output == nullptr)
		return *this;

	auto result = Memoria::FindRelative(*_output, _mem_begin, _mem_end, opcode, index, backward, offset);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::Deref()
{
	if (*_output != nullptr)
		SetOutputInternally(*_output, true);

	return *this;
}

CSigHandle &CSigHandle::Rip()
{
	if (*_output != nullptr)
	{
		void *result = Memoria::RelToAbsEx(*_output, 0, sizeof(uint32_t));
		SetOutputInternally(result, false);
	}

	return *this;
}

CSigHandle &CSigHandle::Rip(ptrdiff_t offset)
{
	if (*_output != nullptr)
	{
		void *result = Memoria::RelToAbs(*_output, offset);
		SetOutputInternally(result, false);
	}

	return *this;
}

CSigHandle &CSigHandle::Rip(ptrdiff_t pre_offset, ptrdiff_t post_offset)
{
	if (*_output != nullptr)
	{
		PtrOffset(pre_offset);
		void *result = Memoria::RelToAbs(*_output, post_offset);
		SetOutputInternally(result, false);
	}

	return *this;
}

CSigHandle &CSigHandle::PtrOffset(ptrdiff_t value)
{
	if (*_output == nullptr)
		return *this;

	void *result;

	result = Memoria::PtrOffset(*_output, value);

	SetOutputInternally(result, false);
	return *this;
}

CSigHandle &CSigHandle::Add(size_t value)
{
	return PtrOffset(static_cast<signed>(value));
}

CSigHandle &CSigHandle::Sub(size_t value)
{
	return PtrOffset(-static_cast<signed>(value));
}

CSigHandle &CSigHandle::Align(size_t value)
{
	if (*_output != nullptr)
		SetOutputInternally((void *)((intptr_t)(*_output) & ~(value - 1)), false);

	return *this;
}

ptrdiff_t CSigHandle::GetOffset() const
{
	if (_output == nullptr || _mem_begin == nullptr)
		return 0;

	if (*_output == nullptr)
		return 0;

	return (ptrdiff_t)(uintptr_t(*_output) - (uintptr_t)_mem_begin);
}

void CSigHandle::Reset()
{
	SetOutputInternally(_mem_begin, false);
}

void CSigHandle::Invalidate()
{
	if (_output)
		*_output = nullptr;
}

void CSigHandle::ForceOutput(const void *value)
{
	if (value == nullptr || !Memoria::IsInBounds(value, _mem_begin, _mem_end))
	{
		*_output = nullptr;
		return;
	}

	SetOutputInternally(value, false);
}

void *CSigHandle::CurrentOutput() const
{
	if (!_output)
		return nullptr;

	return *_output;
}

bool CSigHandle::IsValid() const
{
	if (_output == nullptr || _mem_begin == nullptr)
		return false;

	if (*_output == nullptr)
		return false;

	if (!Memoria::IsInBounds(*_output, _mem_begin, _mem_end))
		return false;

	return true;
}

bool CSigHandle::CheckU8(uint8_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckU8(*_output, value, offset);
}

bool CSigHandle::CheckU16(uint16_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckU16(*_output, value, offset);
}

bool CSigHandle::CheckU24(uint32_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckU24(*_output, value, offset);
}

bool CSigHandle::CheckU24(uint8_t value[3], ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckU24(*_output, value, offset);
}

bool CSigHandle::CheckU32(uint32_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckU32(*_output, value, offset);
}

bool CSigHandle::CheckU64(uint64_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckU64(*_output, value, offset);
}

bool CSigHandle::CheckI8(int8_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckI8(*_output, value, offset);
}

bool CSigHandle::CheckI16(int16_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckI16(*_output, value, offset);
}

bool CSigHandle::CheckI24(int32_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckI24(*_output, value, offset);
}

bool CSigHandle::CheckI24(int8_t value[3], ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckI24(*_output, value, offset);
}

bool CSigHandle::CheckI32(int32_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckI32(*_output, value, offset);
}

bool CSigHandle::CheckI64(int64_t value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckI64(*_output, value, offset);
}

bool CSigHandle::CheckFloat(float value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckFloat(*_output, value, offset);
}

bool CSigHandle::CheckDouble(double value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckDouble(*_output, value, offset);
}

bool CSigHandle::CheckAStr(const std::string_view &value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckAStr(*_output, value, offset);
}

bool CSigHandle::CheckWStr(const std::wstring_view &value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckWStr(*_output, value, offset);
}

bool CSigHandle::CheckSignature(const CSignature &value, ptrdiff_t offset) const
{
	if (*_output == nullptr)
		return false;

	return Memoria::CheckSignature(*_output, value, offset);
}

bool CSigHandle::CheckSignature(const std::string_view &value, ptrdiff_t offset) const
{
	CSignature sig(value);
	return CheckSignature(sig, offset);
}

MEMORIA_END