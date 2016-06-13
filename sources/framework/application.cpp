#include "application.h"

#include <stdint.h> // uint32_t

namespace dot
{

Application::Application()
{

}

Application::~Application()
{

}

void Application::Initialize()
{
}

bool Application::Update()
{
	return false;
}

int Application::Shutdown()
{
	return 0;
}

int RunApp(Application* theApp, int argc, char** argv)
{
	theApp->Initialize();

	//WindowHandle defaultWindow = { 0 };
	//setWindowSize(defaultWindow, ENTRY_DEFAULT_WIDTH, ENTRY_DEFAULT_HEIGHT);

	while (theApp->Update());

	return theApp->Shutdown();
}

int main(int argc, char** argv)
{
	int32_t result = ::mainProxy(argc, argv);

	return result;
}

} // namespace dot