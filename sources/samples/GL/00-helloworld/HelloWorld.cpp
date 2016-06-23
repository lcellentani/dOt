
#include "dot.h"
#include "Log.h"

#include "RendererGLConfig.h"

#include <math.h>

class HelloWorldAppContext : public dot::core::AppContext
{
public:
	virtual const int GetWindowWidth() const OVERRIDE
	{
		return 1024;
	}
	virtual const int GetWindowHeight() const OVERRIDE
	{
		return 576;
	}

	virtual const bool IsDebugModeEnabled() const OVERRIDE
	{
		return true;
	}
};

//=====================================================================================================================

class HelloWorld : public dot::core::Application
{
public:
	HelloWorld() : Application() {}

	virtual void Initialize(dot::core::PlatformContext * const platformContext) OVERRIDE
	{
		Application::Initialize(platformContext);
	}

	virtual void Update() OVERRIDE
	{
		Application::Update();
	}

	virtual void Render() OVERRIDE
	{
		const GLfloat color[] = { (float)sin(GetTime()) * 0.5f + 0.5f, (float)cos(GetTime()) * 0.5f + 0.5f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
	}

	virtual int Shutdown() OVERRIDE
	{
		return 0;
	}

	virtual void Reshape(uint32_t width, uint32_t height) OVERRIDE
	{
		glViewport(0, 0, width, height);
	}
};

//=====================================================================================================================
//=====================================================================================================================

extern "C" dot::core::AppContext * CreateAppContext()
{
	return new HelloWorldAppContext();
}

extern "C" dot::core::Application * CreateApplication()
{
	return new HelloWorld();
}