#ifndef __DOT_GENERICUTILS_H_HEADER_GUARD__
#define __DOT_GENERICUTILS_H_HEADER_GUARD__

namespace dot
{
namespace core
{

template<typename T>
T const & min(T const & a, T const & b)
{
	return !(b < a) ? a : b;
}

template<typename T>
T const & max(T const & a, T const & b)
{
	return (b < a) ? a : b;
}


} // namespace core
} // namespace dot


#endif // __DOT_GENERICUTILS_H_HEADER_GUARD__