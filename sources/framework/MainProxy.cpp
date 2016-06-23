#include "MainProxy.h"
#include "Application.h"

#include "Renderer.h"
#include "Log.h"

using namespace dot::rendering;

namespace dot
{
namespace core
{

int MainProxy(int argv, char ** argc, PlatformContext *platformContext)
{
	Application *theApp = CreateApplication();

	platformContext->CreateRenderer();

	theApp->Initialize(platformContext);

	while (!platformContext->IsExiting())
	{
		platformContext->PollEvents(theApp);

		theApp->Update();

		theApp->Render();

		platformContext->Flip();
	}

	return 0;
}

} // namespace core
} // namespace dot