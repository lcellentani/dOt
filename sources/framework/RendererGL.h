#ifndef __DOT_RENDERERGL_H_HEADER_GUARD__
#define __DOT_RENDERERGL_H_HEADER_GUARD__

#include "Renderer.h"
#include "RendererGLConfig.h"

#include "PlatformMacros.h"

namespace dot { namespace rendering { class RendererContextWGL;  } }

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

	virtual bool Init(dot::core::PlatformContext * const platformContext) OVERRIDE;

private:
	RendererContextWGL *mContextGL;
};

} // namespace rendering
} // namespace dot

#endif // __DOT_RENDERERGL_H_HEADER_GUARD__