#ifndef __DOT_RENDERER_H_HEADER_GUARD__
#define __DOT_RENDERER_H_HEADER_GUARD__

#include "rendering/RendererConfig.h"

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