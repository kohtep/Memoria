#pragma once

#include "memoria_common.hpp"
#include "memoria_captura_common.hpp"

#include <cstdint>
#include <type_traits>

MEMORIA_BEGIN

#pragma pack(push, 1)
class CTrampolineBase
{
	friend class CHookMgr;

	CTrampolineBase(const CTrampolineBase &) = delete;
	CTrampolineBase &operator=(const CTrampolineBase &) = delete;

protected:
	// Pointer to hook owner.
	CHookMgr *_manager;

	// Pointer to the original code.
	void *_pointer;

	// Pointer to the hook.
	const void *_hook;

	// Original code size.
	std::uint8_t _size;

	bool _x64;

	eInvokeMethod _method;

public:
	CTrampolineBase() = delete;

	CTrampolineBase(CHookMgr *manager, void *target, const void *hook, bool is_x64, std::uint8_t size, eInvokeMethod method);
};

class CTrampoline : public CTrampolineBase
{
	CTrampoline(const CTrampoline &) = delete;
	CTrampoline &operator=(const CTrampoline &) = delete;

private:
	// Original code backup.
	//
	// This field must be guaranteed to be safe for execution, meaning that the object of this class
	// should be placed inside the CHookMgr class and is not intended for external use.
	std::uint8_t _backup[64 - sizeof(CTrampolineBase)];

public:
	CTrampoline() = delete;
	CTrampoline(CHookMgr *manager, void *target, const void *hook, bool is_x64, std::uint8_t size, eInvokeMethod method);
	~CTrampoline();

	bool IsActive();

	bool Hook();
	bool Unhook();

	void *GetJmpHook() { return reinterpret_cast<void *>(&_backup[0]); }
	void *GetOriginal() { return reinterpret_cast<void *>(&_backup[5]); }

	__forceinline void operator()()
	{
		using fn_ptr_t = void(*)();
		auto fn = reinterpret_cast<fn_ptr_t>(GetOriginal());
		return fn();
	}
};

static_assert(sizeof(CTrampoline) == 64);

template <typename ret_t = void, typename... args_t>
class CTrampolineEx : public CTrampoline
{
public:
	CTrampolineEx(CHookMgr *manager, void *code, std::uint8_t size, eInvokeMethod method)
		: CTrampoline(manager, code, size, method)
	{
	}

	__forceinline ret_t operator()(args_t... args)
	{
		using fn_ptr_t = ret_t(*)(args_t...);
		auto fn = reinterpret_cast<fn_ptr_t>(GetOriginal());
		return fn(std::forward<args_t>(args)...);
	}
};

#pragma pack(pop)

class CHookMgr
{
private:
	CHookMgr(const CHookMgr &) = delete;
	CHookMgr &operator=(const CHookMgr &) = delete;

private:
	void *_data = nullptr;

	size_t _hooks = 0;
	size_t _max_hooks = 0;

public:
	CHookMgr() = default;
	CHookMgr(const void *addr_nearest, size_t max_hooks = 64);
	~CHookMgr();

	CTrampoline *Allocate(void *target, const void *hook, bool is_x64, eInvokeMethod method);

	bool IsNear(const void *addr) const;
};

extern bool Hook(void *target, const void *hook, void *trampoline = nullptr);

extern bool Hook32(void *target, const void *hook, void *trampoline, eInvokeMethod method = eInvokeMethod::JumpRel);
extern bool Hook64(void *target, const void *hook, void *trampoline, eInvokeMethod method = eInvokeMethod::JumpRel);

MEMORIA_END