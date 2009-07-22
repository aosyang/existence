//-----------------------------------------------------------------------------------
/// System.cpp 系统功能，包括创建渲染窗体和从脚本载入材质
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Engine.h"

#include "System.h"
#include "Image.h"
#include "Mesh.h"
#include "Log.h"

#include <fstream>
#include "EString.h"

using namespace std;

System::System()
: m_FullScreen(false)
{
	m_RenderWindowParam.handle = 0;
	m_RenderWindowParam.width = 0;
	m_RenderWindowParam.height = 0;
	m_RenderWindowParam.color_depth_bit = 0;

	InitRandom();
}

void System::LoadResources(const String& filename)
{
	Log.MsgLn("Loading resources");
	ConfigGroups group;
	LoadConfigFile(filename, group);

	if (group.find("audios") != group.end())
		LoadAudios(&group["audios"]);

	if (group.find("textures") != group.end())
		LoadTextures(&group["textures"]);

	if (group.find("materials") != group.end())
		LoadMaterials(&group["materials"]);

	if (group.find("fonts") != group.end())
		LoadFonts(&group["fonts"]);

	if (group.find("meshes") != group.end())
		LoadMeshes(&group["meshes"]);
}

void System::LoadAudios(ConfigFileKeys* list)
{
	ConfigFileKeys::iterator iter;

	for (iter=list->begin(); iter!=list->end(); iter++)
	{
		String msg;
		if (Engine::Instance().AudioSystem()->LoadAudioBufferFromFile(iter->key, iter->value))
		{
			msg.Format("Loading audio %s... OK.", iter->value.Data());
			Log.MsgLn(msg);
		}
		else
		{
			msg.Format("Loading audio %s FAILED.", iter->value.Data());
			Log.Error(msg);
		}
	}
}

void System::LoadTextures(ConfigFileKeys* list)
{
	ConfigFileKeys::iterator iter;

	for (iter=list->begin(); iter!=list->end(); iter++)
	{
		Image* image = new Image();

		String msg;
		if (image->LoadFromFile(iter->value))
		{
			renderer->BuildTexture(iter->key,
														image->GetWidth(),
														image->GetHeight(),
														image->GetBPP(),
														image->GetData());

			msg.Format("Loading texture %s... OK.", iter->value.Data());
			Log.MsgLn(msg);
		}
		else
		{
			msg.Format("Loading texture %s FAILED.", iter->value.Data());
			Log.Error(msg);
		}

		delete image;
	}

}

void System::LoadMaterials(ConfigFileKeys* list)
{
	ConfigFileKeys::iterator iter;

	for (iter=list->begin(); iter!=list->end(); iter++)
	{
		String msg;
		if (ResourceManager<Material>::Instance().LoadResource(iter->key, iter->value))
		{
			msg.Format("Loading material %s... OK.", iter->value.Data());
			Log.MsgLn(msg);
		}
		else
		{
			msg.Format("Loading material %s FAILED.", iter->value.Data());
			Log.Error(msg);
		}
	}
}

void System::LoadFonts(ConfigFileKeys* list)
{
	ConfigFileKeys::iterator iter;

	for (iter=list->begin(); iter!=list->end(); iter++)
	{
		String msg;
		if (FontManager::Instance().LoadFont(iter->key, iter->value))
		{
			msg.Format("Loading font %s... OK.", iter->value.Data());
			Log.MsgLn(msg);
		}
		else
		{
			msg.Format("Loading font %s FAILED.", iter->value.Data());
			Log.Error(msg);
		}
	}
}

void System::LoadMeshes(ConfigFileKeys* list)
{
	ConfigFileKeys::iterator iter;

	for (iter=list->begin(); iter!=list->end(); iter++)
	{
		String msg;
		if (ResourceManager<Mesh>::Instance().LoadResource(iter->key, iter->value))
		{
			msg.Format("Loading mesh %s... OK.", iter->value.Data());
			Log.MsgLn(msg);
		}
		else
		{
			msg.Format("Loading mesh %s FAILED.", iter->value.Data());
			Log.Error(msg);
		}
	}
}


// 创建渲染窗体
// 注：这个方法必须在调用程序内部使用，否则Platform::WndProc会因dll的加载而导致多个副本

bool System::CreateRenderWindow(const String& title, unsigned int width, unsigned int height, unsigned int bits, bool fullscreen)
{
	WNDCLASS wc;

	DWORD	dwExStyle;
	DWORD	dwStyle;

	m_TitleName = title;

	//m_WindowWidth = width;
	//m_WindowHeight = height;

	//m_Bits = bits;

	m_RenderWindowParam.width = width;
	m_RenderWindowParam.height = height;

	m_RenderWindowParam.color_depth_bit = bits;

	m_FullScreen = fullscreen;

	Log.OutputTime();
	Log << "Creating render window: width = " << width << ", height = " << height << ", color depth = " << bits << ", fullscreen = " << fullscreen << "\n";

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

	AssertFatal(RegisterClass(&wc), "System::CreateRenderWindow() : Failed to register window class.")

	if (fullscreen)
	{
		DEVMODE dmScreenSettings;
		memset (&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH |DM_PELSHEIGHT;

		AssertFatal(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN)==DISP_CHANGE_SUCCESSFUL, "System::CreateRenderWindow() : Unable to use selected display mode.");
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

	if (!(m_RenderWindowParam.handle = CreateWindowEx(	dwExStyle,
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
		NULL)))
	{
		MessageBox( NULL, "Create Error", "ERROR", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	ShowWindow(m_RenderWindowParam.handle,SW_SHOW);						// Show The Window
	SetForegroundWindow(m_RenderWindowParam.handle);						// Slightly Higher Priority
	SetFocus(m_RenderWindowParam.handle);									// Sets Keyboard Focus To The Window

	return true;
}

void System::DestroyRenderWindow()
{
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

	if (!UnregisterClass(m_TitleName.Data(), GetModuleHandle(NULL)))				// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		//hInstance=NULL;										// Set hInstance To NULL
	}

	Log.MsgLn("Render window successfully destroyed.");
}

void System::SetWindowTitle(const String& title)
{
	//m_TitleName = title;
	SetWindowText(m_RenderWindowParam.handle, title.Data());
}

void System::ResizeWindow(unsigned int width, unsigned int height)
{
	m_RenderWindowParam.width = width;
	m_RenderWindowParam.height = height;
}

void System::ToggleMouseCursor(bool toggle)
{
	ShowCursor(toggle);
}

void System::SetMouseCursorPos(int x, int y)
{
	SetCursorPos(x, y);
}

void System::CenterMouseCursor()
{
	if (renderer->GetActive())
	{
		
		RECT rect;
		GetWindowRect(m_RenderWindowParam.handle, &rect);

		SetCursorPos(
			(rect.right - rect.left) / 2 + rect.left,
			(rect.bottom - rect.top) / 2 + rect.top);
	}
}
