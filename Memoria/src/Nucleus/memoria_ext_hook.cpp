#include "memoria_ext_hook.hpp"

#ifndef MEMORIA_DISABLE_EXT_HOOK

#include "memoria_utils_string.hpp"
#include "memoria_core_write.hpp"

MEMORIA_BEGIN

CShadowVTable::CShadowVTable(void *instance)
{
	Assert(instance != nullptr);

	auto _instance = (void ***)instance;
	void **_vmt_org = *_instance;

	size_t methodCount = 0;
	while (_vmt_org[methodCount])
		methodCount++;

	new (this) CShadowVTable(instance, methodCount);
}

CShadowVTable::CShadowVTable(void *instance, size_t methodCount)
{
	Assert(instance != nullptr);
	Assert(methodCount > 0);

	this->_instance = reinterpret_cast<Class *>(instance);
	void **_vmt_org = this->_instance->vtable;

	_vmt_shadow = std::make_unique<void *[]>(methodCount);

	CopyMemory(_vmt_shadow.get(), _vmt_org, methodCount * sizeof(void *));

	new (&_vmt_original) CVTable(instance);

	this->_instance->vtable = _vmt_shadow.get();
}

CShadowVTable::~CShadowVTable()
{
	_instance->vtable = _vmt_original.value();
}

void CShadowVTable::Hook(size_t index, const void *callback)
{
	Memoria::WritePointer(&(_vmt_shadow.get()[index]), callback);
}

MEMORIA_END

#endif