//-----------------------------------------------------------------------------------
/// Platform.cpp 平台相关代码功能实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Platform.h"

namespace Gen
{
	PlatformMessageNotifier* Platform::m_MessageNotifier = (PlatformMessageNotifier *)0x00000001;

#if defined __PLATFORM_LINUX
	namespace X11	// 防止X11的类型名与FreeType冲突
	{
		#include <X11/Xlib.h>
	}

	extern X11::Display*	g_Display;

#endif	// #if defined __PLATFORM_LINUX

	void Platform::HandleWindowMessage()
	{
#ifdef __PLATFORM_WIN32
		MSG msg;

		if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				//Engine::Instance().SetQuitting(true);
				if (m_MessageNotifier)
					m_MessageNotifier->OnMessageQuit();
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
#elif defined __PLATFORM_LINUX
		if (XPending(g_Display) > 0)
		{
			X11::XEvent event;

			XNextEvent(g_Display, &event);

			switch (event.type)
			{
			case ConfigureNotify:
				break;
			case DestroyNotify:
				break;
			}
		}
#endif
	}

#ifdef __PLATFORM_WIN32
	WNDPROC CALLBACK Platform::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)												// Check For Windows Messages
		{
		case WM_ACTIVATE:										// Watch For Window Activate Message
			{
				//if (!HIWORD(wParam))								// Check Minimization State
				//{
				//	Renderer::Instance().SetActive(true);									// Program Is Active
				//}
				//else												// Otherwise
				//{
				//	Renderer::Instance().SetActive(false);									// Program Is No Longer Active

				//}
				if (m_MessageNotifier)
					//m_MessageNotifier->OnMessageActiveWindow(!HIWORD(wParam));
					m_MessageNotifier->OnMessageActiveWindow(wParam);

				return 0;											// Return To The Message Loop
			}

		case WM_SYSCOMMAND:										// Intercept System Commands
			{
				switch (wParam)										// Check System Calls
				{
				case SC_SCREENSAVE:								// Screensaver Trying To Start?
				case SC_MONITORPOWER:							// Monitor Trying To Enter Powersave?
					return 0;										// Prevent From Happening
				}
				break;												// Exit
			}

		case WM_CLOSE:											// Did We Receive A Close Message?
			{
				PostQuitMessage(0);									// Send A Quit Message
				return 0;											// Jump Back
			}

		case WM_SIZE:											// Resize The OpenGL Window
			{
				//Renderer::Instance().ResizeRenderWindow(LOWORD(lParam),HIWORD(lParam));
				if (m_MessageNotifier)
					m_MessageNotifier->OnMessageResizeWindow(LOWORD(lParam), HIWORD(lParam));
				return 0;											// Jump Back
			}
		}

		return (WNDPROC)DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
#endif
}


