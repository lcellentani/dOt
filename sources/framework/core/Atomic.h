#ifndef __DOT_ATOMIC_H_HEADER_GUARD__
#define __DOT_ATOMIC_H_HEADER_GUARD__

#include "core/PlatformMacros.h"
#include "core/PlatformConfig.h"

#if DOT_COMPILER_MSVC
#	include <math.h>
#	include <intrin.h>
#	include <windows.h>
#endif

namespace dot
{
namespace core
{

inline void* AtomicExchangePtr(void** _ptr, void* _new)
{
#if DOT_COMPILER_MSVC
	return InterlockedExchangePointer(_ptr, _new);
#else
	return __sync_lock_test_and_set(_ptr, _new);
#endif // BX_COMPILER
}

} // namespace core
} // namespace dot

#endif // __DOT_ATOMIC_H_HEADER_GUARD__