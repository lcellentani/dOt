#include "core/PlatformContext.h"
#include "core/PlatformEvent.h"
#include "core/Application.h"
#include "rendering/Renderer.h"

using namespace dot::rendering;

namespace dot
{
namespace core
{

PlatformContext::PlatformContext() : mEventQueue(new EventQueue()), mExitRequest(false), mRenderer(nullptr)
{

}

PlatformContext::~PlatformContext()
{
	if (mEventQueue != nullptr)
	{
		delete mEventQueue;
		mEventQueue = nullptr;
	}
}

void PlatformContext::PollEvents(Application * const app)
{
	const Event *evt = nullptr;
	do
	{
		evt = mEventQueue->Poll();
		if (evt != nullptr)
		{
			switch (evt->GetType())
			{
			case Event::Exit:
				mExitRequest = true;
				break;
			case Event::Size:
			{
				const SizeEvent* sizeEvt = static_cast<const SizeEvent *>(evt);
				if (app != nullptr)
				{
					app->Reshape(sizeEvt->mWidth, sizeEvt->mHeight);
				}
			}
			break;
			default:
				break;
			}

			mEventQueue->Release(evt);
		}
	} while (evt != nullptr && !mExitRequest);
}

bool const PlatformContext::IsExiting() const
{
	return mExitRequest;
}

Renderer * const PlatformContext::CreateRenderer()
{
	mRenderer = Renderer::CreateRenderer(this);
	return mRenderer;
}

} // namespace core
} // namespace dot
