#ifndef __DOT_APPLICATION_H_HEADER_GUARD__
#define __DOT_APPLICATION_H_HEADER_GUARD__

#include <stdint.h> // uint32_t

namespace dot
{
namespace core
{

class Application
{

public:
	Application();
	virtual ~Application();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual int Shutdown() = 0;

	virtual void Reshape(uint32_t width, uint32_t height) {}

protected:

};

extern "C" Application * CreateApplication();

} // namespace core
} // namespace dot

#endif // __DOT_APPLICATION_H_HEADER_GUARD__