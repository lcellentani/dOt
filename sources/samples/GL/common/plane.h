#ifndef __DOT_SAMPLES_PLANE_H_HEADER_GUARD__
#define __DOT_SAMPLES_PLANE_H_HEADER_GUARD__

#include "drawable.h"
#include <stdint.h> // uint32_t

namespace dot
{
namespace gl
{
namespace samples
{

class Plane : public Drawable
{
public:
    Plane(float xsize, float zsize, int xdivs, int zdivs, float smax = 1.0f, float tmax = 1.0f);
	virtual ~Plane();

	virtual void Render() const override;

private:
	uint32_t mVaoHandle;
	uint32_t faces;
};

} // samples
} // gl
} // dot

#endif // __DOT_SAMPLES_PLANE_H_HEADER_GUARD__
