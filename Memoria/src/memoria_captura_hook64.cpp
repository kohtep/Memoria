#include "memoria_captura_hook64.hpp"

#include "memoria_captura_trampoline.hpp"

#include "memoria_utils_buffer.hpp"
#include "memoria_core_write.hpp"
#include "memoria_core_misc.hpp"

#include "hde64.h"

#include <list>

MEMORIA_BEGIN

size_t CalculateInstructionSize64(const void *addr, ptrdiff_t offset)
{
	hde64s hs;
	return hde64_disasm(reinterpret_cast<const void *>(reinterpret_cast<uintptr_t>(addr) + offset), &hs);
}

size_t CalculateInstructionBoundary64(const void *addr, size_t min_size)
{
	size_t size = 0;

	while (true)
	{
		size += CalculateInstructionSize64(addr, size);

		if (size >= min_size)
			return size;
	}
}

static size_t WriteJumpRel(void *addr_target, const void *addr_value)
{
	if (addr_value && !IsIn32BitRange(addr_target, addr_value))
		return false;

	CIndependentBuffer64 buf;

	buf.WriteU8(0xE9);                          // JMP
	buf.WriteRelative(addr_target, addr_value, 1); // rel32

	if (addr_value && !buf.Clone(addr_target, true))
		return 0;

	return buf.GetSize();
}

static size_t WriteCallRel(void *addr_target, const void *addr_value)
{
	if (addr_value && !IsIn32BitRange(addr_target, addr_value))
		return false;

	CIndependentBuffer64 buf;

	buf.WriteU8(0xE8);                          // CALL
	buf.WriteRelative(addr_target, addr_value, 1); // rel32

	if (addr_value && !buf.Clone(addr_target, true))
		return 0;

	return buf.GetSize();
}

static size_t WritePushRet(void *addr_target, const void *addr_value)
{
	CIndependentBuffer64 buf;

	buf.WriteU8(0x48);            // REX.W
	buf.WriteU8(0xB8);            // MOV RAX, IMM64
	buf.WritePointer(addr_value); // IMM64
	buf.WriteU8(0x50);            // PUSH RAX
	buf.WriteU8(0xC3);            // RET

	if (addr_value && !buf.Clone(addr_target, true))
		return 0;

	return buf.GetSize();
}

static size_t WriteJumpAbs(void *addr_target, const void *addr_value)
{
	CIndependentBuffer64 buf;

	buf.WriteU8(0x48);            // REX.W
	buf.WriteU8(0xB8);            // MOV RAX, IMM64
	buf.WritePointer(addr_value); // IMM64
	buf.WriteU16(0xE0FF);         // JMP RAX

	if (addr_value && !buf.Clone(addr_target, true))
		return 0;

	return buf.GetSize();
}

static size_t WriteJumpMem(void *addr_target, const void *addr_value)
{
	CIndependentBuffer64 buf;

	buf.WriteU16(0x25FF);         // JMP
	buf.WriteU32(0);              // 0
	buf.WritePointer(addr_value); // DQ IMM64

	if (addr_value && !buf.Clone(addr_target, true))
		return 0;

	return buf.GetSize();
}

bool WriteHook64(void *addr_target, const void *addr_value, eInvokeMethod method)
{
	switch (method)
	{

	case eInvokeMethod::JumpRel:
		return WriteJumpRel(addr_target, addr_value);

	case eInvokeMethod::CallRel:
		return WriteCallRel(addr_target, addr_value);

	case eInvokeMethod::PushRet:
		return WritePushRet(addr_target, addr_value);

	case eInvokeMethod::JumpAbs:
		return WriteJumpAbs(addr_target, addr_value);
		
	case eInvokeMethod::JumpMem:
		return WriteJumpMem(addr_target, addr_value);

	default:
		return false;

	}
}

size_t CalculateHookSize64(void *addr_target, eInvokeMethod method)
{
	switch (method)
	{

	case eInvokeMethod::JumpRel:
		return WriteJumpRel(addr_target, nullptr);

	case eInvokeMethod::CallRel:
		return WriteCallRel(addr_target, nullptr);

	case eInvokeMethod::PushRet:
		return WritePushRet(addr_target, nullptr);

	case eInvokeMethod::JumpAbs:
		return WriteJumpAbs(addr_target, nullptr);

	case eInvokeMethod::JumpMem:
		return WriteJumpMem(addr_target, nullptr);

	default:
		return 0;

	}
}

MEMORIA_END