#ifndef __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__
#define __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__

namespace dot
{
namespace core
{

class Application;

class PlatformContext
{
public:
	virtual void PollEvents(Application * const app) = 0;

	virtual bool const IsExiting() const = 0;
};

}
}

#endif // __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__