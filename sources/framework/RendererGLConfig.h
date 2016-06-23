#ifndef __DOT_RENDERERGLCONFIG_H_HEADER_GUARD__
#define __DOT_RENDERERGLCONFIG_H_HEADER_GUARD__

#include "RendererConfig.h"

#define DOT_USE_GL_DYNAMIC_LIB (DOT_PLATFORM_OSX || DOT_PLATFORM_WINDOWS )


#if DOT_RENDERER_OPENGL
#	if DOT_RENDERER_OPENGL_TARGET_VERSION >= 31
//#		include <gl/glcorearb.h>
#		include <gl_core_4_5.h>
#	else
#		include <gl/GL.h>
#	endif
#endif

//#define GL_IMPORT_TYPEDEFS 1
//#define GL_IMPORT(_optional, _proto, _func, _import) extern _proto _func
//#include "glimports.h"

#endif // __DOT_RENDERERGLCONFIG_H_HEADER_GUARD__
