
#include "dot.h"
#include "Log.h"

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
};

//=====================================================================================================================

class HelloWorld : public dot::core::Application
{
public:
	HelloWorld() : Application() {}

	virtual void Initialize() OVERRIDE
	{
	}

	virtual void Update() OVERRIDE
	{
	}

	virtual void Render() OVERRIDE
	{

	}

	virtual int Shutdown() OVERRIDE
	{
		return 0;
	}

	virtual void Reshape(uint32_t width, uint32_t height) OVERRIDE
	{
		LOGI("[HelloWorld::Reshape]: width = %d, height = %d", width, height);
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