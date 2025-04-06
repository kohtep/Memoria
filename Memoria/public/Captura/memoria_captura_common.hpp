#pragma once

#include "memoria_common.hpp"

#include <cstdint>

MEMORIA_BEGIN

/*

	TODO: VEH HOOK

	Interception using a VEH (Vectored Exception Handler).

	The hook must support:

	1. Generation of ACCESS VIOLATION exceptions
	2. Possibly generation of other types of exceptions (e.g., division by zero)
	3. Software breakpoint

*/

enum class eInvokeMethod : std::uint8_t
{
	// jmp rel32
	JumpRel,

	// call rel32
	CallRel,

	// push imm32/imm64
	// ret
	PushRet,

	// mov eax/rax, imm32/imm64
	// jmp eax/rax
	JumpAbs,

	// jmp [rip+0] ; FF 25 00 00 00 00
	// DQ imm64
	//
	// This method is only for x64!
	JumpMem
};

MEMORIA_END