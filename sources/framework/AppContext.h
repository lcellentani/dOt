#ifndef __DOT_APPCONTEXT_H_HEADER_GUARD__
#define __DOT_APPCONTEXT_H_HEADER_GUARD__

namespace dot
{
namespace core
{

class AppContext
{
public:
	virtual const int GetWindowWidth() const = 0;
	virtual const int GetWindowHeight() const = 0;
};

extern "C" AppContext * CreateAppContext();

} // namespace core
} // namespace dot

#endif // __DOT_APPCONTEXT_H_HEADER_GUARD__