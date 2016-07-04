#include "core/MainProxy.h"
#include "core/PlatformContext.h"
#include "core/Application.h"

#include "rendering/Renderer.h"
#include "core/Log.h"

using namespace dot::rendering;

namespace dot
{
namespace core
{

int MainProxy(int argv, char ** argc, PlatformContext *platformContext)
{
	Application *theApp = CreateApplication();

	Renderer *renderer = platformContext->CreateRenderer();

	theApp->Initialize(platformContext);

	while (!platformContext->IsExiting())
	{
		platformContext->PollEvents(theApp);

		theApp->Update();

		theApp->Render();

		renderer->Flip();
	}

	theApp->Shutdown();

	return 0;
}

} // namespace core
} // namespace dot