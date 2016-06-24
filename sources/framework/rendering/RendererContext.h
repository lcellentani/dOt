#ifndef __DOT_RENDERERCONTEXT_H_HEADER_GUARD__
#define __DOT_RENDERERCONTEXT_H_HEADER_GUARD__

#include "rendering/RendererConfig.h"

#include <stdint.h> // uint32_t

namespace dot { namespace core { class PlatformContext; } }

namespace dot
{
namespace rendering
{

class RendererContext
{
public:
	RendererContext() {};

	virtual void Create(dot::core::PlatformContext * const platformContext) = 0;
	virtual void Destroy() = 0;

	virtual void Resize(uint32_t width, uint32_t height) = 0;

	virtual void MakeCurrent() = 0;

	virtual void Swap() = 0;
};

} // namespace rendering
} // namespace dot

#endif // __DOT_RENDERERCONTEXT_H_HEADER_GUARD__