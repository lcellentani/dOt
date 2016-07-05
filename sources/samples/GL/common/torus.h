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

	int GetVertexArrayHandle() { return vaoHandle; }

private:
	void GenerateVerts(float *, float *, float *, unsigned int *, float, float);

private:
	uint32_t vaoHandle;
	uint32_t faces;
	uint32_t rings;
	uint32_t sides;
};

} // samples
} // gl
} // dot

#endif // __DOT_SAMPLES_TORUS_H_HEADER_GUARD__
