//-----------------------------------------------------------------------------------
/// System.cpp ϵͳ���ܣ�����������Ⱦ����ʹӽű��������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Engine.h"

#include "System.h"
#include "Image.h"
#include "EmdMesh.h"
#include "Log.h"
#include "FileSystem.h"
#include "EString.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Font.h"
#include "SkeletonManager.h"
#include "Renderer.h"
#include "AudioManager.h"

#include <algorithm>
#include <fstream>

using namespace std;

namespace Gen
{
#if defined __PLATFORM_LINUX
	using namespace X11;
	Display*	g_Display = 0;		// ��Platformʹ�õ�X11 Display
#endif	// #if defined __PLATFORM_LINUX

	System::System()
		: m_FullScreen(false)
	{
		//m_RenderWindowParam.handle = 0;
		m_RenderWindowParam.width = 0;
		m_RenderWindowParam.height = 0;
		m_RenderWindowParam.colorDepthBit = 0;

		Math::InitRandom();

		// TODO: ע�������չ����ȡ����

		// ͼ���ļ�
		RegisterExtensionManager(".bmp", &TextureManager::Instance());
		RegisterExtensionManager(".tga", &TextureManager::Instance());
		RegisterExtensionManager(".jpg", &TextureManager::Instance());
		RegisterExtensionManager(".png", &TextureManager::Instance());

		// ���������
		RegisterExtensionManager(".ect", &TextureManager::Instance());
		// To be more...

		// ����
		RegisterExtensionManager(".emt", &MaterialManager::Instance());

		// ����
		RegisterExtensionManager(".ttf", &FontManager::Instance());
		RegisterExtensionManager(".ttc", &FontManager::Instance());

		// ģ��
		RegisterExtensionManager(".emd", &MeshManager::Instance());
		RegisterExtensionManager(".ms3d", &MeshManager::Instance());

		// ��Ƶ
		RegisterExtensionManager(".wav", &AudioManager::Instance());
		RegisterExtensionManager(".ogg", &AudioManager::Instance());

		// ����
		RegisterExtensionManager(".ebh", &SkeletonManager::Instance());
	}

	void System::LoadResources(const String& filename)
	{
		Log.MsgLn("Loading resources");
		ConfigGroups group;

		LoadConfigFile(filename, group);

		if (group.find("path") != group.end())
		{
			ConfigFileKeys::iterator iter;

			for (iter=group["path"].begin();
				iter!=group["path"].end();
				iter++)
			{
				if (iter->key=="data")
				{
					FileSystem::Instance().SetRelativeDataPath(iter->value);
				}
			}
		}

		// ��data·�����������ļ�����(������Ŀ¼)
		vector<ResourceFileNameInfo> list;
		String dataPath = FileSystem::Instance().GetValidResourcePath(FileSystem::Instance().GetDataPath());
		FileSystem::Instance().FindFilesInDir(dataPath, list, true);

		// ������չ����ʹ�ö�Ӧ�Ķ�ȡ����������Դ
		for (vector<ResourceFileNameInfo>::iterator iter=list.begin();
			iter!=list.end();
			iter++)
		{
			// ����Ѿ�ע��������չ�������ö�Ӧ����������Դ
			if (m_ExtManagers.find(iter->ext)!=m_ExtManagers.end())
			{
				String msg;
				msg.Format("Loading resource %s.", iter->filename.Data());
				Log.MsgLn(msg);

				// �����ļ������룬ʹ���ļ�����$data·�������·����Ϊ��Դ��������
				// ����
				//    $data\texture\glow.bmp
				//    ���������Ӧ��Ϊ��texture\glow.bmp

				if (!m_ExtManagers[iter->ext]->CreateResourceHandles(&(*iter)))
				{
					msg.Format("FAILED loading resource %s.", iter->filename.Data());
					Log.Error(msg);
				}
			}
		}
/*
		// ����ָ��·������Դ������resources�鵱��
		// ������е��ļ���Ҫָ��������·��
		ConfigGroups::iterator group_iter;
		if ((group_iter=group.find("resources"))!=group.end())
		{
			// ��������������Ŀ
			ConfigFileKeys::iterator iter;
			for (iter=group_iter->second.begin();
				iter!=group_iter->second.end();
				iter++)
			{
				// ��ȡ��չ����������ȡ����
				String ext = FileSystem::Instance().GetExtension(iter->value);
				ext.ToLowerCase();

				// ����б���뷴б��
				CorrectSlash(iter->key);
				CorrectSlash(iter->value);

				if (m_FileLoaders.find(ext)!=m_FileLoaders.end())
				{
					String msg;
					msg.Format("Loading resource %s.", iter->value.Data());
					Log.MsgLn(msg);

					if (!(*m_FileLoaders[ext])(iter->key, iter->value))
					{
						msg.Format("FAILED loading resource %s.", iter->value.Data());
						Log.Error(msg);
					}
				}
			}
		}
		*/
	}

	// ������Ⱦ����
	// ע��������������ڵ��ó����ڲ�ʹ�ã�����Platform::WndProc����dll�ļ��ض����¶������

	bool System::CreateRenderWindow(const String& title, unsigned int width, unsigned int height, unsigned int bits, bool fullscreen)
	{
		m_TitleName = title;

		m_RenderWindowParam.width = width;
		m_RenderWindowParam.height = height;

		m_RenderWindowParam.colorDepthBit = bits;

		m_FullScreen = fullscreen;

		Log.OutputTime();
		Log << "Creating render window: width = " << width <<
			", height = " << height <<
			", color depth = " << bits <<
			", fullscreen = " << fullscreen << "\n";


#if defined __PLATFORM_WIN32
		WNDCLASS wc;

		DWORD	dwExStyle;
		DWORD	dwStyle;

		RECT WindowRect;
		WindowRect.left = (long)0;
		WindowRect.right = (long)width;
		WindowRect.top = (long)0;
		WindowRect.bottom = (long)height;

		HINSTANCE hInstance = GetModuleHandle(NULL);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC) Platform::WndProc;	
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = title.Data();

		AssertFatal(RegisterClass(&wc), "System::CreateRenderWindow() : Failed to register window class.");

		if (fullscreen)
		{
			DEVMODE dmScreenSettings;
			memset (&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = width;
			dmScreenSettings.dmPelsHeight = height;
			dmScreenSettings.dmBitsPerPel = bits;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH |DM_PELSHEIGHT;

			AssertFatal(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN)==DISP_CHANGE_SUCCESSFUL,
				"System::CreateRenderWindow() : Unable to use selected display mode.");
		}

		if (fullscreen)
		{
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
			ShowCursor(false);
		}
		else
		{
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
		}

		AdjustWindowRectEx (&WindowRect, dwStyle, false, dwExStyle);

		m_RenderWindowParam.handle =
			CreateWindowEx(	dwExStyle,
							title.Data(),
							title.Data(),
							dwStyle |
							WS_CLIPSIBLINGS |
							WS_CLIPCHILDREN,
							(GetSystemMetrics(SM_CXSCREEN) - WindowRect.right) / 2,
							(GetSystemMetrics(SM_CYSCREEN) - WindowRect.bottom) / 2,
							WindowRect.right - WindowRect.left,
							WindowRect.bottom - WindowRect.top,
							NULL,
							NULL,
							hInstance,
							NULL);

		AssertFatal(m_RenderWindowParam.handle, "System::CreateRenderWindow(): Error creating render window.");

		ShowWindow(m_RenderWindowParam.handle,SW_SHOW);						// Show The Window
		SetForegroundWindow(m_RenderWindowParam.handle);						// Slightly Higher Priority
		SetFocus(m_RenderWindowParam.handle);									// Sets Keyboard Focus To The Window

#elif defined __PLATFORM_LINUX
		// ������X Server�����ӣ���ȡDisplay
		RenderWindowHandle* handle = &m_RenderWindowParam.handle;
		handle->display = g_Display = XOpenDisplay(0);
		Display* display = static_cast<Display*>(handle->display);
		handle->screen = DefaultScreen(display);

		Window rootwin = XDefaultRootWindow(display);
		Visual* visual = XDefaultVisualOfScreen(XDefaultScreenOfDisplay(display));

		unsigned int screen_width = DisplayWidth(display, handle->screen);
		unsigned int screen_height = DisplayHeight(display, handle->screen);

		// ����X����
		XSetWindowAttributes attrib;
		attrib.colormap = XCreateColormap(display, rootwin, visual, AllocNone);
		attrib.border_pixel = 0;
		attrib.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask; 
		handle->window = XCreateWindow(display,
			rootwin,
			(screen_width - width) / 2,
			(screen_height - height) / 2,
			width, height, 1,
			XDefaultDepth(display, 0),
			InputOutput,
			visual,
			CWBorderPixel | CWColormap | CWEventMask,
			&attrib);

		XStoreName(display, handle->window, title.Data());
		XSizeHints* hint;
		hint = XAllocSizeHints();
		hint->x = (screen_width - width) / 2;
		hint->y = (screen_height - height) / 2;
		hint->width = width;
		hint->height = height;
		hint->flags = USPosition | USSize;
		XSetNormalHints(display, handle->window, hint);
		XFree(hint);

		// ��ʾ����
		XMapWindow(display, handle->window);

		// ˢ�»���
		XFlush(display);

#endif	// #if defined __PLATFORM_WIN32

		return true;
	}

	void System::DestroyRenderWindow()
	{
#if defined __PLATFORM_WIN32
		if (m_RenderWindowParam.handle)
		{
			AssertFatal(DestroyWindow(m_RenderWindowParam.handle), "System::DestroyRenderWindow() : Could Not Release hWnd.");
			m_RenderWindowParam.handle = NULL;
		}
		if (m_FullScreen)											// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL,0);						// If So Switch Back To The Desktop
			ShowCursor(TRUE);									// Show Mouse Pointer
		}

		AssertFatal(UnregisterClass(m_TitleName.Data(), GetModuleHandle(NULL)), "System::DestroyRenderWindow(): Could not unregister class.");

#elif defined __PLATFORM_LINUX

		Display* display = static_cast<Display*>(m_RenderWindowParam.handle.display);
		// �رմ���
		XDestroyWindow(display, m_RenderWindowParam.handle.window);

		XFlush(display);

		// �ر�display
		XCloseDisplay(display);
#endif	// #if defined __PLATFORM_WIN32
		Log.MsgLn("Render window successfully destroyed.");
	}

	void System::SetWindowTitle(const String& title)
	{
#if defined __PLATFORM_WIN32
		//m_TitleName = title;
		SetWindowText(m_RenderWindowParam.handle, title.Data());
#endif	// #if defined __PLATFORM_WIN32
	}

	void System::ResizeWindow(unsigned int width, unsigned int height)
	{
		m_RenderWindowParam.width = width;
		m_RenderWindowParam.height = height;
	}

	void System::ToggleMouseCursor(bool toggle)
	{
#if defined __PLATFORM_WIN32
		ShowCursor(toggle);
#elif defined __PLATFORM_LINUX
#endif	// #if defined __PLATFORM_WIN32
	}

	void System::SetMouseCursorPos(int x, int y)
	{
#if defined __PLATFORM_WIN32
		SetCursorPos(x, y);
#elif defined __PLATFORM_LINUX
#endif	// #if defined __PLATFORM_WIN32
	}

	void System::CenterMouseCursor()
	{
#if defined __PLATFORM_WIN32
		if (Renderer::Instance().GetActive())
		{

			RECT rect;
			GetWindowRect(m_RenderWindowParam.handle, &rect);

			SetCursorPos(
				(rect.right - rect.left) / 2 + rect.left,
				(rect.bottom - rect.top) / 2 + rect.top);
		}
#elif defined __PLATFORM_LINUX
#endif	// #if defined __PLATFORM_WIN32
	}

	void System::RegisterExtensionManager(const String& ext, ResourceManagerBase* manager)
	{
		AssertFatal(m_ExtManagers.find(ext)==m_ExtManagers.end(),
			"System::RegisterExtensionManager(): Extension has been already registered.");
		AssertFatal(manager, "System::RegisterExtensionManager(): Manager cannot be null.");
		m_ExtManagers[ext] = manager;
	}

}
