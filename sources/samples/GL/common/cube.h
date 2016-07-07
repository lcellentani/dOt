#ifndef __DOT_SAMPLES_CUBE_H_HEADER_GUARD__
#define __DOT_SAMPLES_CUBE_H_HEADER_GUARD__

#include "drawable.h"
#include <stdint.h> // uint32_t

namespace dot
{
namespace gl
{
namespace samples
{

class Cube : Drawable
{
public:
    Cube();
	virtual ~Cube();

	virtual void Render() const override;

private:
	uint32_t mVaoHandle;
};

} // samples
} // gl
} // dot

#endif // __DOT_SAMPLES_CUBE_H_HEADER_GUARD__
