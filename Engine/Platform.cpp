//-----------------------------------------------------------------------------------
/// Platform.cpp 平台相关代码功能实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Platform.h"

PlatformMessageNotifier* Platform::m_MessageNotifier = (PlatformMessageNotifier *)0x00000001;

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
			//	renderer->SetActive(true);									// Program Is Active
			//}
			//else												// Otherwise
			//{
			//	renderer->SetActive(false);									// Program Is No Longer Active

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
			//renderer->ResizeRenderWindow(LOWORD(lParam),HIWORD(lParam));
			if (m_MessageNotifier)
				m_MessageNotifier->OnMessageResizeWindow(LOWORD(lParam), HIWORD(lParam));
			return 0;											// Jump Back
		}
	}

	return (WNDPROC)DefWindowProc(hWnd, uMsg, wParam, lParam);
}
#endif

