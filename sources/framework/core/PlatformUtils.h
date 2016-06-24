#ifndef __DOT_PLATFORMUTILS_H_HEADER_GUARD__
#define __DOT_PLATFORMUTILS_H_HEADER_GUARD__

#include "core/PlatformConfig.h"

#include <stdint.h> // uint32_t
#if DOT_PLATFORM_WINDOWS
#include <windows.h>
#endif

#if DOT_PLATFORM_WINDOWS
#	define DOT_DYNLIB_EXT "dll"
#elif DOT_PLATFORM_OSX
#	define DOT_DYNLIB_EXT "dylib"
#else
#	define DOT_DYNLIB_EXT "so"
#endif

namespace dot
{
namespace core
{

inline void sleep(uint32_t millis)
{
#if DOT_PLATFORM_WINDOWS
	::Sleep(millis);
#else
#endif
}

inline void* dlopen(const char * filepath)
{
#if DOT_PLATFORM_WINDOWS
	return (void *) ::LoadLibrary(filepath);
#else
#endif
}

inline void dlclose(void * handle)
{
#if DOT_PLATFORM_WINDOWS
	::FreeLibrary((HMODULE)handle);
#else
#endif
}

inline void* dlsym(void *handle, const char * symbol)
{
#if DOT_PLATFORM_WINDOWS
	return (void *)::GetProcAddress((HMODULE)handle, symbol);
#else
#endif
}

}
}

#endif // __DOT_PLATFORMUTILS_H_HEADER_GUARD__
