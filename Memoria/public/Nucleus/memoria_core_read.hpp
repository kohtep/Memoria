#pragma once

#include "memoria_common.hpp"

#include "memoria_core_signature.hpp"

#include <stdint.h>
#include <string_view>
#include <string>

#include <optional>
#include <span>

MEMORIA_BEGIN

extern std::optional<uint8_t> ReadU8(const void *addr, ptrdiff_t offset = 0);
extern std::optional<uint16_t> ReadU16(const void *addr, ptrdiff_t offset = 0);
extern std::optional<uint32_t> ReadU24(const void *addr, ptrdiff_t offset = 0);
extern std::optional<uint32_t> ReadU32(const void *addr, ptrdiff_t offset = 0);
extern std::optional<uint64_t> ReadU64(const void *addr, ptrdiff_t offset = 0);

extern std::optional<int8_t> ReadI8(const void *addr, ptrdiff_t offset = 0);
extern std::optional<int16_t> ReadI16(const void *addr, ptrdiff_t offset = 0);
extern std::optional<int32_t> ReadI24(const void *addr, ptrdiff_t offset = 0);
extern std::optional<int32_t> ReadI32(const void *addr, ptrdiff_t offset = 0);
extern std::optional<int64_t> ReadI64(const void *addr, ptrdiff_t offset = 0);

extern std::optional<float> ReadFloat(const void *addr, ptrdiff_t offset = 0);
extern std::optional<double> ReadDouble(const void *addr, ptrdiff_t offset = 0);

extern std::string ReadAStr(const void *addr, ptrdiff_t offset = 0);
extern std::wstring ReadWStr(const void *addr, ptrdiff_t offset = 0);

extern bool GetMemoryBlock(const void *source, size_t size, void *dest);
extern std::vector<uint8_t> GetMemoryData(const void *source, size_t size);

extern std::span<const uint8_t> GetMemorySpan(const void *source, size_t size);
extern std::span<uint8_t> GetMemorySpan(void *source, size_t size);

MEMORIA_END