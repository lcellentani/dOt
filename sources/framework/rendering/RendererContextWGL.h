#ifndef __DOT_RENDERERCONTEXTWGL_H_HEADER_GUARD__
#define __DOT_RENDERERCONTEXTWGL_H_HEADER_GUARD__

#include "rendering/RendererContext.h"

#include "rendering/RendererGLConfig.h"
#include <wgl/wglext.h>

#include "core/PlatformMacros.h"

namespace dot
{
namespace rendering
{

class RendererContextWGL : public RendererContext
{
public:
	RendererContextWGL();
	virtual ~RendererContextWGL();

	virtual void Create(dot::core::PlatformContext * const platformContext) OVERRIDE;
	virtual void Destroy() OVERRIDE;

	virtual void Resize(uint32_t width, uint32_t height) OVERRIDE;

	virtual void MakeCurrent() OVERRIDE;

	virtual void Swap() OVERRIDE;

private:
	HGLRC CreateContext(HDC hdc);
	void DumpExtensions(const char * extensions);
	void Import();

private:
	void * mDynLibHandle;
	HGLRC mContext;
	HDC mHDC;
	HWND mNativeWindowHandle;
	int32_t mContextAttrs[9];
	int mPixelFormat;
	PIXELFORMATDESCRIPTOR mPixFormatDesc;
};

} // namespace rendering
} // namespace dot

#endif // __DOT_RENDERERCONTEXT_H_HEADER_GUARD__