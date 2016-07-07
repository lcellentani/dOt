#ifndef __DOT_SAMPLES_TORUS_H_HEADER_GUARD__
#define __DOT_SAMPLES_TORUS_H_HEADER_GUARD__

#include "drawable.h"
#include <stdint.h> // uint32_t

namespace dot
{
namespace gl
{
namespace samples
{

class Torus : public Drawable
{
public:
	Torus(float outerRadius, float innerRadius, uint32_t nsides, uint32_t nrings);
	virtual ~Torus();

	virtual void Render() const override;

	int GetVertexArrayHandle() { return mVaoHandle; }

private:
	void GenerateVerts(float *, float *, float *, unsigned int *, float, float);

private:
	uint32_t mVaoHandle;
	uint32_t mFaces;
	uint32_t mRings;
	uint32_t mSides;
};

} // samples
} // gl
} // dot

#endif // __DOT_SAMPLES_TORUS_H_HEADER_GUARD__
