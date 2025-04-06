#pragma once

// TODO: Rename? memoria_core_misc -> memoria_core_utils

#include "memoria_common.hpp"
#include "memoria_core_hash.hpp"

#include <stdint.h>
#include <Windows.h>
#include <string>
#include <functional>

MEMORIA_BEGIN

/**
 * @brief Converts the relative offset, at which it is located,
 *        to an absolute address relative to `addr`.
 *
 * @param addr The address from which the offset will be read.
 * @param offset The offset relative to `addr` that will be applied before reading.
 *
 * @return The absolute address relative to `addr` where the offset is located.
 */
extern void *RelToAbs(const void *addr, ptrdiff_t offset = 0);

/**
 * @brief Converts the relative offset, at which it is located,
 *        to an absolute address relative to `addr`.
 *
 * @param addr The address from which the offset will be read.
 * @param pre_offset The offset relative to `addr` that will be applied before reading.
 * @param post_offset The offset relative to `addr` that will be applied after reading.
 *
 * @return The absolute address relative to `addr` where the offset is located.
 */
extern void *RelToAbsEx(const void *addr, ptrdiff_t pre_offset, ptrdiff_t post_offset);

/**
 * @brief Forms an offset relative to `base` for the address `addr`.
 *
 * @param base The reference point.
 * @param addr The address for which the offset will be calculated.
 * @param imm_size An additional offset to the result that will be added after
 *                 the calculation. By default, it is 4 bytes. This is needed because
 *                 some assembly instructions require, for example, an offset of 5 instead of 4.
 *
 * @see CWriteBuffer::WriteRelative
 * @return The offset relative to `base` for the address `addr`, or `0` if offset cannot be calculated.
 */
extern int32_t CalcRel(const void *base, void *addr, size_t imm_size = sizeof(int32_t));

/**
 * @brief Performs an offset relative to `addr` forward or backward by the value of `offset`.
 *
 * @param addr The starting address.
 * @param offset The value for the offset; can be positive (forward offset) or
 *               negative (backward offset).
 * @param dereference If `true`, the function will dereference the specified address
 *                    (after applying the offset) and return the result.
 *
 * @return The new address value after the offset or the dereferenced result if `dereference` is true.
 */
extern void *PtrOffset(const void *addr, ptrdiff_t offset, bool dereference = false);

/**
 * @brief Performs an offset relative to `addr` forward by the value of `offset`.
 *
 * @param addr The starting address.
 * @param offset The value for the forward offset.
 * @param dereference If `true`, the function will dereference the specified address
 *                    (after applying the offset) and return the result.
 *
 * @return The new address value after the offset or the dereferenced result if `dereference` is true.
 */
extern void *PtrAdvance(const void *addr, size_t offset, bool dereference = false);

/**
 * @brief Performs an offset relative to `addr` backward by the value of `offset`.
 *
 * @param addr The starting address.
 * @param offset The value for the backward offset.
 * @param dereference If `true`, the function will dereference the specified address
 *                    (after applying the offset) and return the result.
 *
 * @return The new address value after the offset or the dereferenced result if `dereference` is true.
 */
extern void *PtrRewind(const void *addr, size_t offset, bool dereference = false);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool IsInBounds(uintptr_t addr, uintptr_t addr_lower, uintptr_t addr_upper);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool IsInBounds(const void *addr, const void *addr_lower, const void *addr_upper);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool IsIn32BitRange(uintptr_t addr1, uintptr_t addr2, ptrdiff_t offset = 0);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool IsIn32BitRange(const void *addr1, const void *addr2, ptrdiff_t offset = 0);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool IsMemoryValid(const void *addr, ptrdiff_t offset = 0);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool IsMemoryExecutable(const void *addr, ptrdiff_t offset = 0);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool MakeWritable(void *addr);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool MakeReadable(void *addr);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool MakeExecutable(void *addr);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool RemoveWritable(void *addr);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool RemoveReadable(void *addr);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool RemoveExecutable(void *addr);

/**
 * @brief Returns the base address of the pointer `addr`.
 *
 * @param addr Pointer to an arbitrary memory location.
 *
 * @return Base address.
 */
extern void *GetBaseAddress(const void *addr);

/**
 * @brief Retrieves the address (ImageBase) of the module (DLL/EXE) that called the function.
 *
 * @return The ImageBase of the caller.
 */
extern __declspec(noinline) void *GetSelfAddress();

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern std::string GetModuleName(HMODULE hmod);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern std::string GetModuleNameForAddress(const void *address);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern std::string BeautifyPointer(const void *address);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern bool Free(void *addr);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern size_t Align(size_t value, int alignment = 16);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern void *Align(const void *value, int alignment = 16);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern void *Alloc(size_t size, bool is_executable = false, bool is_readable = true, bool is_writable = true);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern void *AllocNear(const void *addr_source, size_t size, bool is_executable = false, bool is_readable = true, bool is_writable = true);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern void *AllocFar(const void *addr_source, size_t size, bool is_executable = false, bool is_readable = true, bool is_writable = true);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern DWORD BeginThread(std::function<void()> &&fnFunction);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern DWORD BeginThread(void (*fnFunction)(LPVOID), LPVOID param);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern HMODULE GetModuleHandleDirect(fnv1a_t module_name_hash);

/**
 * @brief
 *
 * @param
 *
 * @return
 */
extern void *GetProcAddressDirect(fnv1a_t module_name_hash, fnv1a_t function_name_hash);

/**
 * @brief Retrieves a pointer to the `module_name` interface from the library specified by `handle`.
 *        Used for Valve Software-style modules that expose a `CreateInterface` function.
 *
 * @param handle Handle to the module.
 * @param module_name Name of the interface, e.g., `IFileSystemV009`.
 *
 * @return Pointer to the interface.
 */
extern void *GetInterfaceAddress(HMODULE handle, std::string_view module_name);

/**
 * @brief Retrieves a pointer to the `interface_name` interface from the library specified by `module_name`.
 *        Used for Valve Software-style modules that expose a `CreateInterface` function.
 *
 * @param module_name Name of the module.
 * @param interface_name Name of the interface, e.g., `IFileSystemV009`.
 *
 * @return Pointer to the interface.
 */
extern void *GetInterfaceAddress(std::string_view module_name, std::string_view interface_name);

MEMORIA_END