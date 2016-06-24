#include "rendering/RendererContextWGL.h"

#include "core/PlatformUtils.h"
#include "core/PlatformContext.h"
#include "core/GenericUtils.h"
#include "core/Log.h"

namespace dot
{
namespace rendering
{

//
#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

typedef PROC(APIENTRYP PFNWGLGETPROCADDRESSPROC) (LPCSTR lpszProc);
typedef BOOL(APIENTRYP PFNWGLMAKECURRENTPROC) (HDC hdc, HGLRC hglrc);
typedef HGLRC(APIENTRYP PFNWGLCREATECONTEXTPROC) (HDC hdc);
typedef BOOL(APIENTRYP PFNWGLDELETECONTEXTPROC) (HGLRC hglrc);

PFNWGLGETPROCADDRESSPROC wglGetProcAddress;
PFNWGLMAKECURRENTPROC wglMakeCurrent;
PFNWGLCREATECONTEXTPROC wglCreateContext;
PFNWGLDELETECONTEXTPROC wglDeleteContext;
//
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
//

RendererContextWGL::RendererContextWGL() : RendererContext()
{

}

RendererContextWGL::~RendererContextWGL()
{

}

void RendererContextWGL::Create(dot::core::PlatformContext * const platformContext)
{
	mDynLibHandle = dot::core::dlopen("opengl32.dll");

	wglGetProcAddress = (PFNWGLGETPROCADDRESSPROC)dot::core::dlsym(mDynLibHandle, "wglGetProcAddress");

	HWND hWindowHandle = (HWND)platformContext->GetNativeWindowHandle();
	if (hWindowHandle != 0)
	{
		wglMakeCurrent = (PFNWGLMAKECURRENTPROC)dot::core::dlsym(mDynLibHandle, "wglMakeCurrent");

		wglCreateContext = (PFNWGLCREATECONTEXTPROC)dot::core::dlsym(mDynLibHandle, "wglCreateContext");

		wglDeleteContext = (PFNWGLDELETECONTEXTPROC)dot::core::dlsym(mDynLibHandle, "wglDeleteContext");

		mHDC = GetDC(hWindowHandle);

		// An application can only set the pixel format of a window one time.
		// Once a window's pixel format is set, it cannot be changed.
		// MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/dd369049%28v=vs.85%29.aspx
		HWND hwnd = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, -32000, -32000, 0, 0, NULL, NULL, GetModuleHandle(NULL), 0);

		HDC hdc = GetDC(hwnd);

		HGLRC context = CreateContext(hdc);

		wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		if (wglGetExtensionsStringARB != nullptr)
		{
			const char * extensions = (const char *)wglGetExtensionsStringARB(hdc);
			DumpExtensions(extensions);
		}

		if (wglChoosePixelFormatARB != nullptr &&  wglCreateContextAttribsARB != nullptr)
		{
			int32_t attrs[] =
			{
				WGL_SAMPLE_BUFFERS_ARB, 0,
				WGL_SAMPLES_ARB, 0,
				WGL_SUPPORT_OPENGL_ARB, true,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_DRAW_TO_WINDOW_ARB, true,
				WGL_DOUBLE_BUFFER_ARB, true,
				WGL_COLOR_BITS_ARB, 32,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 8,
				0
			};

			int result;
			uint32_t numFormats = 0;
			do
			{
				result = wglChoosePixelFormatARB(mHDC, attrs, NULL, 1, &mPixelFormat, &numFormats);
				if (result == 0 || numFormats == 0)
				{
					attrs[3] >>= 1;
					attrs[1] = attrs[3] == 0 ? 0 : 1;
				}

			} while (numFormats == 0);

			DescribePixelFormat(mHDC, mPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &mPixFormatDesc);

			LOGI("Pixel format:\n\tiPixelType %d\n\tcColorBits %d\n\tcAlphaBits %d\n\tcDepthBits %d\n\tcStencilBits %d\n",
				mPixFormatDesc.iPixelType,
				mPixFormatDesc.cColorBits,
				mPixFormatDesc.cAlphaBits,
				mPixFormatDesc.cDepthBits,
				mPixFormatDesc.cStencilBits);

			result = SetPixelFormat(mHDC, mPixelFormat, &mPixFormatDesc);

			int32_t flags = platformContext->IsDebugModeEnabled() ? WGL_CONTEXT_DEBUG_BIT_ARB : 0;
			int32_t contextAttrs[9] =
			{
#if DOT_RENDERER_OPENGL_TARGET_VERSION >= 31
				WGL_CONTEXT_MAJOR_VERSION_ARB, DOT_RENDERER_OPENGL_TARGET_VERSION / 10,
				WGL_CONTEXT_MINOR_VERSION_ARB, DOT_RENDERER_OPENGL_TARGET_VERSION % 10,
				WGL_CONTEXT_FLAGS_ARB, flags,
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#else
				WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
				WGL_CONTEXT_MINOR_VERSION_ARB, 1,
				0, 0,
				0, 0,
#endif // BGFX_CONFIG_RENDERER_OPENGL >= 31
				0
			};

			mContext = wglCreateContextAttribsARB(mHDC, 0, contextAttrs);
			if (mContext == 0)
			{
				// nVidia doesn't like context profile mask for contexts below 3.2?
				contextAttrs[6] = WGL_CONTEXT_PROFILE_MASK_ARB == contextAttrs[6] ? 0 : contextAttrs[6];
				mContext = wglCreateContextAttribsARB(mHDC, 0, contextAttrs);
			}

			memcpy(mContextAttrs, contextAttrs, sizeof(contextAttrs));
		}

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(context);
		DestroyWindow(hwnd);

		if (mContext == 0)
		{
			mContext = CreateContext(mHDC);
		}

		int result = wglMakeCurrent(mHDC, mContext);
		//m_current = NULL;

		if (wglSwapIntervalEXT != 0)
		{
			wglSwapIntervalEXT(0);
		}

		//Import();
	}
}

void RendererContextWGL::Destroy()
{
	if (mNativeWindowHandle != 0)
	{
		wglMakeCurrent(0, 0);

		if (mContext != 0)
		{
			wglDeleteContext(mContext);
			mContext = 0;
		}

		if (mHDC != 0)
		{
			ReleaseDC(mNativeWindowHandle, mHDC);
			mHDC = 0;
		}
	}

	if (mDynLibHandle != 0)
	{
		dot::core::dlclose(mDynLibHandle);
		mDynLibHandle = 0;
	}
}

void RendererContextWGL::Resize(uint32_t width, uint32_t height)
{

}

void RendererContextWGL::MakeCurrent()
{
	wglMakeCurrent(mHDC, mContext);
	GLenum err = glGetError();
	if (err != 0)
	{
		LOGE("wglMakeCurrent failed with GL error: 0x%04x.", err);
	}
}

void RendererContextWGL::Swap()
{
	MakeCurrent();

	SwapBuffers(mHDC);
}

HGLRC RendererContextWGL::CreateContext(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);

	DescribePixelFormat(hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	LOGI("Pixel format:\n\tiPixelType %d\n\tcColorBits %d\n\tcAlphaBits %d\n\tcDepthBits %d\n\tcStencilBits %d\n", pfd.iPixelType, pfd.cColorBits, pfd.cAlphaBits, pfd.cDepthBits, pfd.cStencilBits);

	int result;
	result = SetPixelFormat(hdc, pixelFormat, &pfd);

	HGLRC context = wglCreateContext(hdc);

	result = wglMakeCurrent(hdc, context);

	return context;
}

void RendererContextWGL::DumpExtensions(const char * extensions)
{
	if (extensions != nullptr)
	{
		LOGI("WGL extensions:");

		char name[1024];
		const char* pos = extensions;
		const char* end = extensions + strlen(extensions);
		while (pos < end)
		{
			uint32_t len;
			const char* space = strchr(pos, ' ');
			if (NULL != space)
			{
				len = dot::core::min<uint32_t>(sizeof(name), (uint32_t)(space - pos));
			}
			else
			{
				len = dot::core::min<uint32_t>(sizeof(name), (uint32_t)strlen(pos));
			}

			strncpy(name, pos, len);
			name[len] = '\0';

			LOGI("\t%s", name);

			pos += len + 1;
		}
	}
}

void RendererContextWGL::Import()
{
	/*
	LOGI("Import:");
#	define GL_EXTENSION(_optional, _proto, _func, _import) \
				{ \
					if (NULL == _func) \
					{ \
						_func = (_proto)wglGetProcAddress(#_import); \
						if (_func == NULL) \
						{ \
							_func = (_proto)dot::core::dlsym(mDynLibHandle, #_import); \
							LOGI("    %p " #_func " (" #_import ")", _func); \
						} \
						else \
						{ \
							LOGI("wgl %p " #_func " (" #_import ")", _func); \
						} \
					} \
				}
#	include "glimports.h"
	*/
}

} // namespace rendering
} // namespace dot