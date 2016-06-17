#ifndef __DOT_PLATFORMHANDLES_H_HEADER_GUARD__
#define __DOT_PLATFORMHANDLES_H_HEADER_GUARD__

namespace dot
{
namespace core
{

struct WindowHandle
{
	WindowHandle(uint16_t idx) : mIdx(idx) {}

	uint16_t mIdx;
};
inline bool IsValid(WindowHandle handle) { return UINT16_MAX != handle.mIdx; }

} // namespace core
} // namespace dot

#endif // __DOT_PLATFORMHANDLES_H_HEADER_GUARD__