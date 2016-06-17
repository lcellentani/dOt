#ifndef __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__
#define __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__

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

protected:
	EventQueue *mEventQueue;

	bool mExitRequest;
};

}
}

#endif // __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__