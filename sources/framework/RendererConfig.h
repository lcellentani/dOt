#ifndef __DOT_RENDERERCONFIG_H_HEADER_GUARD__
#define __DOT_RENDERERCONFIG_H_HEADER_GUARD__

#include "PlatformConfig.h"

#ifndef DOT_RENDERER_OPENGL
#define DOT_RENDERER_OPENGL (DOT_PLATFORM_OSX || DOT_PLATFORM_WINDOWS ? 1 : 0)
#endif

#ifndef DOT_RENDERER_OPENGL_TARGET_VERSION
#define DOT_RENDERER_OPENGL_TARGET_VERSION 45
#endif

#endif // __DOT_RENDERERCONFIG_H_HEADER_GUARD__