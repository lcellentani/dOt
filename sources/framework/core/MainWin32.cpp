#include "core/MainProxy.h"
#include "core/AppContext.h"
#include "core/PlatformContext.h"
#include "core/Application.h"

#include "core/PlatformHandles.h"
#include "core/PlatformEvent.h"

#include "core/Thread.h"

#include <stdint.h> // uint32_t
#include <windowsx.h>

#if DOT_PLATFORM_WINDOWS

using namespace dot::core;

static WindowHandle sDefaultWindow;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam);

struct MainThreadContext
{
	int mArgc;
	char **mArgv;

	dot::core::PlatformContext *platformContext;

	static int32_t ThreadFunc(void *userData);
};

class Win32StopWatch : public StopWatch
{
public:
	//! Constructor, default
	Win32StopWatch() : StopWatch(), mStartTime(), mEndTime(), mDiffTime(0.0f), mFreq(0.0), mFreqSet(false)
	{
		if (!mFreqSet)
		{
			LARGE_INTEGER temp;

			QueryPerformanceFrequency((LARGE_INTEGER*)&temp);
			mFreq = ((double)temp.QuadPart);

			mFreqSet = true;
		}
	};

	virtual ~Win32StopWatch() {};

	virtual void Start() OVERRIDE
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&mStartTime);
		mRunning = true;
	}

	virtual void Stop() OVERRIDE
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&mEndTime);
		mDiffTime = (float)(((double)mEndTime.QuadPart - (double)mStartTime.QuadPart) / mFreq);
		mRunning = false;
	}

	virtual void Reset() OVERRIDE
	{
		mDiffTime = 0.0f;
		if (mRunning)
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&mStartTime);
		}
	}

	const float GetTime() const
	{
		if (mRunning)
		{
			return getDiffTime();
		}
		return mDiffTime;
	}

private:
	float getDiffTime() const
	{
		LARGE_INTEGER temp;
		QueryPerformanceCounter((LARGE_INTEGER*)&temp);
		return (float)(((double)temp.QuadPart - (double)mStartTime.QuadPart) / mFreq);
	}

	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mEndTime;
	float mDiffTime;

	double mFreq;
	bool mFreqSet;
};

class Win32AppContext : public PlatformContext
{
public:
	Win32AppContext() : PlatformContext()
	, mAppContext(nullptr)
	, mHwnd(0)
	, mRect()
	, mWindowWidth(0)
	, mWindowHeight(0)
	, mWidth(0)
	, mHeight(0)
	, mInit(false)
	, mExit(false)
	, mWindowed(true)
	{
	}

	virtual StopWatch * CreateStopWatch() OVERRIDE
	{
		return (StopWatch *) new Win32StopWatch();
	}

	int Run(int argc, char** argv)
	{
		mAppContext = CreateAppContext();

		mDebugMode = mAppContext->IsDebugModeEnabled();

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
		mNativeWindowHandle = (void *)mHwnd;

		ReshapeWindow(mHwnd, defaultWidth, defaultHeight, true);
		ClearWindow(mHwnd);

		MainThreadContext mtc;
		mtc.mArgc = argc;
		mtc.mArgv = argv;
		dot::core::Thread thread;
		thread.Start(mtc.ThreadFunc, &mtc);

		mInit = true;

		mEventQueue->PostSizeEvent(sDefaultWindow, mWidth, mHeight);
		mFrameWidth = mWidth;
		mFrameHeight = mHeight;

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
					mEventQueue->PostExitEvent();
				}
				return 0;

			/*case WM_SIZING:
			{
				RECT& rect = *(RECT*)lparam;
				uint32_t width = rect.right - rect.left - mFrameWidth;
				uint32_t height = rect.bottom - rect.top - mFrameHeight;

				// Recalculate size according to aspect ratio
				switch (wparam)
				{
				case WMSZ_LEFT:
				case WMSZ_RIGHT:
				{
					float aspectRatio = 1.0f / mAspectRatio;
					//width = bx::uint32_max(ENTRY_DEFAULT_WIDTH / 4, width);
					height = uint32_t(float(width)*aspectRatio);
				}
				break;

				default:
				{
					float aspectRatio = mAspectRatio;
					//height = bx::uint32_max(ENTRY_DEFAULT_HEIGHT / 4, height);
					width = uint32_t(float(height)*aspectRatio);
				}
				break;
				}

				// Recalculate position using different anchor points
				switch (wparam)
				{
				case WMSZ_LEFT:
				case WMSZ_TOPLEFT:
				case WMSZ_BOTTOMLEFT:
					rect.left = rect.right - width - mFrameWidth;
					rect.bottom = rect.top + height + mFrameHeight;
					break;

				default:
					rect.right = rect.left + width + mFrameWidth;
					rect.bottom = rect.top + height + mFrameHeight;
					break;
				}

				mEventQueue->PostSizeEvent(sDefaultWindow, width, height);
			}
			return 0;*/

			case WM_SIZE:
			{
				uint32_t width = GET_X_LPARAM(lparam);
				uint32_t height = GET_Y_LPARAM(lparam);

				mWidth = width;
				mHeight = height;
				mEventQueue->PostSizeEvent(sDefaultWindow, mWidth, mHeight);
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

	AppContext *mAppContext;

	Mutex mLock;

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