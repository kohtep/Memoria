#pragma once

#include "memoria_common.hpp"
#include "memoria_captura_common.hpp"

#include <stdint.h>

MEMORIA_BEGIN

extern size_t CalculateInstructionSize64(const void *addr, ptrdiff_t offset = 0);
extern size_t CalculateInstructionBoundary64(const void *addr, size_t min_size);

extern bool WriteHook64(void *addr_target, const void *addr_value, eInvokeMethod method);
extern size_t CalculateHookSize64(void *addr_target, eInvokeMethod method);

MEMORIA_END