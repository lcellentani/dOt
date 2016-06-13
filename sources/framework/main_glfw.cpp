#include "main_p.h"

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
		int result = main(argc, argv);

		return result;
	}
};

AppContext context;

} // namespace dot

int main(int argc, char** argv)
{
	using namespace dot;
	return context.Run(argc, argv);
}