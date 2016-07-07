#ifndef __DOT_SAMPLES_SPHERE_H_HEADER_GUARD__
#define __DOT_SAMPLES_SPHERE_H_HEADER_GUARD__

#include "drawable.h"
#include <stdint.h> // uint32_t

namespace dot
{
namespace gl
{
namespace samples
{

class Sphere : public Drawable
{
public:
    Sphere(float, uint32_t, uint32_t);
	virtual ~Sphere();

	virtual void Render() const override;

	int GetVertexArrayHandle() { return mVaoHandle; }

private:
	uint32_t mVaoHandle;
	uint32_t mNumVerts;
	uint32_t mNumElements;
	float mRadius;
	uint32_t mSlices;
	uint32_t mStacks;

	void GenerateVerts(float *, float *, float *, uint32_t *);
};

} // samples
} // gl
} // dot

#endif // __DOT_SAMPLES_SPHERE_H_HEADER_GUARD__
