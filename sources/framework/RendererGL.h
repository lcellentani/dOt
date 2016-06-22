#ifndef __DOT_RENDERERGL_H_HEADER_GUARD__
#define __DOT_RENDERERGL_H_HEADER_GUARD__

#include "Renderer.h"
#include "PlatformMacros.h"

#if DOT_RENDERER_OPENGL
#	if DOT_RENDERER_OPENGL_TARGET_VERSION >= 31
#		include <gl/glcorearb.h>
#	else
#		include <gl/GL.h>
#	endif
#endif

namespace dot
{
namespace rendering
{

class RendererGL : public Renderer
{
public:
	RendererGL();
	virtual ~RendererGL();

	virtual RendererType GetRendererType() const OVERRIDE;

	virtual const char * GetRendererName() const OVERRIDE;

	virtual void Flip() OVERRIDE;

	virtual bool Init() OVERRIDE;
};

} // namespace rendering
} // namespace dot

#endif // __DOT_RENDERERGL_H_HEADER_GUARD__