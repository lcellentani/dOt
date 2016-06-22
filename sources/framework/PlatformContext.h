#ifndef __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__
#define __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__

namespace dot { namespace rendering { class Renderer; } }

namespace dot
{
namespace core
{

class Application;
class EventQueue;

class PlatformContext
{
public:
	PlatformContext();
	virtual ~PlatformContext();

	virtual void PollEvents(Application * const app);
	virtual bool const IsExiting() const;

	bool CreateRenderer();

protected:
	EventQueue *mEventQueue;

	bool mExitRequest;

	dot::rendering::Renderer * renderer;
};

}
}

#endif // __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__