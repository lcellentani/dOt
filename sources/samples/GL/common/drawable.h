#ifndef __DOT_SAMPLES_DRAWABLE_H_HEADER_GUARD__
#define __DOT_SAMPLES_DRAWABLE_H_HEADER_GUARD__

namespace dot
{
namespace gl
{
namespace samples
{

class Drawable
{
public:
	Drawable() {};
	virtual ~Drawable() {};

    virtual void Render() const = 0;
};

}
}
}

#endif // __DOT_SAMPLES_DRAWABLE_H_HEADER_GUARD__
