//-----------------------------------------------------------------------------------
/// GLRenderer.cpp OpenGL渲染器实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "GLRenderer.h"

//#include <gl/gl.h>
#include <GL/glew.h>
#include <gl/glu.h>

#include <string>

#include "FontPrint.h"

//GLRenderer* Singleton<GLRenderer>::mSingleton = 0;

GLRenderer::GLRenderer()
: m_hWnd(0), m_hDC(0), m_hRC(0),
  m_WindowWidth(0), m_WindowHeight(0), m_Bits(0), m_FullScreen(false),
  m_Active(false)
{
	m_ViewMatrix.Identity();
}

GLRenderer::~GLRenderer()
{
}

bool GLRenderer::CreateRenderWindow(char* title, unsigned int width, unsigned int height, unsigned int bits, bool fullscreen)
{
	WNDCLASS wc;

	DWORD	dwExStyle;
	DWORD	dwStyle;

	strcpy(m_TitleName, title);

	m_WindowWidth = width;
	m_WindowHeight = height;

	m_Bits = bits;

	m_FullScreen = fullscreen;

	RECT WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;

	fullscreen = fullscreen;

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
	wc.lpszClassName = title;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL,"Register","ERROR", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (fullscreen)
	{
		DEVMODE dmScreenSettings;
		memset (&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPanningHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH |DM_PELSHEIGHT;
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

	if (!(m_hWnd = CreateWindowEx(	dwExStyle,
		title, 
		title, 
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

	return Initialize();
}

void GLRenderer::DestroyRenderWindow()
{
	if (m_FullScreen)											// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);						// If So Switch Back To The Desktop
		ShowCursor(TRUE);									// Show Mouse Pointer
	}

	if (m_hRC)												// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))						// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(m_hRC))							// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		m_hRC=NULL;											// Set RC To NULL
	}

	if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))						// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hDC=NULL;											// Set DC To NULL
	}

	if (m_hWnd && !DestroyWindow(m_hWnd))						// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hWnd=NULL;											// Set hWnd To NULL
	}

	if (!UnregisterClass(m_TitleName, GetModuleHandle(NULL)))				// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		//hInstance=NULL;										// Set hInstance To NULL
	}

}

void GLRenderer::ResizeRenderWindow(unsigned int width, unsigned int height)
{
	if (width != 0 && height != 0)
	{
		m_WindowWidth = width;
		m_WindowHeight = height;
	}

	if (m_WindowHeight == 0) m_WindowHeight = 1;
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);	//Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)m_WindowWidth/(GLfloat)m_WindowHeight,0.1f,100.0f);
	//gluOrtho2D(0, m_WindowWidth, 0, m_WindowHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void GLRenderer::AddRenderObject(RenderObject *ro)
{
	m_RenderList.AddRenderObject(ro);
}

void GLRenderer::RemoveRenderObject(RenderObject* ro)
{
	m_RenderList.RemoveRenderObject(ro);
}

bool GLRenderer::Initialize()
{
	if (!m_hWnd)
		return false;

	//glewInit();

	if (!(m_hDC = GetDC(m_hWnd)))
	{
		DestroyRenderWindow();
		MessageBox(NULL, "lpText", "lpCaption", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	GLuint PixelFormat;

	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size
		1,												// Version Number
		PFD_DRAW_TO_WINDOW |							// dwFormat
		PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,								// Must Support Double Buffering
		PFD_TYPE_RGBA,									// Request An RGBA Format
		m_Bits,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,								// Color Bits Ignored
		0,												// No Alpha Buffer
		0,												// Shift Bit Ignored
		0,												// No Accumulation Buffer
		0, 0, 0, 0,										// Accumulation Bits Ignored
		16,												// 16Bit Z-Buffer (Depth Buffer)
		0,												// No Stencil Buffer
		0,												// No Auxiliary Buffer
		PFD_MAIN_PLANE,									// Main Drawing Layer
		0,												// Reserved
		0, 0, 0											// Layer Masks Ignored
	};

	if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		DestroyRenderWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if(!SetPixelFormat(m_hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		DestroyRenderWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if (!(m_hRC=wglCreateContext(m_hDC)))				// Are We Able To Get A Rendering Context?
	{
		DestroyRenderWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	if(!wglMakeCurrent(m_hDC, m_hRC))					// Try To Activate The Rendering Context
	{
		DestroyRenderWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return false
	}

	ShowWindow(m_hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(m_hWnd);						// Slightly Higher Priority
	SetFocus(m_hWnd);									// Sets Keyboard Focus To The Window

	ResizeRenderWindow();							// Set Up Our Perspective GL Screen

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);	
	glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
	glClearDepth(1.0f);										// Depth Buffer Setup

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					// Set Line Antialiasing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable(GL_BLEND);										// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// Type Of Blending To Use

	// 开启光照
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	return true;
}

bool GLRenderer::Render()
{
	// 渲染器在未激活状态下不进行渲染
	if (!m_Active)
		return false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();									// Reset The Current Modelview Matrix

	float viewMat[16];
	BuildGLMatrix(m_ViewMatrix, viewMat);
	//glMultMatrixf(viewMat);
	glLoadMatrixf(viewMat);

	// 灯光位置
	// 注：必须在每次更新了ViewMatrix之后也更新灯光位置，否则灯光位置就会相对视点固定

	float lightDir[] = { 1.0f, 1.0f, 1.0f, 0.0f };		// w = 0.0f 代表这是一个方向光
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);

	// 遍历渲染列表中的元素，进行渲染
	RenderListElementsIterator iter;
	for (iter = m_RenderList.m_RenderObjectList.begin();
		 iter != m_RenderList.m_RenderObjectList.end();
		 iter++)
	{
		// 视图矩阵已经由摄像机的属性决定
		// 渲染模型前需要保存这个矩阵，因为在渲染模型过程中会改变模型视图矩阵
		glPushMatrix();

		float mat[16];
		BuildGLMatrix((*iter)->GetTransformMatrix(), mat);

		glMultMatrixf(mat);

		//(*iter)->RenderSingleObject();

		unsigned int face_num = (*iter)->GetFaceNumber();
		if (face_num)
		{

			// 指定材质
			Material* mat = (*iter)->GetMaterial();
			if (mat)
			{
				//glColor4fv(mat->GetColor().GetArray());

				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat->GetAmbient().GetArray());
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->GetDiffuse().GetArray());
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->GetSpecular().GetArray());
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat->GetEmissive().GetArray());
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->GetSpecularLevel());

				if (ITexture* tex = mat->GetTexture())
				{
					BindTexture(tex);
				}
				else
				{
					BindTexture(TextureManager::Instance().GetTexture("no_material"));
				}
			}
			else
			{
				// 绑定默认材质
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				// TODO : 使用名称绑定材质的方法，添加给Renderer使用
				//TextureManager::Instance().BindTexture("no_material");

				ITexture* null_texture = TextureManager::Instance().GetTexture("no_material");
				AssertFatal(null_texture, "Unable to find 'no_material', define it in texture.cfg first.");

				BindTexture(null_texture);
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, (*iter)->GetVertexArray());

			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, (*iter)->GetNormalArray());

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, (*iter)->GetTextureCoordArray());

			glDrawElements(GL_TRIANGLES, face_num * 3, GL_UNSIGNED_INT, (*iter)->GetFaceArray());

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}


		// 渲染完恢复渲染前的矩阵状况
		glPopMatrix();
	}
	SwapBuffers(m_hDC);

	// 清除渲染列表
	//m_RenderList.ClearList();

	return true;
}

//-----------------------------------------------------------------------------------
/// \brief
/// 从Matrix4生成OpenGL使用的float[16]矩阵
/// 
/// \param mat
/// Matrix4矩阵
/// 
/// \param glMat
/// OpenGL矩阵
/// 
/// 这个方法从Matrix4创建一个float[16]的矩阵数组供OpenGL函数使用
/// 
/// \remarks
/// OpenGL采用Column-major矩阵，顺序如下:<br>
/// <br>
/// [0  4  8  12]<br>
/// |1  5  9  13|<br>
/// |2  6  10 14|<br>
/// [3  7  11 15]<br>
/// 
//-----------------------------------------------------------------------------------
void GLRenderer::BuildGLMatrix(const Matrix4& mat, float* glMat)
{
	int m = 0;
	for (int y=0; y<4; y++)
		for (int x=0; x<4; x++)
		{
			glMat[m] = mat.m[x][y];
			m++;
		}
}

void GLRenderer::BindTexture(ITexture* texture)
{
	GLTexture* glTex = static_cast<GLTexture*>(texture);
	glBindTexture(GL_TEXTURE_2D, glTex->GetGLTextureID());
}
