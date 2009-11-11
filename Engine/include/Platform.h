//-----------------------------------------------------------------------------------
/// Platform.h 实现平台相关代码功能
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _PLATFORM_H
#define _PLATFORM_H

#if defined _WIN32
#define __PLATFORM_WIN32
#include <windows.h>
#elif defined _LINUX
#define __PLATFORM_LINUX
#else
#error Unsupported platform
#endif

namespace Gen
{
#ifdef __PLATFORM_WIN32
	typedef HWND RenderWindowHandle;

#define DLLEXPORT __declspec(dllexport)

#elif defined __PLATFORM_LINUX

	typedef struct
	{
		void*              display;
		int         	   screen;
		unsigned long      window;
	} RenderWindowHandle;

#define DLLEXPORT __attribute__ ((visibility("default")))

#endif

	struct RenderWindowParam
	{
		RenderWindowHandle	handle;
		unsigned int		width, height;
		unsigned int		colorDepthBit;
	};

	class PlatformMessageNotifier;

	class Platform
	{
	public:
		static void SetMessageNotifier(PlatformMessageNotifier* notifier) { m_MessageNotifier = notifier; }

		static void HandleWindowMessage();

#ifdef __PLATFORM_WIN32
		// Win32消息回调函数
		static WNDPROC CALLBACK WndProc(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM	lParam);
#endif

		static PlatformMessageNotifier* m_MessageNotifier;
	};

	class PlatformMessageNotifier
	{
	public:
		virtual ~PlatformMessageNotifier() {}

		virtual void OnMessageQuit() = 0;

		virtual void OnMessageActiveWindow(bool active) = 0;

		virtual void OnMessageResizeWindow(unsigned int width, unsigned int height) = 0;
	};

#define SAFE_DELETE(x)			if (x) { delete x; x = 0; }
#define SAFE_DELETE_ARRAY(x)	if (x) { delete [] x; x = 0; }
}

#endif

