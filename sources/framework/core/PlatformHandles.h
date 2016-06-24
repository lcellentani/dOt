#ifndef __DOT_PLATFORMHANDLES_H_HEADER_GUARD__
#define __DOT_PLATFORMHANDLES_H_HEADER_GUARD__

#include <stdint.h> // uint16_t

#include "core/Handle.h"

namespace dot
{
namespace core
{

struct tagWindowHandle {};
typedef Handle<tagWindowHandle> WindowHandle;


} // namespace core
} // namespace dot

#endif // __DOT_PLATFORMHANDLES_H_HEADER_GUARD__