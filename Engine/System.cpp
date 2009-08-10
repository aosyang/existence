//-----------------------------------------------------------------------------------
/// System.cpp 系统功能，包括创建渲染窗体和从脚本载入材质
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Engine.h"

#ifdef __PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#endif	// #ifdef __PLATFORM_LINUX

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

	Math::InitRandom();
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
#if defined __PLATFORM_WIN32
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
	
#elif defined __PLATFORM_LINUX

    XVisualInfo *vi;
    Colormap cmap;
    int dpyWidth, dpyHeight;
    int i;
    int glxMajorVersion, glxMinorVersion;
    int vidModeMajorVersion, vidModeMinorVersion;
    XF86VidModeModeInfo **modes;
    int modeNum;
    int bestMode;
    Atom wmDelete;
    Window winDummy;
    unsigned int borderDummy;
    
    GLWin.fs = fullscreenflag;
    /* set best mode to current */
    bestMode = 0;
    /* get a connection */
    GLWin.dpy = XOpenDisplay(0);
    GLWin.screen = DefaultScreen(GLWin.dpy);
    XF86VidModeQueryVersion(GLWin.dpy, &vidModeMajorVersion,
        &vidModeMinorVersion);
    printf("XF86VidModeExtension-Version %d.%d\n", vidModeMajorVersion,
        vidModeMinorVersion);
    XF86VidModeGetAllModeLines(GLWin.dpy, GLWin.screen, &modeNum, &modes);
    /* save desktop-resolution before switching modes */
    GLWin.deskMode = *modes[0];
    /* look for mode with requested resolution */
    for (i = 0; i < modeNum; i++)
    {
        if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
        {
            bestMode = i;
        }
    }
    /* get an appropriate visual */
    vi = glXChooseVisual(GLWin.dpy, GLWin.screen, attrListDbl);
    if (vi == NULL)
    {
        vi = glXChooseVisual(GLWin.dpy, GLWin.screen, attrListSgl);
        GLWin.doubleBuffered = False;
        printf("Only Singlebuffered Visual!\n");
    }
    else
    {
        GLWin.doubleBuffered = True;
        printf("Got Doublebuffered Visual!\n");
    }
    glXQueryVersion(GLWin.dpy, &glxMajorVersion, &glxMinorVersion);
    printf("glX-Version %d.%d\n", glxMajorVersion, glxMinorVersion);
    /* create a GLX context */
    GLWin.ctx = glXCreateContext(GLWin.dpy, vi, 0, GL_TRUE);
    /* create a color map */
    cmap = XCreateColormap(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
        vi->visual, AllocNone);
    GLWin.attr.colormap = cmap;
    GLWin.attr.border_pixel = 0;

    if (GLWin.fs)
    {
        XF86VidModeSwitchToMode(GLWin.dpy, GLWin.screen, modes[bestMode]);
        XF86VidModeSetViewPort(GLWin.dpy, GLWin.screen, 0, 0);
        dpyWidth = modes[bestMode]->hdisplay;
        dpyHeight = modes[bestMode]->vdisplay;
        printf("Resolution %dx%d\n", dpyWidth, dpyHeight);
        XFree(modes);
    
        /* create a fullscreen window */
        GLWin.attr.override_redirect = True;
        GLWin.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
            StructureNotifyMask;
        GLWin.win = XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
            0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
            CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
            &GLWin.attr);
        XWarpPointer(GLWin.dpy, None, GLWin.win, 0, 0, 0, 0, 0, 0);
		XMapRaised(GLWin.dpy, GLWin.win);
        XGrabKeyboard(GLWin.dpy, GLWin.win, True, GrabModeAsync,
            GrabModeAsync, CurrentTime);
        XGrabPointer(GLWin.dpy, GLWin.win, True, ButtonPressMask,
            GrabModeAsync, GrabModeAsync, GLWin.win, None, CurrentTime);
    }
    else
    {
        /* create a window in window mode*/
        GLWin.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
            StructureNotifyMask;
        GLWin.win = XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
            0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
            CWBorderPixel | CWColormap | CWEventMask, &GLWin.attr);
        /* only set window title and handle wm_delete_events if in windowed mode */
        wmDelete = XInternAtom(GLWin.dpy, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(GLWin.dpy, GLWin.win, &wmDelete, 1);
        XSetStandardProperties(GLWin.dpy, GLWin.win, title,
            title, None, NULL, 0, NULL);
        XMapRaised(GLWin.dpy, GLWin.win);
    }       
    /* connect the glx-context to the window */
    glXMakeCurrent(GLWin.dpy, GLWin.win, GLWin.ctx);
    XGetGeometry(GLWin.dpy, GLWin.win, &winDummy, &GLWin.x, &GLWin.y,
        &GLWin.width, &GLWin.height, &borderDummy, &GLWin.depth);
    printf("Depth %d\n", GLWin.depth);
    if (glXIsDirect(GLWin.dpy, GLWin.ctx)) 
        printf("Congrats, you have Direct Rendering!\n");
    else
        printf("Sorry, no Direct Rendering possible!\n");
#endif	// #if defined __PLATFORM_WIN32

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
	if (renderer->GetActive())
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
