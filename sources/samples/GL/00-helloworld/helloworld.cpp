
#include "application.h"


class HelloWorld : public dot::Application
{
public:
	virtual void Initialize() override
	{
		dot::Application::Initialize();
	}

	virtual bool Update() override
	{
		return true;
	}
};

DECLARE_MAIN(HelloWorld)
