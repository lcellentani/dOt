#ifndef __DOT_RENDERER_H_HEADER_GUARD__
#define __DOT_RENDERER_H_HEADER_GUARD__

#include "RendererConfig.h"

#if DOT_RENDERER_OPENGL
#	if DOT_RENDERER_OPENGL_TARGET_VERSION >= 31 && DOT_RENDERER_OPENGL_TARGET_VERSION <= 33
#		if DOT_RENDERER_OPENGL_TARGET_VERSION == 31
#			define DOT_RENDERER_OPENGL_NAME "OpenGL 3.1"
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 32
#			define DOT_RENDERER_OPENGL_NAME "OpenGL 3.2"
#		else
#			define DOT_RENDERER_OPENGL_NAME "OpenGL 3.3"
#		endif
#	elif DOT_RENDERER_OPENGL_TARGET_VERSION >= 40 && DOT_RENDERER_OPENGL_TARGET_VERSION <= 45
#		if DOT_RENDERER_OPENGL_TARGET_VERSION == 40
#			define DOT_RENDERER_OPENGL_NAME "OpenGL 4.0"
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 41
#			define DOT_RENDERER_OPENGL_NAME "OpenGL 4.1"
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 42
#			define DOT_RENDERER_OPENGL_NAME "OpenGL 4.2"
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 43
#			define DOT_RENDERER_OPENGL_NAME "OpenGL 4.3"
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 44
#			define DOT_RENDERER_OPENGL_NAME "OpenGL 4.4"
#		else
#			define DOT_RENDERER_OPENGL_NAME "OpenGL 4.5"
#		endif
#	else
#		define DOT_RENDERER_OPENGL_NAME "OpenGL 2.1"
#	endif
#endif

namespace dot { namespace core { class PlatformContext; } }

namespace dot
{
namespace rendering
{

enum RendererType
{
	Null,
	OpenGL,

	Count
};

class Renderer
{
public:
	static Renderer * CreateRenderer(dot::core::PlatformContext * const platformContext);

public:
	Renderer();
	virtual ~Renderer();

	virtual RendererType GetRendererType() const = 0;

	virtual const char * GetRendererName() const = 0;

	virtual void Flip() = 0;

	virtual bool Init(dot::core::PlatformContext * const platformContext);
};

} // namespace rendering
} // namespace dot

#endif // __DOT_RENDERER_H_HEADER_GUARD__