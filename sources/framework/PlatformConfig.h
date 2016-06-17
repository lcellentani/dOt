#ifndef __DOT_PLATFORMCONFIG_H_HEADER_GUARD__
#define __DOT_PLATFORMCONFIG_H_HEADER_GUARD__

#define DOT_COMPILER_CLANG           0
#define DOT_COMPILER_CLANG_ANALYZER  0
#define DOT_COMPILER_GCC             0
#define DOT_COMPILER_MSVC            0
#define DOT_COMPILER_MSVC_COMPATIBLE 0

#define DOT_PLATFORM_ANDROID    0
#define DOT_PLATFORM_IOS        0
#define DOT_PLATFORM_OSX        0
#define DOT_PLATFORM_WINDOWS    0

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(__clang__)
// clang defines __GNUC__ or _MSC_VER
#	undef  DOT_COMPILER_CLANG
#	define DOT_COMPILER_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#	if defined(__clang_analyzer__)
#		undef  DOT_COMPILER_CLANG_ANALYZER
#		define DOT_COMPILER_CLANG_ANALYZER 1
#	endif // defined(__clang_analyzer__)
#	if defined(_MSC_VER)
#		undef  DOT_COMPILER_MSVC_COMPATIBLE
#		define DOT_COMPILER_MSVC_COMPATIBLE _MSC_VER
#	endif // defined(_MSC_VER)
#elif defined(_MSC_VER)
#	undef  DOT_COMPILER_MSVC
#	define DOT_COMPILER_MSVC _MSC_VER
#	undef  DOT_COMPILER_MSVC_COMPATIBLE
#	define DOT_COMPILER_MSVC_COMPATIBLE _MSC_VER
#elif defined(__GNUC__)
#	undef  DOT_COMPILER_GCC
#	define DOT_COMPILER_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#	error "DOT_COMPILER_* is not defined!"
#endif //

// https://sourceforge.net/p/predef/wiki/Home/
#if defined(_WIN32) || defined(_WIN64)
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif // NOMINMAX
#	undef  DOT_PLATFORM_WINDOWS
#	define DOT_PLATFORM_WINDOWS 1
#elif defined(__ANDROID__)
// Android compiler defines __linux__
#	include <android/api-level.h>
#	undef  DOT_PLATFORM_ANDROID
#	define DOT_PLATFORM_ANDROID __ANDROID_API__
#elif  defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) || defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#	undef  DOT_PLATFORM_IOS
#	define DOT_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#	undef  DOT_PLATFORM_OSX
#	define DOT_PLATFORM_OSX 1
#else
#	error "DOT_PLATFORM_* is not defined!"
#endif

#if DOT_PLATFORM_ANDROID
#	define DOT_PLATFORM_NAME "Android " DOT_STRINGIZE(DOT_PLATFORM_ANDROID)
#elif DOT_PLATFORM_IOS
#	define DOT_PLATFORM_NAME "iOS"
#elif DOT_PLATFORM_OSX
#	define DOT_PLATFORM_NAME "OSX"
#elif DOT_PLATFORM_WINDOWS
#	define DOT_PLATFORM_NAME "Windows"
#endif // DOT_PLATFORM_

#define DOT_PLATFORM_POSIX (0 \
						|| DOT_PLATFORM_ANDROID \
						|| DOT_PLATFORM_IOS \
						|| DOT_PLATFORM_OSX \
						)

#ifndef  DOT_CONFIG_ENABLE_MSVC_LEVEL4_WARNINGS
#	define DOT_CONFIG_ENABLE_MSVC_LEVEL4_WARNINGS 0
#endif // DOT_CONFIG_ENABLE_MSVC_LEVEL4_WARNINGS

#if DOT_CONFIG_ENABLE_MSVC_LEVEL4_WARNINGS && DOT_COMPILER_MSVC
#	pragma warning(error:4062) // ENABLE warning C4062: enumerator'...' in switch of enum '...' is not handled
#	pragma warning(error:4121) // ENABLE warning C4121: 'symbol' : alignment of a member was sensitive to packing
//#	pragma warning(error:4127) // ENABLE warning C4127: conditional expression is constant
#	pragma warning(error:4130) // ENABLE warning C4130: 'operator' : logical operation on address of string constant
#	pragma warning(error:4239) // ENABLE warning C4239: nonstandard extension used : 'argument' : conversion from '*' to '* &' A non-const reference may only be bound to an lvalue
//#	pragma warning(error:4244) // ENABLE warning C4244: 'argument' : conversion from 'type1' to 'type2', possible loss of data
#	pragma warning(error:4245) // ENABLE warning C4245: 'conversion' : conversion from 'type1' to 'type2', signed/unsigned mismatch
#	pragma warning(error:4263) // ENABLE warning C4263: 'function' : member function does not override any base class virtual member function
#	pragma warning(error:4265) // ENABLE warning C4265: class has virtual functions, but destructor is not virtual
#	pragma warning(error:4431) // ENABLE warning C4431: missing type specifier - int assumed. Note: C no longer supports default-int
#	pragma warning(error:4545) // ENABLE warning C4545: expression before comma evaluates to a function which is missing an argument list
#	pragma warning(error:4549) // ENABLE warning C4549: 'operator' : operator before comma has no effect; did you intend 'operator'?
#	pragma warning(error:4701) // ENABLE warning C4701: potentially uninitialized local variable 'name' used
#	pragma warning(error:4706) // ENABLE warning C4706: assignment within conditional expression
#	pragma warning(error:4100) // ENABLE warning C4100: '' : unreferenced formal parameter
#	pragma warning(error:4189) // ENABLE warning C4189: '' : local variable is initialized but not referenced
#	pragma warning(error:4505) // ENABLE warning C4505: '' : unreferenced local function has been removed
#endif // DOT_CONFIG_ENABLE_MSVC_LEVEL4_WARNINGS && DOT_COMPILER_MSVC

#endif // __DOT_PLATFORMCONFIG_H_HEADER_GUARD__