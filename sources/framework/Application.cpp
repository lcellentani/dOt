#include "Application.h"
#include "PlatformContext.h"

namespace dot
{
namespace core
{

Application::Application() : mFrameTimer(nullptr), mTime(0.0f), mDeltaTime(0.0f)
{
}

Application::~Application()
{

}

void Application::Initialize(PlatformContext * const platformContext)
{
	mFrameTimer = platformContext->CreateStopWatch();
	mFrameTimer->Start();
}

void Application::Update()
{
	mTime = mFrameTimer->GetTime();
}

} // namespace core
} // namespace dot