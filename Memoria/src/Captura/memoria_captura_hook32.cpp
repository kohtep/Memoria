#include "memoria_captura_hook32.hpp"

#ifndef MEMORIA_DISABLE_CAPTURA_HOOK32

#include "memoria_utils_buffer.hpp"
#include "memoria_utils_assert.hpp"

#include "hde32.h"

MEMORIA_BEGIN

size_t CalculateInstructionSize32(const void *addr, ptrdiff_t offset)
{
	hde32s hs;
	return hde32_disasm(reinterpret_cast<const void *>(reinterpret_cast<uintptr_t>(addr) + offset), &hs);
}

size_t CalculateInstructionBoundary32(const void *addr, size_t min_size)
{
	size_t size = 0;

	while (true)
	{
		size += CalculateInstructionSize32(addr, size);

		if (size >= min_size)
			return size;
	}
}

static size_t WriteJumpRel(void *addr_target, const void *addr_value)
{
	CIndependentBuffer64 buf;

	buf.WriteU8(0xE9);                          // JMP rel32
	buf.WriteRelative(addr_target, addr_value, 1); // rel32

	if (addr_value && !buf.Clone(addr_target, true))
		return 0;

	return buf.GetSize();
}

static size_t WriteCallRel(void *addr_target, const void *addr_value)
{
	CIndependentBuffer64 buf;

	buf.WriteU8(0xE8);                          // CALL rel32
	buf.WriteRelative(addr_target, addr_value, 1); // rel32

	if (addr_value && !buf.Clone(addr_target, true))
		return 0;

	return buf.GetSize();
}

static size_t WritePushRet(void *addr_target, const void *addr_value)
{
	CIndependentBuffer64 buf;

	buf.WriteU8(0x68);            // PUSH imm32
	buf.WritePointer(addr_value); // imm32
	buf.WriteU8(0xC3);            // RET

	if (addr_value && !buf.Clone(addr_target, true))
		return 0;

	return buf.GetSize();
}

static size_t WriteJumpAbs(void *addr_target, const void *addr_value)
{
	CIndependentBuffer64 buf;

	buf.WriteU8(0xB8);            // MOV EAX, imm32
	buf.WritePointer(addr_value); // imm32
	buf.WriteU16(0xE0FF);         // JMP EAX

	if (addr_value && !buf.Clone(addr_target, true))
		return 0;

	return buf.GetSize();
}

static size_t WriteJumpMem(void *addr_target, const void *addr_value)
{
	AssertMsg(false, "WriteJumpMem is not supported in x32 mode.");
	return 0;
}

bool WriteHook32(void *addr_target, const void *addr_value, eInvokeMethod method)
{
	switch (method)
	{
	
	case eInvokeMethod::JumpRel:
		return WriteJumpRel(addr_target, addr_value) != 0;

	case eInvokeMethod::CallRel:
		return WriteCallRel(addr_target, addr_value) != 0;

	case eInvokeMethod::PushRet:
		return WritePushRet(addr_target, addr_value) != 0;

	case eInvokeMethod::JumpAbs:
		return WriteJumpAbs(addr_target, addr_value) != 0;

	case eInvokeMethod::JumpMem:
		return WriteJumpMem(addr_target, addr_value) != 0;
	
	default:
		return false;
	
	}
}

size_t CalculateHookSize32(void *addr_target, eInvokeMethod method)
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

#endif