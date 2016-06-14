#include "main_p.h"

#include "glfw/glfw3.h"

namespace dot
{

class AppContext
{
public:
	AppContext()
	{

	}

	~AppContext()
	{

	}

	int Run(int argc, char** argv)
	{
		if (!glfwInit())
		{
			return -1;
		}

		window = glfwCreateWindow(1280, 720, "dOt", NULL, NULL);
		glfwMakeContextCurrent(window);

		int result = main(argc, argv);

		glfwDestroyWindow(window);
		glfwTerminate();

		return result;
	}

private:
	GLFWwindow* window;
};

AppContext context;

} // namespace dot

int main(int argc, char** argv)
{
	using namespace dot;
	return context.Run(argc, argv);
}