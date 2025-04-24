#pragma once

#include "memoria_common.hpp"
#include "memoria_captura_common.hpp"

#include <stdint.h>

MEMORIA_BEGIN

extern size_t CalculateInstructionSize32(const void *addr, ptrdiff_t offset = 0);
extern size_t CalculateInstructionBoundary32(const void *addr, size_t min_size);

extern bool WriteHook32(void *addr_target, const void *addr_value, eInvokeMethod method);
extern size_t CalculateHookSize32(void *addr_target, eInvokeMethod method);

MEMORIA_END