#include "dot.h"
#include "Log.h"
#include "MainProxy.h"
#include "PlatformContext.h"
#include "Application.h"

#include <stdint.h> // uint32_t
#include <windowsx.h>

#if DOT_PLATFORM_WINDOWS

using namespace dot::core;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam);

struct MainThreadContext
{
	int mArgc;
	char **mArgv;

	dot::core::PlatformContext *platformContext;

	static int32_t ThreadFunc(void *userData);
};

class Win32AppContext : public PlatformContext
{
public:
	Win32AppContext()
		: mAppContext(nullptr)
		, mHwnd(0)
		, mRect()
		, mWindowWidth(0)
		, mWindowHeight(0)
		, mWidth(0)
		, mHeight(0)
		, mInit(false)
		, mExit(false)
		, mWindowed(true)
		, mExitRequest(false)
	{
	}

	virtual void PollEvents(Application * const app) OVERRIDE
	{
		const Event *evt = nullptr;
		do
		{
			evt = mEventQueue.Poll();
			if (evt != nullptr)
			{
				switch (evt->GetType())
				{
				case Event::Exit:
					mExitRequest = true;
					break;
				case Event::Size:
					{
						const SizeEvent* sizeEvt = static_cast<const SizeEvent *>(evt);
						if (app != nullptr)
						{
							app->Reshape(sizeEvt->mWidth, sizeEvt->mHeight);
						}
					}
					break;
				default:
					break;
				}

				mEventQueue.Release(evt);
			}
		} while (evt != nullptr && !mExitRequest);
	}

	virtual bool const IsExiting() const OVERRIDE
	{
		return mExitRequest;
	}

	int Run(int argc, char** argv)
	{
		mAppContext = CreateAppContext();

		int defaultWidth = mAppContext->GetWindowWidth();
		int defaultHeight = mAppContext->GetWindowHeight();

		SetDllDirectory(".");
		HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);

		WNDCLASSEX wnd;
		memset(&wnd, 0, sizeof(wnd));
		wnd.cbSize = sizeof(wnd);
		wnd.style = CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc = WndProc;
		wnd.hInstance = instance;
		wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
		wnd.lpszClassName = "dOt";
		wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassExA(&wnd);

		mHwnd = CreateWindowA("dOt", "dOt", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, defaultWidth, defaultHeight, NULL, NULL, instance, 0);

		ReshapeWindow(mHwnd, defaultWidth, defaultHeight, true);
		ClearWindow(mHwnd);

		MainThreadContext mtc;
		mtc.mArgc = argc;
		mtc.mArgv = argv;
		dot::core::Thread thread;
		thread.Start(mtc.ThreadFunc, &mtc);

		mInit = true;

		mEventQueue.PostSizeEvent(FindHandle(mHwnd), mWidth, mHeight);

		MSG msg;
		msg.message = WM_NULL;

		while (!mExit)
		{
			WaitForInputIdle(GetCurrentProcess(), 16);

			while (0 != PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		thread.Shutdown();

		DestroyWindow(mHwnd);

		return thread.GetExitCode();
	}

	LRESULT ProcessMessages(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam)
	{
		if (mInit)
		{
			switch (id)
			{
			case WM_DESTROY:
				break;

			case WM_QUIT:
			case WM_CLOSE:
				if (hwnd == mHwnd)
				{
					mExit = true;
					mEventQueue.PostExitEvent();
				}
				//else
				//{
				//	destroyWindow(findHandle(_hwnd));
				//}
				return 0;

			case WM_SIZE:
			{
				WindowHandle handle = FindHandle(hwnd);
				if (IsValid(handle))
				{
					uint32_t width = GET_X_LPARAM(lparam);
					uint32_t height = GET_Y_LPARAM(lparam);

					mWidth = width;
					mHeight = height;
					mEventQueue.PostSizeEvent(handle, mWidth, mHeight);
				}
			}
			break;

			case WM_SYSCOMMAND:
				switch (wparam)
				{
				case SC_MINIMIZE:
				case SC_RESTORE:
					{
						HWND parent = GetWindow(hwnd, GW_OWNER);
						if (NULL != parent)
						{
							PostMessage(parent, id, wparam, lparam);
						}
					}
				}
				break;

			default:
				break;
			}
		}
		return DefWindowProc(hwnd, id, wparam, lparam);
	}

	void ReshapeWindow(HWND hwnd, uint32_t width, uint32_t height, bool windowFrame)
	{
		mWidth = width;
		mHeight = height;
		mAspectRatio = float(mWidth) / float(mHeight);

		ShowWindow(hwnd, SW_SHOWNORMAL);
		RECT rect;
		RECT newrect = { 0, 0, (LONG)mWidth, (LONG)mHeight };
		DWORD style = WS_POPUP | WS_SYSMENU;

		if (mWindowed)
		{
			GetWindowRect(hwnd, &mRect);
			mStyle = GetWindowLong(hwnd, GWL_STYLE);
		}

		if (windowFrame)
		{
			rect = mRect;
			style = mStyle;
		}
		else
		{
			HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi;
			mi.cbSize = sizeof(mi);
			GetMonitorInfo(monitor, &mi);
			newrect = mi.rcMonitor;
			rect = mi.rcMonitor;
			mAspectRatio = float(newrect.right - newrect.left) / float(newrect.bottom - newrect.top);
		}

		SetWindowLong(hwnd, GWL_STYLE, style);
		uint32_t prewidth = newrect.right - newrect.left;
		uint32_t preheight = newrect.bottom - newrect.top;
		AdjustWindowRect(&newrect, style, FALSE);
		mWindowWidth = (newrect.right - newrect.left) - prewidth;
		mWindowHeight = (newrect.bottom - newrect.top) - preheight;
		UpdateWindow(hwnd);

		if (rect.left == -32000 || rect.top == -32000)
		{
			rect.left = 0;
			rect.top = 0;
		}

		int32_t newLeft = rect.left;
		int32_t newTop = rect.top;
		int32_t newWidth = (newrect.right - newrect.left);
		int32_t newHeight = (newrect.bottom - newrect.top);

		if (!windowFrame)
		{
			float aspectRatio = 1.0f / mAspectRatio;
			newWidth = width;// bx::uint32_max(ENTRY_DEFAULT_WIDTH / 4, width);
			newHeight = uint32_t(float(newWidth)*aspectRatio);

			newLeft = newrect.left + (newrect.right - newrect.left - newWidth) / 2;
			newTop = newrect.top + (newrect.bottom - newrect.top - newHeight) / 2;
		}

		SetWindowPos(hwnd, HWND_TOP, newLeft, newTop, newWidth, newHeight, SWP_SHOWWINDOW);

		ShowWindow(hwnd, SW_RESTORE);

		mWindowed = windowFrame;
		if (mWindowed)
		{
			GetWindowRect(hwnd, &mRect);
		}
	}

	void ClearWindow(HWND hwnd)
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		HDC hdc = GetDC(hwnd);
		SelectObject(hdc, brush);
		FillRect(hdc, &rect, brush);
	}

	dot::core::WindowHandle FindHandle(HWND hwnd)
	{
		dot::core::MutexScope scope(mLock);
		//for (uint16_t ii = 0, num = m_windowAlloc.getNumHandles(); ii < num; ++ii)
		//{
		//	uint16_t idx = m_windowAlloc.getHandleAt(ii);
		//	if (_hwnd == m_hwnd[idx])
		//	{
		//		WindowHandle handle = { idx };
		//		return handle;
		//	}
		//}

		WindowHandle invalid = { 0/*UINT16_MAX*/ };
		return invalid;
	}

	AppContext *mAppContext;

	Mutex mLock;
	EventQueue mEventQueue;

	HWND mHwnd;
	RECT mRect;
	DWORD mStyle;
	uint32_t mWindowWidth;
	uint32_t mWindowHeight;

	uint32_t mWidth;
	uint32_t mHeight;
	float mAspectRatio;

	bool mInit;
	bool mExit;
	bool mWindowed;

	bool mExitRequest;

};

//=====================================================================================================================

Win32AppContext sPlatformContext;

LRESULT CALLBACK WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam)
{
	return sPlatformContext.ProcessMessages(hwnd, id, wparam, lparam);
}

int32_t MainThreadContext::ThreadFunc(void* userData)
{
	MainThreadContext* self = (MainThreadContext*)userData;
	int32_t result = dot::core::MainProxy(self->mArgc, self->mArgv, &sPlatformContext);
	PostMessage(sPlatformContext.mHwnd, WM_QUIT, 0, 0);
	return result;
}

//=====================================================================================================================

int main(int argc, char** argv)
{
	return sPlatformContext.Run(argc, argv);
}

#endif // DOT_PLATFORM_WINDOWS