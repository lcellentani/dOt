#ifndef __DOT_RENDERERGL_H_HEADER_GUARD__
#define __DOT_RENDERERGL_H_HEADER_GUARD__

#include "rendering/Renderer.h"
#include "rendering/RendererGLConfig.h"

#include "core/PlatformMacros.h"

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

	const char* mVendorString;
	const char* mRendererString;
	const char* mVersionString;
	const char* mGLSLlVersionString;
};

} // namespace rendering
} // namespace dot

#endif // __DOT_RENDERERGL_H_HEADER_GUARD__