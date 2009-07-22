#include "CgGLRenderSystem.h"

#include "Debug.h"
#include "CgGLTexture.h"
#include "MathUtil.h"

#include "GL/glu.h"

#include "EString.h"

CgGLRenderSystem::CgGLRenderSystem()
: m_hWnd(0), m_hDC(0), m_hRC(0),
  m_WindowWidth(0), m_WindowHeight(0), // m_FullScreen(false),
  m_Active(true),
  m_Frustum(NULL)
{
	m_ViewMatrix.Identity();
}

CgGLRenderSystem::~CgGLRenderSystem()
{
	UnloadAllTextures();
}

RenderWindowHandle CgGLRenderSystem::GetRenderWindowHandle()
{
	return m_hWnd;
}

bool CgGLRenderSystem::Initialize(RenderWindowParam* windowParam)
{
	m_hWnd = windowParam->handle;

	AssertFatal(m_hDC = GetDC(m_hWnd), "CgGLRenderSystem::Initialize() : Failed to get device context.");

	int PixelFormat;

	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size
		1,												// Version Number
		PFD_DRAW_TO_WINDOW |							// dwFormat
		PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,								// Must Support Double Buffering
		PFD_TYPE_RGBA,									// Request An RGBA Format
		windowParam->color_depth_bit,					// Select Our Color Depth
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

	AssertFatal(PixelFormat = ChoosePixelFormat(m_hDC,&pfd), "CgGLRenderSystem::CreateRenderWindow() : Can't Find A Suitable PixelFormat.");
	AssertFatal(SetPixelFormat(m_hDC, PixelFormat, &pfd), "CgGLRenderSystem::CreateRenderWindow() : Can't Set The PixelFormat.");
	AssertFatal(m_hRC=wglCreateContext(m_hDC), "CgGLRenderSystem::CreateRenderWindow() : Can't Create A GL Rendering Context.");
	AssertFatal(wglMakeCurrent(m_hDC, m_hRC), "CgGLRenderSystem::CreateRenderWindow() : Can't Activate The GL Rendering Context.");

	// 注：glewInit必须放在wglMakeCurrent之后进行，否则无法成功初始化
	AssertFatal(GLEW_OK == glewInit(), "CgGLRenderSystem::Initialize() : Failed to init glew.");

	// 初始化CG

	// 创建Cg Context
	m_CgContext = cgCreateContext();
	CheckCgError();

	cgSetParameterSettingMode(m_CgContext, CG_DEFERRED_PARAMETER_SETTING);

	// 获得Vertex Profile
	m_CgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgGLSetOptimalOptions(m_CgVertexProfile);
	CheckCgError();

	// 获得Fragment Profile
	m_CgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(m_CgFragmentProfile);
	CheckCgError();



	ResizeRenderWindow(windowParam->width, windowParam->height);							// Set Up Our Perspective GL Screen

	glEnable(GL_TEXTURE_2D);	
	glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
	glClearDepth(1.0f);										// Depth Buffer Setup

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					// Set Line Antialiasing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable(GL_BLEND);										// Enable Blending

	// Alpha Test(AKA: Alpha Rejection)
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GEQUAL, 0.5f);

//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);		// Type Of Blending To Use
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 确保高光亮点计算是从视点出发
	//   注：false时高光视点方向的计算是按照平行与视点方向的向量
	//       true时高光视点方向从视点出发向被照亮点求向量(速度慢，但效果正确)
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// 开启光照
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	return true;
}

void CgGLRenderSystem::Shutdown()
{
    cgDestroyContext(m_CgContext);

	if (m_hRC)												// Do We Have A Rendering Context?
	{
		AssertFatal(wglMakeCurrent(NULL, NULL), "CgGLRenderSystem::Shutdown() : Release Of DC And RC Failed.");
		AssertFatal(wglDeleteContext(m_hRC), "CgGLRenderSystem::Shutdown() : Release Rendering Context Failed.");

		m_hRC = NULL;											// Set RC To NULL
	}

	if (m_hDC)
	{
		AssertFatal(ReleaseDC(m_hWnd, m_hDC), "CgGLRenderSystem::Shutdown() : Release Device Context Failed.");
		m_hDC = NULL;
	}

	// hWnd不要擅自删除，交由System完成
	//if (m_hWnd)
	//{
	//	AssertFatal(DestroyWindow(m_hWnd), "CgGLRenderSystem::Shutdown() : Could Not Release hWnd.");
	//	m_hWnd = NULL;
	//}

}

void CgGLRenderSystem::SetFrustum(Frustum* frustum)
{
	m_Frustum = frustum;
	//m_ProjMatrix = m_Frustum->BuildPrespectiveProjMatrix();
	m_ProjMatrix = m_Frustum->ProjectionMatrix();
}

void CgGLRenderSystem::SetClearColor(const Color4f& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void CgGLRenderSystem::ResizeRenderWindow(unsigned int width, unsigned int height)
{
	if (width != 0 && height != 0)
	{
		m_WindowWidth = width;
		m_WindowHeight = height;
	}

	if (m_WindowHeight == 0) m_WindowHeight = 1;
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);	//Reset The Current Viewport

	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0f,(GLfloat)m_WindowWidth/(GLfloat)m_WindowHeight,0.1f,100.0f);

	float w = (GLfloat)m_WindowWidth/(GLfloat)m_WindowHeight;


	// gluOrtho2D等于glOrtho最后两个参数设为1.0f
	gluOrtho2D(-w, w, -1.0f, 1.0f);
	//glOrtho(-w, w, -1.0f, 1.0f, 0.1f, 100.0f);
	*/
}

void CgGLRenderSystem::SetProjectionMode(ProjectionMatrixMode mode)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch (mode)
	{
	case PROJECTION_MODE_PERSPECTIVE:
		{
			//gluPerspective(45.0f,(GLfloat)m_WindowWidth/(GLfloat)m_WindowHeight,0.1f,100.0f);
			float projMat[16];
			BuildGLMatrix(m_ProjMatrix, projMat);
			glLoadMatrixf(projMat);
			//float projMat[16];
			//glGetFloatv(GL_PROJECTION_MATRIX, projMat);
			//int m = 0;
			//for (int y=0; y<4; y++)
			//	for (int x=0; x<4; x++)
			//	{
			//		m_ProjMatrix.m[x][y] = projMat[m];
			//		m++;
			//	}
			return;
		}
	case PROJECTION_MODE_ORTHO:
		{
			float w = (GLfloat)m_WindowWidth/(GLfloat)m_WindowHeight;
			gluOrtho2D(-w, w, -1.0f, 1.0f);
			return;
		}
	default:
		AssertFatal(0, "CgGLRenderSystem::SetProjectionMode() : Invalid projection matrix mode.");
	}
}

void CgGLRenderSystem::ClearBuffer(bool color, bool depth, bool stencil)
{
	GLbitfield clearMask = 0;
	if (color) clearMask |= GL_COLOR_BUFFER_BIT;
	if (depth) clearMask |= GL_DEPTH_BUFFER_BIT;
	if (stencil) clearMask |= GL_STENCIL_BUFFER_BIT;

	glClear(clearMask);
}

void CgGLRenderSystem::ToggleDepthTest(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void CgGLRenderSystem::ToggleLighting(bool enable)
{
	if (enable)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void CgGLRenderSystem::BeginRender()
{
	glMatrixMode(GL_MODELVIEW);
	
	float viewMat[16];
	BuildGLMatrix(m_ViewMatrix, viewMat);
	glLoadMatrixf(viewMat);

	// 灯光位置
	// 注：必须在每次更新了ViewMatrix之后也更新灯光位置，否则灯光位置就会相对视点固定

	//float lightDir[] = { 0.2f, 1.0f, 0.5f, 0.0f };		// w = 0.0f 代表这是一个方向光
	//glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
}

void CgGLRenderSystem::EndRender(bool swapBuffer)
{
	if (swapBuffer)
		SwapBuffers(m_hDC);
}


void CgGLRenderSystem::RenderMesh(const float* vertexArray,
							const float* normalArray,
							const float* textureCoordArray,
							unsigned int faceNumber,
							const unsigned int* faceArray,
							Material* material,
							//ILight* light,
							const Matrix4& transform)
{
	if (!faceNumber)
		return;

	//if (light)
	//{
	//	glEnable(GL_LIGHT0);
	//	float lightPos[4];
	//	memcpy(lightPos, light->GetPosition().GetArray(), 3 * sizeof(float));
	//	lightPos[3] = 1.0f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//	glLightfv(GL_LIGHT0, GL_AMBIENT, light->GetAmbientColor().GetArray());
	//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light->GetDiffuseColor().GetArray());
	//	glLightfv(GL_LIGHT0, GL_SPECULAR, light->GetSpecularColor().GetArray());
	//}
	//else
	//{
	//	glDisable(GL_LIGHT0);
	//}

	// 指定材质
	SetupMaterial(material, &transform);

	glPushMatrix();

	float mat[16];
	BuildGLMatrix(transform, mat);

	glMultMatrixf(mat);

	// TODO: 如果支持则使用VBO
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertexArray);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normalArray);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordArray);

	glDrawElements(GL_TRIANGLES, faceNumber * 3, GL_UNSIGNED_INT, faceArray);

	//unsigned int error = glGetError();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}

void CgGLRenderSystem::RenderAABB(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color, const Matrix4& transform)
{
	// TODO: 集中渲染Debug信息，不需要在这个方法内部关闭光照和纹理
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	cgGLDisableProfile(m_CgVertexProfile);
	CheckCgError();
	cgGLDisableProfile(m_CgFragmentProfile);
	CheckCgError();

	glColor4fv(color.GetArray());

	glPushMatrix();

	float mat[16];
	BuildGLMatrix(transform, mat);

	glMultMatrixf(mat);

	// 左矩形
	glBegin(GL_LINE_LOOP);
	glVertex3f(vMin.x, vMin.y, vMin.z);
	glVertex3f(vMin.x, vMax.y, vMin.z);
	glVertex3f(vMin.x, vMax.y, vMax.z);
	glVertex3f(vMin.x, vMin.y, vMax.z);
	glEnd();

	// 右矩形
	glBegin(GL_LINE_LOOP);
	glVertex3f(vMax.x, vMin.y, vMin.z);
	glVertex3f(vMax.x, vMax.y, vMin.z);
	glVertex3f(vMax.x, vMax.y, vMax.z);
	glVertex3f(vMax.x, vMin.y, vMax.z);
	glEnd();

	// 连接剩下四条线
	glBegin(GL_LINES);
	glVertex3f(vMin.x, vMin.y, vMin.z);
	glVertex3f(vMax.x, vMin.y, vMin.z);

	glVertex3f(vMin.x, vMax.y, vMin.z);
	glVertex3f(vMax.x, vMax.y, vMin.z);

	glVertex3f(vMin.x, vMax.y, vMax.z);
	glVertex3f(vMax.x, vMax.y, vMax.z);

	glVertex3f(vMin.x, vMin.y, vMax.z);
	glVertex3f(vMax.x, vMin.y, vMax.z);
	glEnd();

	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void CgGLRenderSystem::RenderSphere(const Vector3f& point, float radius, const Color4f& color, unsigned int segment)
{
	// 同上
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	cgGLDisableProfile(m_CgVertexProfile);
	CheckCgError();
	cgGLDisableProfile(m_CgFragmentProfile);
	CheckCgError();

	glColor4fv(color.GetArray());

	glBegin(GL_LINE_LOOP);
	for (unsigned int i=0; i<segment; i++)
	{
		glVertex3f(point.x, point.y + radius * cos(float(i)/segment * k2Pi), point.z + radius * sin(float(i)/segment * k2Pi));
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	for (unsigned int i=0; i<segment; i++)
	{
		glVertex3f(point.x + radius * sin(float(i)/segment * k2Pi), point.y + radius * cos(float(i)/segment * k2Pi), point.z);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	for (unsigned int i=0; i<segment; i++)
	{
		glVertex3f(point.x + radius * sin(float(i)/segment * k2Pi), point.y, point.z + radius * cos(float(i)/segment * k2Pi));
	}
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void CgGLRenderSystem::RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	cgGLDisableProfile(m_CgVertexProfile);
	CheckCgError();
	cgGLDisableProfile(m_CgFragmentProfile);
	CheckCgError();

	glColor4fv(color.GetArray());

	glBegin(GL_LINES);
	glVertex3fv(begin.GetArray());
	glVertex3fv(end.GetArray());
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

}

void CgGLRenderSystem::RenderScreenQuad(float left, float top, float right, float bottom, ITexture* texture)
{
	if (texture)
	{
		BindTexture(texture);
	}
	else
	{
		ITexture* null_texture = GetTexture("no_material");
		AssertFatal(null_texture, "CgGLRenderSystem::Render() : Unable to find 'no_material', define it in texture.cfg first.");

		BindTexture(null_texture);
	}

	glBegin(GL_QUADS);
	//glBegin(GL_TRIANGLES);
	glVertex3f(left, bottom, -1.0f);
	glTexCoord2f(1.0f, 0.0f);

	glVertex3f(right, bottom, -1.0f);
	glTexCoord2f(1.0f, 1.0f);

	glVertex3f(right, top, -1.0f);
	glTexCoord2f(0.0f, 1.0f);


	glVertex3f(left, top, -1.0f);
	glTexCoord2f(0.0f, 0.0f);


	//glVertex3f(-1.0f, -1.0f, -1.0f);
	//glTexCoord2f(0.0f, 1.0f);

	//glVertex3f(-1.0f, 1.0f, -1.0f);
	//glTexCoord2f(1.0f, 1.0f);

	//glVertex3f(1.0f, 1.0f, -1.0f);
	//glTexCoord2f(1.0f, 0.0f);

	//glVertex3f(1.0f, -1.0f, -1.0f);
	//glTexCoord2f(0.0f, 0.0f);*/


	glEnd();
}

void CgGLRenderSystem::RenderScreenQuad(int x1, int y1, int x2, int y2, ITexture* texture, const Color4f& color)
{
	glColor4f(color.r, color.g, color.b, color.a);

	if (texture)
	{
		BindTexture(texture);
	}
	else
	{
		ITexture* null_texture = GetTexture("no_material");
		AssertFatal(null_texture, "CgGLRenderSystem::Render() : Unable to find 'no_material', define it in texture.cfg first.");

		BindTexture(null_texture);
	}

	float aspect = (float)m_WindowWidth / m_WindowHeight;

	//(x1 / m_WindowWidth * 2 - 1) * aspect;

	float nx1 = ((float)x1 / m_WindowWidth * 2 - 1) * aspect;
	float nx2 = ((float)x2 / m_WindowWidth * 2 - 1) * aspect;
	float ny1 = (-(float)y1 / m_WindowHeight * 2 + 1);
	float ny2 = (-(float)y2 / m_WindowHeight * 2 + 1);
	//float nx1 = 2.0f * aspect * x1 / m_WindowWidth;
	//float nx2 = 2.0f * aspect * x2 / m_WindowWidth;
	//float ny1 = -2.0f * y1 / m_WindowHeight;
	//float ny2 = -2.0f * y2 / m_WindowHeight;

	glBegin(GL_QUADS);
	glVertex3f(nx1, ny2, -1.0f);
	glTexCoord2f(1.0f, 0.0f);

	glVertex3f(nx2, ny2, -1.0f);
	glTexCoord2f(1.0f, 1.0f);

	glVertex3f(nx2, ny1, -1.0f);
	glTexCoord2f(0.0f, 1.0f);

	glVertex3f(nx1, ny1, -1.0f);
	glTexCoord2f(0.0f, 0.0f);

	glEnd();
}

void CgGLRenderSystem::SetAmbientColor(const Color4f& color)
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color.GetArray());
}

const Color4f CgGLRenderSystem::GetAmbientColor()
{
	float ambientColor[4];
	glGetFloatv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	return Color4f(ambientColor);
}

ITexture* CgGLRenderSystem::BuildTexture(const String& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data)
{
	
	//if this texture ID is in use, unload the current texture
	if(m_TextureList.find(textureName) != m_TextureList.end())
	{
		glDeleteTextures(1, static_cast<CgGLTexture*>(m_TextureList[textureName])->GetGLTextureIDPtr());

		delete m_TextureList[textureName];
		//glDeleteTextures(1, &(m_TextureList[textureName]));
	}

	//GLint internalFormat = GL_RGB;
	GLint internalFormat = 3;
	GLint format = GL_BGR_EXT;

	if (bpp == 32)
	{
		//internalFormat = GL_RGBA;
		internalFormat = 4;
		format = GL_BGRA_EXT;
	}

	CgGLTexture* tex = new CgGLTexture();

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, tex->GetGLTextureIDPtr());

	//store the texture ID mapping
	m_TextureList[textureName] = tex;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, tex->GetGLTextureID());
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

	gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, data);

	return tex;
}

ITexture* CgGLRenderSystem::BuildDepthTexture(const String& textureName, unsigned int width, unsigned int height)
{
	
	if(m_TextureList.find(textureName) != m_TextureList.end())
	{
		glDeleteTextures(1, static_cast<CgGLTexture*>(m_TextureList[textureName])->GetGLTextureIDPtr());

		delete m_TextureList[textureName];
	}

	CgGLTexture* tex = new CgGLTexture();

	glGenTextures(1, tex->GetGLTextureIDPtr());

	m_TextureList[textureName] = tex;
	glBindTexture(GL_TEXTURE_2D, tex->GetGLTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
				 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	return tex;
}

ITexture* CgGLRenderSystem::GetTexture(const String& textureName)
{
	TextureList::iterator iter;

	if ((iter = m_TextureList.find(textureName)) != m_TextureList.end())
		return iter->second;

	return NULL;
}

int CgGLRenderSystem::GetMaxLightsNumber()
{
	// TODO: 最大光源数目不受到固定管线限制
	int num;
	glGetIntegerv(GL_MAX_LIGHTS, &num);

	return num;
}

void CgGLRenderSystem::SetupLight(int index, ILight* light)
{
	int lightIndex = GL_LIGHT0 + index;

	if (light)
	{
		glEnable(lightIndex);
		float lightPos[4] = {0.0f, 0.0f, 0.0f, 1.0f};

		switch(light->GetLightType())
		{
		case LIGHT_TYPE_POINT:
		case LIGHT_TYPE_SPOT:
			memcpy(lightPos, light->GetPosition().GetArray(), 3 * sizeof(float));
			lightPos[3] = 1.0f;
			break;
		case LIGHT_TYPE_DIRECTIONAL:
			memcpy(lightPos, light->GetDirection().GetArray(), 3 * sizeof(float));

			// OpenGL使用Position最后一位为0.0f表示方向光
			lightPos[3] = 0.0f;
			break;
		default:
			break;
		}
		glLightfv(lightIndex, GL_POSITION, lightPos);
		glLightfv(lightIndex, GL_AMBIENT, light->GetAmbientColor().GetArray());
		glLightfv(lightIndex, GL_DIFFUSE, light->GetDiffuseColor().GetArray());
		glLightfv(lightIndex, GL_SPECULAR, light->GetSpecularColor().GetArray());

		glLightf(lightIndex, GL_CONSTANT_ATTENUATION, light->GetConstantAttenuation());
		glLightf(lightIndex, GL_LINEAR_ATTENUATION, light->GetLinearAttenuation());
		glLightf(lightIndex, GL_QUADRATIC_ATTENUATION, light->GetQuadraticAttenuation());
	}
	else
	{
		glDisable(lightIndex);
	}
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
void CgGLRenderSystem::BuildGLMatrix(const Matrix4& mat, float* glMat)
{
	int m = 0;
	for (int y=0; y<4; y++)
		for (int x=0; x<4; x++)
		{
			glMat[m] = mat.m[x][y];
			m++;
		}
}

void CgGLRenderSystem::BindTexture(ITexture* texture, const Matrix4* matModel)
{
	CgGLTexture* glTex = static_cast<CgGLTexture*>(texture);

	glBindTexture(GL_TEXTURE_2D, glTex->GetGLTextureID());

	// 纹理环绕模式
	GLint warpMode;

	switch(glTex->GetWrapType())
	{
	case WRAP_TYPE_CLAMP:
		warpMode = GL_CLAMP;
		break;
	case WRAP_TYPE_REPEAT:
		warpMode = GL_REPEAT;
		break;
	default:
		break;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpMode);

	GLint minFilterType = GetFilterType(glTex->GetMinFilterType());
	GLint magFilterType = GetFilterType(glTex->GetMagFilterType());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterType);

	// 使用视点空间纹理坐标，用于生成Projective Decal
	if (glTex->GetUseEyeSpaceTexCoord())
	{
		// 启用自动纹理坐标生成功能
		// 注：R与Q在普通投影纹理中可能不需要，但在做阴影映射时会用到
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_GEN_Q);

		// 这里的视点空间矩阵的指定必须在对MODELVIEW矩阵乘以模型矩阵之前进行，
		// 否则纹理矩阵也会受到影响
		Matrix4 eyeMat;
		eyeMat = glTex->GetEyeSpaceMatrix();

		glTexGenfv(GL_S, GL_EYE_PLANE, eyeMat.m[0]); // Row 0
		glTexGenfv(GL_T, GL_EYE_PLANE, eyeMat.m[1]); // Row 1
		glTexGenfv(GL_R, GL_EYE_PLANE, eyeMat.m[2]); // Row 2
		glTexGenfv(GL_Q, GL_EYE_PLANE, eyeMat.m[3]); // Row 3

		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	}
	else
	{
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_GEN_Q);
	}

	// TODO: Gpu program会覆盖固定管线的属性，考虑使用shader时候不设置固定管线的属性
	if (glTex->GetVertexProgram())
	{
		CGparameter param = glTex->GetModelViewProjMat();
		if (param != 0)
		{
			//float matMVP[16];
			//BuildGLMatrix(m_ViewMatrix * m_ProjMatrix, matMVP);
			Matrix4 matMVP = m_ProjMatrix * m_ViewMatrix;
			if (matModel)
				matMVP *= *matModel;
			cgSetMatrixParameterfr(param, matMVP.mArray);
		}

		param = glTex->GetModelViewMat();
		if (param)
		{
			Matrix4 matMV = m_ViewMatrix;
			if (matModel)
				matMV *= *matModel;
			cgSetMatrixParameterfr(param, matMV.mArray);
		}

		//cgUpdateProgramParameters(glTex->GetVertexProgram());
		cgGLBindProgram(glTex->GetVertexProgram());
		CheckCgError();

		cgGLEnableProfile(m_CgVertexProfile);
		CheckCgError();
	}
	else
	{
		cgGLDisableProfile(m_CgVertexProfile);
		CheckCgError();
	}

	if (glTex->GetFragmentProgram())
	{
		cgGLBindProgram(glTex->GetFragmentProgram());
		CheckCgError();

		cgGLEnableProfile(m_CgFragmentProfile);
		CheckCgError();
	}
	else
	{
		cgGLDisableProfile(m_CgFragmentProfile);
		CheckCgError();
	}

}

GLint CgGLRenderSystem::GetFilterType(int type)
{
	switch (type)
	{
	case FILTER_TYPE_NEAREST:
		return GL_NEAREST;
	case FILTER_TYPE_LINEAR:
		return GL_LINEAR;
	case FILTER_TYPE_NEAREST_MIPMAP_NEAREST:
		return GL_NEAREST_MIPMAP_NEAREST;
	case FILTER_TYPE_LINEAR_MIPMAP_NEAREST:
		return GL_LINEAR_MIPMAP_NEAREST;
	case FILTER_TYPE_NEAREST_MIPMAP_LINEAR:
		return GL_NEAREST_MIPMAP_LINEAR;
	case FILTER_TYPE_LINEAR_MIPMAP_LINEAR:
		return GL_LINEAR_MIPMAP_LINEAR;
	}

	// 默认返回Nearest模式
	return GL_NEAREST;
}

void CgGLRenderSystem::SetupMaterial(Material* material, const Matrix4* matModel)
{
	if (material)
	{
		//glColor4fv(mat->GetColor().GetArray());
		if (material->GetLighting())
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->GetAmbient().GetArray());
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->GetDiffuse().GetArray());
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->GetSpecular().GetArray());
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->GetEmissive().GetArray());
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->GetSpecularLevel());

		if (material->GetDepthWriting())
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);

		if (material->GetDepthTest())
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		if (ITexture* tex = material->GetTexture())
		{
			BindTexture(tex, matModel);
		}
		else
		{
			ITexture* null_texture = GetTexture("no_material");
			AssertFatal(null_texture, "CgGLRenderSystem::Render() : Unable to find 'no_material', define it in texture.cfg first.");

			BindTexture(null_texture, matModel);
		}
	}
	else
	{
		// 绑定默认材质
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);

		ITexture* null_texture = GetTexture("no_material");
		AssertFatal(null_texture, "CgGLRenderSystem::Render() : Unable to find 'no_material', define it in texture.cfg first.");

		BindTexture(null_texture, matModel);
	}
}

bool CgGLRenderSystem::UnloadTexture(const String& textureName)
{
	bool result(true);
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if(m_TextureList.find(textureName) != m_TextureList.end())
	{
		glDeleteTextures(1, static_cast<CgGLTexture*>(m_TextureList[textureName])->GetGLTextureIDPtr());
		delete m_TextureList[textureName];
		m_TextureList.erase(textureName);
	}
	//otherwise, unload failed
	else
	{
		result = false;
	}

	return result;
}

void CgGLRenderSystem::UnloadAllTextures()
{
	//start at the begginning of the texture map
	TextureList::iterator i = m_TextureList.begin();

	//Unload the textures untill the end of the texture map is found
	while(i != m_TextureList.end())
	{
		UnloadTexture(i->first);
		i = m_TextureList.begin();
	}

	//clear the texture map
	m_TextureList.clear();
}

void CgGLRenderSystem::CheckCgError()
{
	CGerror error;
	const char* errorStr = cgGetLastErrorString(&error);

	if (error != CG_NO_ERROR)
	{
		if (error == CG_COMPILER_ERROR)
		{
			//errorStr.append(cgGetLastListing(m_CgContext));
			AssertFatal(0, "CgGLRenderSystem::CheckCgError() : Error checking cg!");
		}
		//AssertFatal(0, errorStr.Data());
		AssertFatal(0, "CgGLRenderSystem::CheckCgError() : Error checking cg!");
	}
}

CGprogram CgGLRenderSystem::LoadProgramFromFile(const String& filename, const String entry, CgProgramType type)
{
	CGprofile* profile;
	switch(type)
	{
	case CG_VERTEX_PROGRAM:
		profile = &m_CgVertexProfile;
		break;
	case CG_FRAGMENT_PROGRAM:
		profile = &m_CgFragmentProfile;
		break;
	default:
		AssertFatal(0, "CgGLRenderSystem::LoadProgramFromFile() : Invalid program type!");
	};

	CGprogram program = cgCreateProgramFromFile(m_CgContext, CG_SOURCE, filename.Data(), *profile, entry.Data(), NULL);
	CheckCgError();
	cgGLLoadProgram(program);
	CheckCgError();
	return program;
}
//
//CGprogram CgGLRenderSystem::CreateVertexProgramFromFile(const String& filename, const String entry)
//{
//	CGprogram program = cgCreateProgramFromFile(m_CgContext, CG_SOURCE, filename.Data(), m_CgVertexProfile, entry.Data(), NULL);
//	CheckCgError();
//	cgGLLoadProgram(program);
//	CheckCgError();
//	return program;
//}


IRenderer* CreateRenderSystem()
{
	CgGLRenderSystem* renderer = new CgGLRenderSystem;
	CgGLTexture::m_CgGLRenderer = renderer;
	return renderer;
}

