#include "memoria_captura_function.hpp"

#ifndef MEMORIA_DISABLE_CAPTURA_FUNCTION

MEMORIA_BEGIN

bool CFunctionImpl::IsActive() const
{
	return false;
}

bool CFunctionImpl::Hook()
{
	return false;
}

bool CFunctionImpl::Unhook()
{
	return false;
}

MEMORIA_END

#endif