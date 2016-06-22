#include "RendererGL.h"

namespace dot
{
namespace rendering
{

RendererGL::RendererGL() : Renderer()
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

}

bool RendererGL::Init()
{
	if (Renderer::Init())
	{
		return true;
	}

	return false;
}

} // namespace rendering
} // namespace dot