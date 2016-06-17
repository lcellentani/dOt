#ifndef __DOT_LOG_H_HEADER_GUARD__
#define __DOT_LOG_H_HEADER_GUARD__

#include "PlatformMacros.h"
#include "PlatformConfig.h"

/// \file
/// Cross-platform application logging to file and console.
/// - LOGI(...) printf-style "info" logging
/// - LOGE(...) printf-style "error" logging
/// - CHECK_GL_ERROR() check the current GL error status and log any error

#define LOG_TAG    "dOt"

#if DOT_PLATFORM_WINDOWS

#include <stdio.h>

namespace dot
{
extern void WindowsLog(const char* fmt, ...);
} // namespace dot

#define LOGI(...) { dot::WindowsLog(__VA_ARGS__); }
#define LOGE(...) { dot::WindowsLog(__VA_ARGS__); }

#elif DOT_PLATFORM_ANDROID

#include <stdlib.h> // for exit()
#include <android/log.h>

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  { __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__); }

#elif DOT_PLATFORM_OSX

#include <stdlib.h> // for exit()
#include <stdio.h>

#define LOGI(...) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); }
#define LOGE(...) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); }

#else

#error "No supported platform specified for Logs.h"

#endif


#endif // __DOT_LOG_H_HEADER_GUARD__