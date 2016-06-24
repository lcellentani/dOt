#ifndef __DOT_PLATFORMMACROS_H_HEADER_GUARD__
#define __DOT_PLATFORMMACROS_H_HEADER_GUARD__

#define OVERRIDE override

#define DOT_MAKEFOURCC(_a, _b, _c, _d) ( ( (uint32_t)(_a) | ( (uint32_t)(_b) << 8) | ( (uint32_t)(_c) << 16) | ( (uint32_t)(_d) << 24) ) )

#define DOT_STRINGIZE(_x) DOT_STRINGIZE_(_x)
#define DOT_STRINGIZE_(_x) #_x

#define DOT_CONCATENATE(_x, _y) DOT_CONCATENATE_(_x, _y)
#define DOT_CONCATENATE_(_x, _y) _x ## _y

#endif // __DOT_PLATFORMMACROS_H_HEADER_GUARD__