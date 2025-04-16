#pragma once

#include "memoria_common.hpp"
#include "memoria_utils_assert.hpp"

#include <stdint.h>
#include <functional>
#include <memory>

MEMORIA_BEGIN

class CVTable
{
private:
	void **_vtable = {};

public:
	CVTable() = default;
	CVTable(void *instance) : _vtable(*reinterpret_cast<void ***>(instance)) {}

	auto value() const { return _vtable; }

	template <typename Ret = void, typename... Args> Ret Invoke(size_t index, Args... args);
	template <typename Fn, typename... Args> auto InvokeFn(size_t index, Args... args);
};

template <typename Ret, typename... Args>
Ret CVTable::Invoke(size_t index, Args... args)
{
	Assert(index >= 0);

	auto method = reinterpret_cast<void *>(_vtable[index]);

	auto func = std::function<Ret(Args...)>(
		reinterpret_cast<Ret(*)(Args...)>(method)
	);

	return func(args...);
}

template <typename Fn, typename... Args>
auto CVTable::InvokeFn(size_t index, Args... args)
{
	Assert(index >= 0);

	return ((Fn)_vtable[index])(args...);
}

class CShadowVTable
{
	struct Class
	{
		void **vtable;
	};

private:
	CShadowVTable() = delete;
	CShadowVTable(const CShadowVTable &) = delete;
	CShadowVTable &operator=(const CShadowVTable &) = delete;

	void Hook(size_t index, const void *callback);

	CVTable _vmt_original = {};

	Class *_instance = {};
	std::unique_ptr<void *[]> _vmt_shadow = {};

public:
	CShadowVTable(void *instance);
	CShadowVTable(void *instance, size_t methodCount);
	~CShadowVTable();

	template <typename T> void Hook(size_t index, const T hook);

	template <typename Ret = void, typename... Args> Ret Invoke(size_t index, Args... args);
	template <typename Fn, typename... Args> auto InvokeFn(size_t index, Args... args);
};

template <typename T>
void CShadowVTable::Hook(size_t index, T hook)
{
	Assert(index >= 0);
	Assert(hook != nullptr);

	Hook(index, (void *)hook);
}

template <typename Ret, typename... Args>
Ret CShadowVTable::Invoke(size_t index, Args... args)
{
	return _vmt_original.Invoke<Ret>(index, std::forward<Args>(args)...);
}

template <typename Fn, typename... Args>
auto CShadowVTable::InvokeFn(size_t index, Args... args)
{
	return _vmt_original.InvokeFn<Fn>(index, std::forward<Args>(args)...);
}

MEMORIA_END