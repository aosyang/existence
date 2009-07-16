//-----------------------------------------------------------------------------------
/// GLRenderSystem.cpp OpenGL��Ⱦϵͳ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "GLRenderSystem.h"

#include "Debug.h"
#include "GLTexture.h"
#include "GLCubeTexture.h"
#include "MathUtil.h"
#include "GLVertexArray.h"
#include "GLVertexBufferObject.h"

#include "GL/glu.h"

#include <string>


//#define FORCE_VERTEX_ARRAY


GLRenderer::GLRenderer()
: m_hWnd(0), m_hDC(0), m_hRC(0),
  m_WindowWidth(0), m_WindowHeight(0), // m_FullScreen(false),
  m_Active(true),
  m_Frustum(NULL),
  m_DepthWriting(true),
  m_VertexBufferFactoryFunc(NULL)
{
	m_ViewMatrix.Identity();
}

GLRenderer::~GLRenderer()
{
}

RenderWindowHandle GLRenderer::GetRenderWindowHandle()
{
	return m_hWnd;
}

bool GLRenderer::Initialize(RenderWindowParam* windowParam)
{
	m_hWnd = windowParam->handle;

	AssertFatal(m_hDC = GetDC(m_hWnd), "GLRenderer::Initialize() : Failed to get device context.");

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

	AssertFatal(PixelFormat = ChoosePixelFormat(m_hDC,&pfd), "GLRenderer::CreateRenderWindow() : Can't Find A Suitable PixelFormat.");
	AssertFatal(SetPixelFormat(m_hDC, PixelFormat, &pfd), "GLRenderer::CreateRenderWindow() : Can't Set The PixelFormat.");
	AssertFatal(m_hRC=wglCreateContext(m_hDC), "GLRenderer::CreateRenderWindow() : Can't Create A GL Rendering Context.");
	AssertFatal(wglMakeCurrent(m_hDC, m_hRC), "GLRenderer::CreateRenderWindow() : Can't Activate The GL Rendering Context.");

	// ע��glewInit�������wglMakeCurrent֮����У������޷��ɹ���ʼ��
	AssertFatal(GLEW_OK == glewInit(), "GLRenderer::Initialize() : Failed to init glew.");
	m_HardwareFeature.DetectFeature();

	ResizeRenderWindow(windowParam->width, windowParam->height);							// Set Up Our Perspective GL Screen

	glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
	glClearDepth(1.0f);										// Depth Buffer Setup

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	ToggleTexture(true);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					// Set Line Antialiasing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	//glEnable(GL_BLEND);										// Enable Blending

	// Alpha Test(AKA: Alpha Rejection)
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GEQUAL, 0.5f);

//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);		// Type Of Blending To Use
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//GLfloat fogColor[4] = {0.5, 0.5, 0.5, 0.5}; //set the for color to grey
	//glEnable (GL_FOG); //enable the fog
	//glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2
	//glFogfv (GL_FOG_COLOR, fogColor); //set the fog color to our color chosen above
	//glFogf (GL_FOG_DENSITY, 0.1f); //set the density to the value above
	//glFogf(GL_FOG_START, 1.0f);
	//glFogf(GL_FOG_END, 1000.0f);

	// ȷ���߹���������Ǵ��ӵ����
	//   ע��falseʱ�߹��ӵ㷽��ļ����ǰ���ƽ�����ӵ㷽�������
	//       trueʱ�߹��ӵ㷽����ӵ������������������(�ٶ�������Ч����ȷ)
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	// ��������
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

#ifndef FORCE_VERTEX_ARRAY
	if (m_HardwareFeature.supportedVBO)
	{
		GLVertexBufferObject::ChooseFunc(__GLEW_VERSION_1_5==GL_TRUE);
		m_VertexBufferFactoryFunc = FactoryCreateVertexBufferObject;
	}
	else
#endif
		m_VertexBufferFactoryFunc = FactoryCreateVertexArray;

	if (__GLEW_ARB_texture_non_power_of_two==GL_FALSE)
		GLTexture::s_ForcePowOfTwo = true;

	return true;
}

void GLRenderer::Shutdown()
{
	UnloadAllTextures();

	if (m_hRC)												// Do We Have A Rendering Context?
	{
		AssertFatal(wglMakeCurrent(NULL, NULL), "GLRenderer::Shutdown() : Release Of DC And RC Failed.");
		AssertFatal(wglDeleteContext(m_hRC), "GLRenderer::Shutdown() : Release Rendering Context Failed.");

		m_hRC = NULL;											// Set RC To NULL
	}

	if (m_hDC)
	{
		AssertFatal(ReleaseDC(m_hWnd, m_hDC), "GLRenderer::Shutdown() : Release Device Context Failed.");
		m_hDC = NULL;
	}

	// hWnd��Ҫ����ɾ��������System���
	//if (m_hWnd)
	//{
	//	AssertFatal(DestroyWindow(m_hWnd), "GLRenderer::Shutdown() : Could Not Release hWnd.");
	//	m_hWnd = NULL;
	//}

}

void GLRenderer::SetFrustum(Frustum* frustum)
{
	m_Frustum = frustum;
	//m_ProjMatrix = m_Frustum->BuildPrespectiveProjMatrix();
	m_ProjMatrix = m_Frustum->ProjectionMatrix();
}

void GLRenderer::SetClearColor(const Color4f& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
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

	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0f,(GLfloat)m_WindowWidth/(GLfloat)m_WindowHeight,0.1f,100.0f);

	float w = (GLfloat)m_WindowWidth/(GLfloat)m_WindowHeight;


	// gluOrtho2D����glOrtho�������������Ϊ1.0f
	gluOrtho2D(-w, w, -1.0f, 1.0f);
	//glOrtho(-w, w, -1.0f, 1.0f, 0.1f, 100.0f);
	*/
}

void GLRenderer::SetProjectionMode(ProjectionMatrixMode mode)
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
		AssertFatal(0, "GLRenderer::SetProjectionMode() : Invalid projection matrix mode.");
	}
}

void GLRenderer::ClearBuffer(bool color, bool depth, bool stencil)
{

	GLbitfield clearMask = 0;
	if (color) clearMask |= GL_COLOR_BUFFER_BIT;
	if (depth)
	{
		// ������д�뱻�رգ��������´򿪣������޷������Ȼ���
		if (!m_DepthWriting)
		{
			glDepthMask(GL_TRUE);
		}
		clearMask |= GL_DEPTH_BUFFER_BIT;
	}
	if (stencil) clearMask |= GL_STENCIL_BUFFER_BIT;

	glClear(clearMask);

	if (depth && !m_DepthWriting)
	{
		glDepthMask(GL_FALSE);
	}
}

void GLRenderer::ToggleTexture(bool enable, unsigned int index)
{
	static bool texture[8] = { false };

	if (index>=8) return;

	glActiveTexture(GL_TEXTURE0 + index);
	if (enable && !texture[index])
	{
		glEnable(GL_TEXTURE_2D);
		texture[index] = true;
	}
	else if (!enable && texture[index])
	{
		//glActiveTexture(GL_TEXTURE0 + index);
		glDisable(GL_TEXTURE_CUBE_MAP);
		glDisable(GL_TEXTURE_2D);
		texture[index] = false;
	}

}

void GLRenderer::ToggleDepthWriting(bool enable)
{
	static bool depthWriting = true;

	if (enable && !depthWriting)
	{
		glDepthMask(GL_TRUE);
		depthWriting = true;
		m_DepthWriting = true;
	}
	else if (!enable && depthWriting)
	{
		glDepthMask(GL_FALSE);
		depthWriting = false;
		m_DepthWriting = false;
	}
}

void GLRenderer::ToggleDepthTest(bool enable)
{
	static bool depthTest = false;			// ��̬����ȷ����ǰ״̬

	if (enable && !depthTest)		// ����״̬�����ı�ʱ���в���
	{
		glEnable(GL_DEPTH_TEST);
		depthTest = true;
	}
	else if (!enable && depthTest)
	{
		glDisable(GL_DEPTH_TEST);
		depthTest = false;
	}
}

void GLRenderer::ToggleLighting(bool enable)
{
	static bool lighting = false;

	if (enable && !lighting)
	{
		glEnable(GL_LIGHTING);
		lighting = true;
	}
	else if (!enable && lighting)
	{
		glDisable(GL_LIGHTING);
		lighting = false;
	}
}

void GLRenderer::ToggleBlend(bool enable)
{
	static bool blend = false;
	
	if (enable && !blend)
	{
		glEnable(GL_BLEND);
		blend = true;
	}
	else if (!enable && blend)
	{
		glDisable(GL_BLEND);
		blend = false;
	}
}

void GLRenderer::BeginRender()
{
	glMatrixMode(GL_MODELVIEW);
	
	float viewMat[16];
	BuildGLMatrix(m_ViewMatrix, viewMat);
	glLoadMatrixf(viewMat);

	// �ƹ�λ��
	// ע��������ÿ�θ�����ViewMatrix֮��Ҳ���µƹ�λ�ã�����ƹ�λ�þͻ�����ӵ�̶�

	//float lightDir[] = { 0.2f, 1.0f, 0.5f, 0.0f };		// w = 0.0f ��������һ�������
	//glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
}

void GLRenderer::EndRender(bool swapBuffer)
{
	if (swapBuffer)
		SwapBuffers(m_hDC);
}

void GLRenderer::RenderVertexBuffer(IVertexBuffer* vbuffer, Material* material, const Matrix4& transform)
{
	// ָ������
	SetupMaterial(material);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	float mat[16];
	BuildGLMatrix(transform, mat);

	glMultMatrixf(mat);

	vbuffer->RenderBuffer();
	glPopMatrix();

	//glMatrixMode(GL_TEXTURE);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}

void GLRenderer::RenderAABB(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color, const Matrix4& transform)
{
	// TODO: ������ȾDebug��Ϣ������Ҫ����������ڲ��رչ��պ�����
	ToggleLighting(false);
	ToggleTexture(false);

	glColor4fv(color.GetArray());

	glPushMatrix();

	float mat[16];
	BuildGLMatrix(transform, mat);

	glMultMatrixf(mat);

	// �����
	glBegin(GL_LINE_LOOP);
	glVertex3f(vMin.x, vMin.y, vMin.z);
	glVertex3f(vMin.x, vMax.y, vMin.z);
	glVertex3f(vMin.x, vMax.y, vMax.z);
	glVertex3f(vMin.x, vMin.y, vMax.z);
	glEnd();

	// �Ҿ���
	glBegin(GL_LINE_LOOP);
	glVertex3f(vMax.x, vMin.y, vMin.z);
	glVertex3f(vMax.x, vMax.y, vMin.z);
	glVertex3f(vMax.x, vMax.y, vMax.z);
	glVertex3f(vMax.x, vMin.y, vMax.z);
	glEnd();

	// ����ʣ��������
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

	ToggleLighting(true);
	ToggleTexture(true);
}

void GLRenderer::RenderSphere(const Vector3f& point, float radius, const Color4f& color, unsigned int segment)
{
	// ͬ��
	ToggleLighting(false);
	ToggleTexture(false);

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

	ToggleLighting(true);
	ToggleTexture(true);
}

void GLRenderer::RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color)
{
	ToggleLighting(false);
	ToggleTexture(false);

	glColor4fv(color.GetArray());

	glBegin(GL_LINES);
	glVertex3fv(begin.GetArray());
	glVertex3fv(end.GetArray());
	glEnd();

	ToggleLighting(true);
	ToggleTexture(true);
}

void GLRenderer::RenderScreenQuad(float left, float top, float right, float bottom, ITexture* texture)
{
	for (int i=1; i<8; i++)
		ToggleTexture(false, i);
	ToggleTexture(true);

	if (texture)
	{
		BindTextureRenderState(texRenderState(texture));
	}
	else
	{
		ITexture* null_texture = GetTexture("no_material");
		AssertFatal(null_texture, "GLRenderer::Render() : Unable to find 'no_material', define it in texture.cfg first.");

		BindTextureRenderState(texRenderState(null_texture));
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

void GLRenderer::RenderScreenQuad(int x1, int y1, int x2, int y2, ITexture* texture, const Color4f& color)
{
	for (int i=1; i<8; i++)
		ToggleTexture(false, i);
	ToggleTexture(true);

	glColor4f(color.r, color.g, color.b, color.a);

	if (texture)
	{
		BindTextureRenderState(texRenderState(texture));
	}
	else
	{
		ITexture* null_texture = GetTexture("no_material");
		AssertFatal(null_texture, "GLRenderer::Render() : Unable to find 'no_material', define it in texture.cfg first.");

		BindTextureRenderState(texRenderState(null_texture));
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

void GLRenderer::SetAmbientColor(const Color4f& color)
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color.GetArray());
}

const Color4f GLRenderer::GetAmbientColor()
{
	float ambientColor[4];
	glGetFloatv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	return Color4f(ambientColor);
}

ITexture* GLRenderer::BuildTexture(const string& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data)
{
	if (width==0 || height==0)
		return NULL;
	
	//if this texture ID is in use, unload the current texture
	if(m_TextureList.find(textureName) != m_TextureList.end())
	{
		delete m_TextureList[textureName];
		//glDeleteTextures(1, &(m_TextureList[textureName]));
	}

	GLTexture* tex = new GLTexture();

	tex->Create(width, height, bpp, data);

	//store the texture ID mapping
	m_TextureList[textureName] = tex;
	return tex;
}

ITexture* GLRenderer::BuildCubeTexture(const string& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data[6])
{
	if (width==0 || height==0)
		return NULL;
	
	//if this texture ID is in use, unload the current texture
	if(m_TextureList.find(textureName) != m_TextureList.end())
	{
		delete m_TextureList[textureName];
		//glDeleteTextures(1, &(m_TextureList[textureName]));
	}

	GLCubeTexture* tex = new GLCubeTexture();

	for (int i=0; i<6; i++)
		tex->Create(width, height, bpp, data[i], i);

	//store the texture ID mapping
	m_TextureList[textureName] = tex;
	return tex;
}

ITexture* GLRenderer::BuildDepthTexture(const string& textureName, unsigned int width, unsigned int height)
{
	// TODO: ������ṹ�����仯��������Ҫ�޸�
	if(m_TextureList.find(textureName) != m_TextureList.end())
	{
		delete m_TextureList[textureName];
	}

	GLTexture* tex = new GLTexture();

	m_TextureList[textureName] = tex;
	glBindTexture(GL_TEXTURE_2D, tex->GetGLTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
				 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	return tex;
}

ITexture* GLRenderer::GetTexture(const string& textureName)
{
	TextureList::iterator iter;

	if ((iter = m_TextureList.find(textureName)) != m_TextureList.end())
		return iter->second;

	return NULL;
}

int GLRenderer::GetMaxLightsNumber()
{
	return m_HardwareFeature.GetMaxLightNum();
}

void GLRenderer::SetupLight(int index, ILight* light)
{
	static bool lightEnable[8] = { false };
	int lightIndex = GL_LIGHT0 + index;

	if (light)
	{
		if (!lightEnable[index])
		{
			glEnable(lightIndex);
			lightEnable[index] = true;
		}
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

			// OpenGLʹ��Position���һλΪ0.0f��ʾ�����
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
		if (lightEnable[index])
		{
			glDisable(lightIndex);
			lightEnable[index] = false;
		}
	}
}

IVertexBuffer* GLRenderer::BuildVertexBuffer()
{
	return (*m_VertexBufferFactoryFunc)();
}


//-----------------------------------------------------------------------------------
/// \brief
/// ��Matrix4����OpenGLʹ�õ�float[16]����
/// 
/// \param mat
/// Matrix4����
/// 
/// \param glMat
/// OpenGL����
/// 
/// ���������Matrix4����һ��float[16]�ľ������鹩OpenGL����ʹ��
/// 
/// \remarks
/// OpenGL����Column-major����˳������:<br>
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

void GLRenderer::BindTextureRenderState(const texRenderState_t& texState)
{
	GLTexture* glTex = static_cast<GLTexture*>(texState.texture);

	texState.texture->BindTexture();
	//glBindTexture(texState.texture->GetTarget(), glTex->GetGLTextureID());

	// ������ģʽ
	GLint wrapMode;

	switch(texState.wrapType)
	{
	case WRAP_TYPE_CLAMP:
		wrapMode = GL_CLAMP;
		break;
	case WRAP_TYPE_REPEAT:
		wrapMode = GL_REPEAT;
		break;
	case WRAP_TYPE_CLAMP_TO_EDGE:
		wrapMode = GL_CLAMP_TO_EDGE;
		break;
	default:
		break;
	}
	glTexParameteri(texState.texture->GetTarget(), GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(texState.texture->GetTarget(), GL_TEXTURE_WRAP_T, wrapMode);

	GLint minFilterType = GetFilterType(texState.minFilterType);
	GLint magFilterType = GetFilterType(texState.magFilterType);

	glTexParameteri(texState.texture->GetTarget(), GL_TEXTURE_MIN_FILTER, minFilterType);
	glTexParameteri(texState.texture->GetTarget(), GL_TEXTURE_MAG_FILTER, magFilterType);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//glEnable(GL_TEXTURE_CUBE_MAP);

	//glDisable(GL_TEXTURE_CUBE_MAP);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	// ʹ���ӵ�ռ��������꣬��������Projective Decal]
	switch (texState.genMode)
	{
	case GEN_MODE_EYE_LINEAR:
		{
			glDisable(GL_TEXTURE_CUBE_MAP);

			// �����Զ������������ɹ���
			// ע��R��Q����ͨͶӰ�����п��ܲ���Ҫ����������Ӱӳ��ʱ���õ�
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			glEnable(GL_TEXTURE_GEN_Q);

			// ������ӵ�ռ�����ָ�������ڶ�MODELVIEW�������ģ�;���֮ǰ���У�
			// �����������Ҳ���ܵ�Ӱ��
			Matrix4 eyeMat;
			eyeMat = texState.eyeSpaceMatrix;

			glTexGenfv(GL_S, GL_EYE_PLANE, eyeMat.m[0]); // Row 0
			glTexGenfv(GL_T, GL_EYE_PLANE, eyeMat.m[1]); // Row 1
			glTexGenfv(GL_R, GL_EYE_PLANE, eyeMat.m[2]); // Row 2
			glTexGenfv(GL_Q, GL_EYE_PLANE, eyeMat.m[3]); // Row 3

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		}
		break;
	case GEN_MODE_SPHERE:
		{
			glDisable(GL_TEXTURE_CUBE_MAP);

			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_GEN_Q);

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		}
		break;
	case GEN_MODE_CUBE_MAP:
		{
			glEnable(GL_TEXTURE_CUBE_MAP);

			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_GEN_Q);

			// ʹCube Mappingʹ���ӵ���ת���������Ǳ�ڿ���Ӱ�쵽�������꣬��Ҫע��
			glMatrixMode(GL_TEXTURE);
			//glPushMatrix();
			glLoadIdentity();

			Matrix4 m, invert;
			m.Identity();
			m.SetRotation(m_ViewMatrix.GetRotationMatrix());
			invert = m.GetInverseMatrix();
			float inv[16];
			BuildGLMatrix(invert, inv);
			glMultMatrixf(inv);

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

			//glPopMatrix();
		}
		break;
	case GEN_MODE_TEXCOORD:
		{
			glDisable(GL_TEXTURE_CUBE_MAP);

			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_GEN_Q);
		}
		break;
	}

	if (texState.useBlending)
	{
		ToggleBlend(true);
		glBlendFunc(GetBlendFactor(texState.srcBlendFactor),
					GetBlendFactor(texState.dstBlendFactor));
	}
	else
	{
		ToggleBlend(false);
	}

	// TODO: ��һ��������״̬����
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GetEnvMode(texState.envMode));	
}

GLint GLRenderer::GetFilterType(int type)
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

	// Ĭ�Ϸ���Nearestģʽ
	return GL_NEAREST;
}

GLint GLRenderer::GetBlendFactor(int factor)
{
	switch (factor)
	{
	case BLEND_FACTOR_ZERO:
		return GL_ZERO;
	case BLEND_FACTOR_ONE:
		return GL_ONE;
	case BLEND_FACTOR_SRC_COLOR:
		return GL_SRC_COLOR;
	case BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
		return GL_ONE_MINUS_SRC_COLOR;
	case BLEND_FACTOR_SRC_ALPHA:
		return GL_SRC_ALPHA;
	case BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
		return GL_ONE_MINUS_SRC_ALPHA;
	case BLEND_FACTOR_DST_ALPHA:
		return GL_DST_ALPHA;
	case BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
		return GL_ONE_MINUS_DST_ALPHA;
	case BLEND_FACTOR_DST_COLOR:
		return GL_DST_COLOR;
	case BLEND_FACTOR_ONE_MINUS_DST_COLOR:
		return GL_ONE_MINUS_DST_COLOR;
	case BLEND_FACTOR_SRC_ALPHA_SATURATE:
		return GL_SRC_ALPHA_SATURATE;
	}

	// ���������Ƿ�����������zero
	return BLEND_FACTOR_ZERO;
}

GLint GLRenderer::GetEnvMode(int mode)
{
	switch (mode)
	{
	case ENV_MODE_MODULATE:
		return GL_MODULATE;
	case ENV_MODE_REPLACE:
		return GL_REPLACE;
	case ENV_MODE_ADD:
		return GL_ADD;
	}

	return GL_MODULATE;
}


void GLRenderer::SetupMaterial(Material* material)
{
	if (material)
	{
		//glColor4fv(mat->GetColor().GetArray());
		bool lighting = material->GetLighting();

		ToggleLighting(lighting);
		if (!lighting)
		{
			glColor4fv(material->GetColor().GetArray());
		}

		static Color4f ambient(0.2f, 0.2f, 0.2f);
		static Color4f diffuse(0.8f, 0.8f, 0.8f);
		static Color4f specular(0.0f, 0.0f, 0.0f);
		static Color4f emissive(0.0f, 0.0f, 0.0f);
		static float specularLevel = 0.0f;

		if (material->GetAmbient() != ambient)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->GetAmbient().GetArray());
			ambient = material->GetAmbient();
		}

		if (material->GetDiffuse() != diffuse)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->GetDiffuse().GetArray());
			diffuse = material->GetDiffuse();
		}

		if (material->GetSpecular() != specular)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->GetSpecular().GetArray());
			specular = material->GetSpecular();
		}

		if (material->GetEmissive() != emissive)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->GetEmissive().GetArray());
			emissive = material->GetEmissive();
		}

		if (material->GetSpecularLevel() != specularLevel)
		{
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->GetSpecularLevel());
			specularLevel = material->GetSpecularLevel();
		}

		ToggleDepthWriting(material->GetDepthWriting());

		ToggleDepthTest(material->GetDepthTest());
		if (material->GetAlphaTest())
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GEQUAL, material->GetAlphaReference());
		}
		else
			glDisable(GL_ALPHA_TEST);

		texRenderState_t* rs;
		for (int i=0; i<8; i++)
		{
			if (material->GetTextureLayerEnabled(i))
			{
				ToggleTexture(true, i);
				//glActiveTexture(GL_TEXTURE0 + i);
				//glEnable(GL_TEXTURE_2D);

				if ((rs = material->GetTextureRenderState(i)) && material->GetTextureRenderState(i)->texture)
				{
					BindTextureRenderState(*rs);
				}
				else
				{
					ITexture* null_texture = GetTexture("no_material");
					AssertFatal(null_texture, "GLRenderer::Render() : Unable to find 'no_material', define it in texture.cfg first.");

					BindTextureRenderState(texRenderState(null_texture));
				}
			}
			else
			{
				// û��ʹ�ø�����㣬�رն�������
				ToggleTexture(false, i);
				//glActiveTexture(GL_TEXTURE0 + i);
				//glDisable(GL_TEXTURE_2D);
			}
		}
	}
	else
	{
		for (int i=1; i<8; i++)
		{
			ToggleTexture(false, i);
		}
		ToggleTexture(true);

		// ��Ĭ�ϲ���
		ToggleLighting(false);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		ToggleDepthWriting(true);
		ToggleDepthTest(true);

		ITexture* null_texture = GetTexture("no_material");
		AssertFatal(null_texture, "GLRenderer::Render() : Unable to find 'no_material', define it in texture.cfg first.");

		BindTextureRenderState(texRenderState(null_texture));
	}
}

bool GLRenderer::UnloadTexture(const string& textureName)
{
	bool result(true);
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if(m_TextureList.find(textureName) != m_TextureList.end())
	{
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

void GLRenderer::UnloadAllTextures()
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


IRenderer* CreateRenderSystem()
{
	return new GLRenderer;
}
