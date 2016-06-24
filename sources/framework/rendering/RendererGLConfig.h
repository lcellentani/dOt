#ifndef __DOT_RENDERERGLCONFIG_H_HEADER_GUARD__
#define __DOT_RENDERERGLCONFIG_H_HEADER_GUARD__

#include "rendering/RendererConfig.h"

#define DOT_USE_GL_DYNAMIC_LIB (DOT_PLATFORM_OSX || DOT_PLATFORM_WINDOWS )

#if DOT_RENDERER_OPENGL
//#	if DOT_RENDERER_OPENGL_TARGET_VERSION >= 31
//#		include <gl/glcorearb.h>
//#	else
//#		include <gl/GL.h>
#	if DOT_RENDERER_OPENGL_TARGET_VERSION >= 31 && DOT_RENDERER_OPENGL_TARGET_VERSION <= 33
#		if DOT_RENDERER_OPENGL_TARGET_VERSION == 31
#			include <gl_core_3_1.h>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 32
#			include <gl_core_3_2.h>
#		else
#			include <gl_core_3_3.h>
#		endif
#	elif DOT_RENDERER_OPENGL_TARGET_VERSION >= 40 && DOT_RENDERER_OPENGL_TARGET_VERSION <= 45
#		if DOT_RENDERER_OPENGL_TARGET_VERSION == 40
#			include <gl_core_4_0.h>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 41
#			include <gl_core_4_1.h>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 42
#			include <gl_core_4_2.h>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 43
#			include <gl_core_4_3.h>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 44
#			include <gl_core_4_4.h>
#		else
#			include <gl_core_4_5.h>
#		endif
#	else
#		include <gl_core_2_1.h>
#	endif
#endif

//#define GL_IMPORT_TYPEDEFS 1
//#define GL_IMPORT(_optional, _proto, _func, _import) extern _proto _func
//#include "glimports.h"

#endif // __DOT_RENDERERGLCONFIG_H_HEADER_GUARD__
