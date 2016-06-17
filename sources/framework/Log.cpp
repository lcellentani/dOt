#include "Log.h"

namespace dot
{

#if DOT_PLATFORM_WINDOWS

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>

void WindowsLog(const char* fmt, ...)
{
	const int length = 1024;
	char buffer[length];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf_s(buffer, length - 1, fmt, ap);
	OutputDebugString(buffer);
	OutputDebugString("\n");
	va_end(ap);
}

#endif

} // namespace dot
