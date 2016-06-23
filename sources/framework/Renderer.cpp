#include "Renderer.h"

#include "PlatformContext.h"
#include "RendererGL.h"

namespace dot
{
namespace rendering
{
	
Renderer * Renderer::CreateRenderer(dot::core::PlatformContext * const platformContext)
{
	Renderer *newRenderer = new RendererGL();
	newRenderer->Init(platformContext);

	return newRenderer;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init(dot::core::PlatformContext * const platformContext)
{
	return true;
}

} // namespace rendering
} // namespace dot
