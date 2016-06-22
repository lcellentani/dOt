#include "Renderer.h"

#include "PlatformContext.h"
#include "RendererGL.h"

namespace dot
{
namespace rendering
{
	
Renderer * Renderer::CreateRenderer(dot::core::PlatformContext *platformContext)
{
	Renderer *newRenderer = new RendererGL();
	newRenderer->Init();

	return newRenderer;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	return true;
}

} // namespace rendering
} // namespace dot
