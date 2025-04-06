#include "memoria_captura_trampoline.hpp"

#include "memoria_core_misc.hpp"
#include "memoria_core_write.hpp"

#include "memoria_captura_hook32.hpp"
#include "memoria_captura_hook64.hpp"

#include "memoria_utils_buffer.hpp"

#include <list>

MEMORIA_BEGIN

CTrampolineBase::CTrampolineBase(CHookMgr *manager, void *target, const void *hook, bool is_x64, std::uint8_t size, eInvokeMethod method)
	: _manager(manager)
	, _pointer(target)
	, _hook(hook)
	, _size(size)
	, _method(method)
	, _x64(is_x64)
{
	
}

CTrampoline::CTrampoline(CHookMgr *manager, void *target, const void *hook, bool is_x64, std::uint8_t size, eInvokeMethod method)
	: CTrampolineBase(manager, target, hook, is_x64, size, method)
{
#ifdef _DEBUG
	std::memset(_backup, 0x90, sizeof(_backup));
#endif

	CWriteBuffer buf(GetJmpHook(), 5);
	buf.WriteU8(0xE9);
	buf.WriteRelative(_hook, GetJmpHook());

	std::memcpy(GetOriginal(), target, size);

	auto _backup_new = PtrOffset(GetOriginal(), size);
	auto _target_new = PtrOffset(target, size);

	// funny ternary stuff
	((_x64) ? (WriteHook64) : (WriteHook32))(_backup_new, _target_new, eInvokeMethod::JumpRel);
}

CTrampoline::~CTrampoline()
{

}

bool CTrampoline::IsActive()
{
	return std::memcmp(_pointer, const_cast<const void *>(GetOriginal()), _size) != 0;
}

bool CTrampoline::Hook()
{
	if (IsActive())
		return false;

	// funny ternary stuff again
	return ((_x64) ? (WriteHook64) : (WriteHook32))(_pointer, _hook, _method);
}

bool CTrampoline::Unhook()
{
	if (!IsActive())
		return false;

	return WriteMemory(_pointer, _backup, _size);
}

CHookMgr::~CHookMgr()
{
	Free(_data);
}

CHookMgr::CHookMgr(const void *addr_nearest, size_t max_hooks)
{
	_max_hooks = max_hooks;
	_hooks = 0;

	_data = AllocFar(addr_nearest, _max_hooks * sizeof(CTrampoline), true, true, true);
}

CTrampoline *CHookMgr::Allocate(void *target, const void *hook, bool is_x64, eInvokeMethod method)
{
	if (_hooks >= _max_hooks)
		return nullptr;

	if (_data == nullptr)
		return nullptr;

	uintptr_t base = reinterpret_cast<uintptr_t>(_data);
	size_t offset = sizeof(CTrampoline) * _hooks;

	CTrampoline *result = reinterpret_cast<CTrampoline *>(base + offset);
	++_hooks;

	size_t size;

	if (is_x64)
	{
		size = CalculateHookSize64(target, method);
		size = CalculateInstructionBoundary64(target, size);
	}
	else
	{
		size = CalculateHookSize32(target, method);
		size = CalculateInstructionBoundary32(target, size);
	}

	new (result) CTrampoline(this, target, hook, is_x64, static_cast<uint8_t>(size), method);

	return result;
}

bool CHookMgr::IsNear(const void *addr) const
{
	return IsIn32BitRange(_data, addr);
}

static std::list<CHookMgr> gTrampolineMgrs;

static CHookMgr *FindNearestTrampolineMgr(const void *addr)
{
	for (auto &mgr : gTrampolineMgrs)
	{
		if (mgr.IsNear(addr))
			return &mgr;
	}

	gTrampolineMgrs.emplace_back(addr);
	return &gTrampolineMgrs.back();
}

static bool HookInternal(void *target, const void *hook, void *trampoline, bool is_x64, eInvokeMethod method)
{
	auto mgr = FindNearestTrampolineMgr(target);
	if (!mgr)
		return false;

	CTrampoline *tmp = mgr->Allocate(target, hook, is_x64, method);

	if (!tmp->Hook())
		return false;

	if (trampoline)
		*reinterpret_cast<void **>(trampoline) = tmp->GetOriginal();

	return true;
}

bool Hook(void *target, const void *hook, void *trampoline)
{
	return HookInternal(target, hook, trampoline, IsX64(), eInvokeMethod::JumpRel);
}

bool Hook32(void *target, const void *hook, void *trampoline, eInvokeMethod method)
{
	return HookInternal(target, hook, trampoline, false, method);
}

bool Hook64(void *target, const void *hook, void *trampoline, eInvokeMethod method)
{
	return HookInternal(target, hook, trampoline, true, method);
}

MEMORIA_END