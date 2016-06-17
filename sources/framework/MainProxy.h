#ifndef __DOT_MAINPROXY_H_HEADER_GUARD__
#define __DOT_MAINPROXY_H_HEADER_GUARD__

#include <stdint.h> // uint16_t

#include "PlatformContext.h"
#include "SPSCQueue.h"

namespace dot
{
namespace core
{

extern "C" int MainProxy(int argv, char ** argc, PlatformContext *platformContext);

} // namespace core
} // namespace dot

#endif // __DOT_MAINPROXY_H_HEADER_GUARD__