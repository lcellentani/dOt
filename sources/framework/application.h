#ifndef __DOT_APPLICATION_H_HEADER_GUARD__
#define __DOT_APPLICATION_H_HEADER_GUARD__

extern "C" int mainProxy(int argc, char** argv);

#define DECLARE_MAIN(AppClass)				\
int mainProxy(int argc, char** argv)		\
{											\
    AppClass app;							\
    return RunApp(&app, argc, argv);		\
}

namespace dot
{

class Application
{

public:
	Application();
	virtual ~Application();

	virtual void Initialize();
	
	virtual bool Update();

	virtual int Shutdown();
};

int RunApp(Application* theApp, int argc, char** argv);

} // namespace dot

#endif // __DOT_APPLICATION_H_HEADER_GUARD__