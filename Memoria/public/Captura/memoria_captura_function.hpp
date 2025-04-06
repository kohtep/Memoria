#pragma once

#include "memoria_common.hpp"
#include "memoria_captura_common.hpp"

#include <stdint.h>
#include <type_traits>

MEMORIA_BEGIN

class CFunctionImpl
{
protected:
	eInvokeMethod _method = eInvokeMethod::JumpRel;
	void *_function = {};
	void *_hook = {};
	void *_trampoline = {};

public:
	CFunctionImpl() = default;

	CFunctionImpl(void *function, void *hook)
		: _function(function)
		, _hook(hook)
		, _trampoline{}
		, _method(eInvokeMethod::JumpRel)
	{

	}

	void SetHook(void *value) { _hook = value; }
	void SetTarget(void *value) { _function = value; }

	bool IsActive() const;

	bool Hook();
	bool Unhook();
};

template <typename ret_t, typename... args_t>
class CFunction : public CFunctionImpl
{
public:
	using T = ret_t(args_t...);

public:
	CFunction() = default;

	CFunction(void *function, void *hook) : CFunctionImpl(function, hook) {}

	inline auto GetFunction() const { return reinterpret_cast<T *>(_function); }
	inline auto GetHook() const { return reinterpret_cast<T *>(_hook); }
	inline auto GetTrampoline() const { return reinterpret_cast<T *>(_trampoline); }

	ret_t operator()(args_t... args) const
	{
		if (auto f = GetTrampoline())
			return f(std::forward<args_t>(args)...);
		else
			return ret_t();
	}
};

MEMORIA_END