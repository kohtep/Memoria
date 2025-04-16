#pragma once

#define MEMORIA_BEGIN \
	namespace Memoria {

#define MEMORIA_END \
	}

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__)

#define MEMORIA_64BIT 1
#undef  MEMORIA_32BIT

#define IsX64() true
#define IsX32() false

#else

#define MEMORIA_32BIT 1
#undef MEMORIA_64BIT

#define IsX64() false
#define IsX32() true

#endif

// memoria_utils_buffer.cpp
//#define MEMORIA_DISABLE_UTILS_BUFFER

// memoria_captura_function.cpp
//#define MEMORIA_DISABLE_CAPTURA_FUNCTION

// memoria_captura_hook32.cpp
//#define MEMORIA_DISABLE_CAPTURA_HOOK32

// memoria_captura_hook64.cpp
//#define MEMORIA_DISABLE_CAPTURA_HOOK64

// memoria_captura_trampoline.cpp
//#define MEMORIA_DISABLE_CAPTURA_TRAMPOLINE

// memoria_core_check.cpp
//#define MEMORIA_DISABLE_CORE_CHECK

// memoria_core_debug.cpp
//#define MEMORIA_DISABLE_CORE_DEBUG

// memoria_core_errors.cpp
//#define MEMORIA_DISABLE_CORE_ERRORS

// memoria_core_misc.cpp
//#define MEMORIA_DISABLE_CORE_MISC

// memoria_core_options.cpp
//#define MEMORIA_DISABLE_CORE_OPTIONS

// memoria_core_read.cpp
//#define MEMORIA_DISABLE_CORE_READ

// memoria_core_rtti.cpp
//#define MEMORIA_DISABLE_CORE_RTTI

// memoria_core_search.cpp
//#define MEMORIA_DISABLE_CORE_SEARCH

// memoria_core_signature.cpp
//#define MEMORIA_DISABLE_CORE_SIGNATURE

// memoria_core_windows.cpp
//#define MEMORIA_DISABLE_CORE_WINDOWS

// memoria_core_write.cpp
//#define MEMORIA_DISABLE_CORE_WRITE

// memoria_core_mempool.cpp
//#define MEMORIA_DISABLE_CORE_MEMPOOL

// memoria_ext_hook.cpp
//#define MEMORIA_DISABLE_EXT_HOOK

// memoria_ext_module.cpp
//#define MEMORIA_DISABLE_EXT_MODULE

// memoria_ext_patch.cpp
//#define MEMORIA_DISABLE_EXT_PATCH

// memoria_ext_sig.cpp
//#define MEMORIA_DISABLE_EXT_SIG