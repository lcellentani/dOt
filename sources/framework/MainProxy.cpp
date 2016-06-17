#include "MainProxy.h"
#include "Application.h"

namespace dot
{
namespace core
{

int MainProxy(int argv, char ** argc, PlatformContext *platformContext)
{
	Application *theApp = CreateApplication();

	while (!platformContext->IsExiting())
	{
		platformContext->PollEvents(theApp);
	}

	return 0;
}

} // namespace core
} // namespace dot