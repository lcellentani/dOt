#include "RendererGL.h"
#include "RendererContextWGL.h"

#include "Log.h"

namespace dot
{
namespace rendering
{

RendererGL::RendererGL() : Renderer()
, mContextGL(nullptr)
{

}

RendererGL::~RendererGL()
{

}

RendererType RendererGL::GetRendererType() const
{
	return RendererType::OpenGL;
}

const char * RendererGL::GetRendererName() const
{
	return DOT_RENDERER_OPENGL_NAME;
}

void RendererGL::Flip()
{
	mContextGL->Swap();
}

bool RendererGL::Init(dot::core::PlatformContext * const platformContext)
{
	if (Renderer::Init(platformContext))
	{
		mContextGL = new RendererContextWGL();
		mContextGL->Create(platformContext);

		int loaded = ogl_LoadFunctions();
		if (loaded == ogl_LOAD_FAILED)
		{
			LOGE("Cannot load GL functions\n");
			return false;
		}

		return true;
	}

	return false;
}

} // namespace rendering
} // namespace dot