//-----------------------------------------------------------------------------------
/// GLRenderSystem.cpp OpenGL渲染系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "GLRenderSystem.h"

#include "Plugin.h"
#include "Debug.h"
#include "EString.h"
#include "GLTexture.h"
#include "GLCubeTexture.h"
#include "MathUtil.h"
#include "GLVertexArray.h"
#include "GLVertexBufferObject.h"
#include "GLRenderTargetFBO.h"
#include "GLRenderTargetTexture.h"

#include "GL/glu.h"

namespace Gen
{
	// gpu插件句柄
	Module_t	g_GpuPluginHandle = NULL;

	// gpu插件函数
	CREATEGPUPLUGIN				g_CreateGpuPluginFunc = NULL;
	DESTROYGPUPLUGIN			g_DestroyGpuPluginFunc = NULL;
	LOADGPUPROGRAM				g_LoadGpuProgramFunc = NULL;
	BINDGPUPROGRAM				g_BindGpuProgramFunc = NULL;
	UNBINDGPUPROGRAM			g_UnbindGpuProgramFunc = NULL;


	GLRenderer::GLRenderer()
	:
#if defined __PLATFORM_WIN32
		m_WindowHandle(0),
		m_hDC(0), m_hRC(0),
#elif defined __PLATFORM_LINUX
		m_Context(0),
#endif
		m_DepthWriting(true),
		m_RenderTarget(NULL),
		m_VertexBufferFactoryFunc(NULL),
		m_IndexBufferFactoryFunc(NULL)
	{
		m_ViewMatrix.MakeIdentity();
	}

	GLRenderer::~GLRenderer()
	{
	}

	RenderWindowHandle GLRenderer::GetRenderWindowHandle()
	{
		return m_WindowHandle;
	}

	void GLRenderer::SetGpuPluginName(const String& filename)
	{
		m_PluginName = filename;
	}

	bool GLRenderer::Initialize(RenderWindowParam* windowParam)
	{
		m_WindowHandle = windowParam->handle;

#if defined __PLATFORM_WIN32
		AssertFatal(m_hDC = GetDC(m_WindowHandle), "GLRenderer::Initialize() : Failed to get device context.");

		int PixelFormat;

		static	PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),					// Size
			1,												// Version Number
			PFD_DRAW_TO_WINDOW |							// dwFormat
			PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,								// Must Support Double Buffering
			PFD_TYPE_RGBA,									// Request An RGBA Format
			windowParam->colorDepthBit,					// Select Our Color Depth
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

#elif defined __PLATFORM_LINUX

		/* attributes for a single buffered visual in RGBA format with at least
		* 4 bits per color and a 16 bit depth buffer */
		static int attrListSgl[] = {GLX_RGBA, GLX_RED_SIZE, 4,
			GLX_GREEN_SIZE, 4,
			GLX_BLUE_SIZE, 4,
			GLX_DEPTH_SIZE, 16,
			None
		};

		/* attributes for a double buffered visual in RGBA format with at least
		* 4 bits per color and a 16 bit depth buffer */
		static int attrListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER,
			GLX_RED_SIZE, 4,
			GLX_GREEN_SIZE, 4,
			GLX_BLUE_SIZE, 4,
			GLX_DEPTH_SIZE, 16,
			None
		};

		// TODO: 目前只针对窗口模式有效
		XVisualInfo *vi;

		vi = glXChooseVisual((Display*)m_WindowHandle.display, m_WindowHandle.screen, attrListDbl);
		if (vi == NULL)
		{
			vi = glXChooseVisual((Display*)m_WindowHandle.display, m_WindowHandle.screen, attrListSgl);
		}

		m_Context = glXCreateContext((Display*)m_WindowHandle.display, vi, 0, GL_TRUE);

		glXMakeCurrent((Display*)m_WindowHandle.display, m_WindowHandle.window, m_Context);

#endif	// #if defined __PLATFORM_WIN32

		// 注：glewInit必须放在wglMakeCurrent之后进行，否则无法成功初始化
		AssertFatal(GLEW_OK == glewInit(), "GLRenderer::Initialize() : Failed to init glew.");
		m_HardwareFeature.DetectFeature();

		if (m_PluginName!="")
		{
			g_GpuPluginHandle = LoadModule(m_PluginName.Data());

			if (g_GpuPluginHandle)
			{
				g_CreateGpuPluginFunc = (CREATEGPUPLUGIN)GetFunction(g_GpuPluginHandle, "CreateGpuPlugin");
				g_DestroyGpuPluginFunc = (DESTROYGPUPLUGIN)GetFunction(g_GpuPluginHandle, "DestroyGpuPlugin");
				g_LoadGpuProgramFunc = (LOADGPUPROGRAM)GetFunction(g_GpuPluginHandle, "LoadGpuProgram");
				g_BindGpuProgramFunc = (BINDGPUPROGRAM)GetFunction(g_GpuPluginHandle, "BindGpuProgram");
				g_UnbindGpuProgramFunc = (UNBINDGPUPROGRAM)GetFunction(g_GpuPluginHandle, "UnbindGpuProgram");

				// 初始化插件
				(*g_CreateGpuPluginFunc)();
			}
		}

		glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
		glClearDepth(1.0f);										// Depth Buffer Setup

		glEnable(GL_CULL_FACE);

		ToggleTexture(true);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					// Set Line Antialiasing
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

		//glLineWidth(5.0f);
		//glEnable(GL_POLYGON_OFFSET_LINE);

		// 雾相关代码
		//GLfloat fogColor[4] = {0.5, 0.5, 0.5, 0.5}; //set the for color to grey
		//glEnable (GL_FOG); //enable the fog
		//glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2
		//glFogfv (GL_FOG_COLOR, fogColor); //set the fog color to our color chosen above
		//glFogf (GL_FOG_DENSITY, 0.1f); //set the density to the value above
		//glFogf(GL_FOG_START, 1.0f);
		//glFogf(GL_FOG_END, 1000.0f);

		// 确保高光亮点计算是从视点出发
		//   注：false时高光视点方向的计算是按照平行与视点方向的向量
		//       true时高光视点方向从视点出发向被照亮点求向量(速度慢，但效果正确)
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
		//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);


//#define FORCE_VERTEX_ARRAY

		// 如果硬件支持，使用VBO，否则使用顶点数组
#ifndef FORCE_VERTEX_ARRAY
		if (m_HardwareFeature.supportedVBO)
		{
			ChooseVBOMethods(__GLEW_VERSION_1_5==GL_TRUE);
			m_VertexBufferFactoryFunc = FactoryCreateVBOVertexBuffer;
			m_IndexBufferFactoryFunc = FactoryCreateVBOIndexBuffer;
		}
		else
#endif
		{
			m_VertexBufferFactoryFunc = FactoryCreateVertexArray;
			m_IndexBufferFactoryFunc = FactoryCreateIndexArray;
		}

		if (!m_HardwareFeature.supportedNonPowOf2Texture)
			GLTexture::m_sForcePowOfTwo = true;

		return true;
	}

	void GLRenderer::Shutdown()
	{
		UnloadGpuPrograms();

		if (g_GpuPluginHandle)
			(*g_DestroyGpuPluginFunc)();

#if defined __PLATFORM_WIN32
		if (m_hRC)												// Do We Have A Rendering Context?
		{
			AssertFatal(wglMakeCurrent(NULL, NULL), "GLRenderer::Shutdown() : Release Of DC And RC Failed.");
			AssertFatal(wglDeleteContext(m_hRC), "GLRenderer::Shutdown() : Release Rendering Context Failed.");

			m_hRC = NULL;											// Set RC To NULL
		}

		if (m_hDC)
		{
			AssertFatal(ReleaseDC(m_WindowHandle, m_hDC), "GLRenderer::Shutdown() : Release Device Context Failed.");
			m_hDC = NULL;
		}

#elif defined __PLATFORM_LINUX
		if (m_Context)
		{
			AssertFatal(glXMakeCurrent((Display*)m_WindowHandle.display, None, 0), "GLRenderer::Shutdown(): Could not release drawing context.");
			glXDestroyContext((Display*)m_WindowHandle.display, m_Context);
			m_Context = 0;
		}
#endif	// #if defined __PLATFORM_WIN32
	}

	const String GLRenderer::GetFeatureString() const
	{
		return m_HardwareFeature.m_FeatureString;
	}

	void GLRenderer::SetClearColor(const Color4f& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GLRenderer::SetViewport(int left, int bottom, unsigned int width, unsigned int height)
	{
		glViewport(left, bottom, width, height);
	}

	void GLRenderer::SetViewMatrix(const Matrix4& viewMat)
	{
		glMatrixMode(GL_MODELVIEW);

		float glMat[16];
		BuildGLMatrix(viewMat, glMat);
		glLoadMatrixf(glMat);

		// hack: 当RenderState移至Renderer中以后，删除这里
		m_ViewMatrix = viewMat;
	}

	void GLRenderer::SetProjectionMatrix(const Matrix4& projMat)
	{
		glMatrixMode(GL_PROJECTION);

		float glMat[16];
		BuildGLMatrix(projMat, glMat);
		glLoadMatrixf(glMat);
	}


	void GLRenderer::ClearBuffer(bool color, bool depth, bool stencil)
	{

		GLbitfield clearMask = 0;
		if (color) clearMask |= GL_COLOR_BUFFER_BIT;

		if (depth)
		{
			// 如果深度写入被关闭，必须重新打开，否则无法清除深度缓冲
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

		ActiveTextureUnit(index);
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
		static bool depthTest = false;			// 静态变量确定当前状态

		if (enable && !depthTest)		// 仅当状态发生改变时进行操作
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

	void GLRenderer::ToggleAlphaTest(bool enable)
	{
		static bool alphaTest = false;

		if (enable && !alphaTest)
		{
			glEnable(GL_ALPHA_TEST);
			alphaTest = true;
		}
		else if (!enable && alphaTest)
		{
			glDisable(GL_ALPHA_TEST);
			alphaTest = false;
		}
	}

	void GLRenderer::SetAlphaReference(float ref)
	{
		static float alphaRef = 0.0f;

		if (!FLOAT_EQUAL(alphaRef, ref))
		{
			// TODO: 指定比较方式
			glAlphaFunc(GL_GEQUAL, ref);
			alphaRef = ref;
		}
	}

	void GLRenderer::ToggleBlending(bool enable)
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

	void GLRenderer::SetBlendFactor(BlendFactor src, BlendFactor dst)
	{
		glBlendFunc(GetBlendFactor(src), GetBlendFactor(dst));
	}

	// 切换到指定纹理单元
	void GLRenderer::ActiveTextureUnit(unsigned int index)
	{
		static unsigned int texUnit = 0;

		if (index!=texUnit)
		{
			glActiveTexture(GL_TEXTURE0 + index);
			texUnit = index;
		}
	}

	void GLRenderer::BeginRender()
	{
		//glMatrixMode(GL_MODELVIEW);

		//float viewMat[16];
		//BuildGLMatrix(m_ViewMatrix, viewMat);
		//glLoadMatrixf(viewMat);

		// 灯光位置
		// 注：必须在每次更新了ViewMatrix之后也更新灯光位置，否则灯光位置就会相对视点固定

		//float lightDir[] = { 0.2f, 1.0f, 0.5f, 0.0f };		// w = 0.0f 代表这是一个方向光
		//glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
	}

	void GLRenderer::EndRender()
	{
	}

	void GLRenderer::SwapBuffer()
	{
#if defined __PLATFORM_WIN32
		SwapBuffers(m_hDC);
#elif defined __PLATFORM_LINUX
		glXSwapBuffers((Display*)m_WindowHandle.display, m_WindowHandle.window);
#endif	// #if defined __PLATFORM_WIN32
	}

	void GLRenderer::RenderVertexBuffer(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, const Matrix4& transform)
	{
		m_ModelMatrix = transform;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		float mat[16];
		BuildGLMatrix(transform, mat);

		glMultMatrixf(mat);

		vbuffer->SetAsVertexDataSource();
		ibuffer->RenderPrimitive();
		glPopMatrix();

		//glMatrixMode(GL_TEXTURE);
		//glPopMatrix();
		//glMatrixMode(GL_MODELVIEW);
	}

	void GLRenderer::RenderBox(const Vector3f& vMin, const Vector3f& vMax, const Matrix4& transform)
	{
		glMatrixMode(GL_MODELVIEW);
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
	}

	void GLRenderer::RenderSphere(const Vector3f& point, float radius, unsigned int segment)
	{
		glBegin(GL_LINE_LOOP);
		for (unsigned int i=0; i<segment; i++)
		{
			glVertex3f(point.x, point.y + radius * cos(float(i)/segment * Math::k2Pi), point.z + radius * sin(float(i)/segment * Math::k2Pi));
		}
		glEnd();

		glBegin(GL_LINE_LOOP);
		for (unsigned int i=0; i<segment; i++)
		{
			glVertex3f(point.x + radius * sin(float(i)/segment * Math::k2Pi), point.y + radius * cos(float(i)/segment * Math::k2Pi), point.z);
		}
		glEnd();

		glBegin(GL_LINE_LOOP);
		for (unsigned int i=0; i<segment; i++)
		{
			glVertex3f(point.x + radius * sin(float(i)/segment * Math::k2Pi), point.y, point.z + radius * cos(float(i)/segment * Math::k2Pi));
		}
		glEnd();
	}

	void GLRenderer::RenderLine(const Vector3f& begin, const Vector3f& end)
	{
		glBegin(GL_LINES);
		glVertex3fv(begin.GetArray());
		glVertex3fv(end.GetArray());
		glEnd();
	}

	void GLRenderer::SetAmbientColor(const Color4f& color)
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color.GetArray());
	}


	IDeviceTexture* GLRenderer::BuildTexture()
	{
		return new GLTexture();
	}

	IDeviceTexture* GLRenderer::BuildCubeTexture()
	{
		return new GLCubeTexture();
	}

	IDeviceTexture* GLRenderer::BuildDepthTexture(const String& textureName, unsigned int width, unsigned int height)
	{
		//// TODO: 纹理类结构发生变化，这里需要修改
		//if(m_TextureList.find(textureName) != m_TextureList.end())
		//{
		//	delete m_TextureList[textureName];
		//}

		//GLTexture* tex = new GLTexture();

		//m_TextureList[textureName] = tex;
		//glBindTexture(GL_TEXTURE_2D, tex->GetGLTextureID());
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
		//	GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

		//return tex;
		return NULL;
	}

	void GLRenderer::SetVertexColor(const Color4f& color)
	{
		glColor4fv(color.GetArray());
	}

	void GLRenderer::SetMaterialAmbientColor(const Color4f& color)
	{
		static Color4f ambient(0.2f, 0.2f, 0.2f);

		if (color!=ambient)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color.GetArray());
			ambient = color;
		}
	}

	void GLRenderer::SetMaterialDiffuseColor(const Color4f& color)
	{
		static Color4f diffuse(0.8f, 0.8f, 0.8f);

		if (color!=diffuse)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color.GetArray());
			diffuse = color;
		}
	}

	void GLRenderer::SetMaterialSpecularColor(const Color4f& color)
	{
		static Color4f specular(0.0f, 0.0f, 0.0f);

		if (color!=specular)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color.GetArray());
			specular = color;
		}
	}

	void GLRenderer::SetMaterialEmissiveColor(const Color4f& color)
	{
		static Color4f emissive(0.0f, 0.0f, 0.0f);

		if (color!=emissive)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color.GetArray());
			emissive = color;
		}
	}

	void GLRenderer::SetMaterialSpecularLevel(float level)
	{
		static float specularLevel = 0.0f;

		if (!FLOAT_EQUAL(specularLevel, level))
		{
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, level);
			specularLevel = level;
		}
	}

	IGpuProgram* GLRenderer::LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type)
	{
		if (!g_GpuPluginHandle) return NULL;

		GpuProgramInfo info;
		info.filename = filename;
		info.entry = entry;
		info.type = type;

		// 如果已经存在这个资源
		if (m_GpuPrograms.find(info)!=m_GpuPrograms.end())
		{
			return NULL;
		}

		IGpuProgram* program = (*g_LoadGpuProgramFunc)(filename, entry, type);
		m_GpuPrograms[info] = program;

		return program;
	}

	// 获取一个已有Shader
	IGpuProgram* GLRenderer::GetGpuProgram(const String& filename, const String& entry, GpuProgramType type)
	{
		if (!g_GpuPluginHandle) return NULL;

		GpuProgramInfo info;
		info.filename = filename;
		info.entry = entry;
		info.type = type;

		if (m_GpuPrograms.find(info)==m_GpuPrograms.end())
			return NULL;

		return m_GpuPrograms[info];
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

	IIndexBuffer* GLRenderer::BuildIndexBuffer()
	{
		return (*m_IndexBufferFactoryFunc)();
	}

	// 创建一个新的渲染目标
	IRenderTarget* GLRenderer::CreateRenderTarget()
	{
		if (m_HardwareFeature.supportedFBO)
			return new GLRenderTargetFBO();
		return new GLRenderTargetTexture();
	}

	// 指定渲染目标
	void GLRenderer::BindRenderTarget(IRenderTarget* rt)
	{
		AssertFatal(!m_RenderTarget, "GLRenderer::BindRenderTarget(): A render target has been assigned already.");
		AssertFatal(rt, "GLRenderer::BindRenderTarget(): Unable to deal with a null render target.");

		m_RenderTarget = rt;
		rt->BindRenderTarget();
	}

	// 解除渲染目标，重新渲染到后台缓冲
	void GLRenderer::UnbindRenderTarget()
	{
		AssertFatal(m_RenderTarget, "GLRenderer::UnbindRenderTarget(): Function called without a render target being assigned.");

		m_RenderTarget->UnbindRenderTarget();
		m_RenderTarget = NULL;
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

	void GLRenderer::BindTextureRenderState(const TextureRenderState& texState, unsigned int texUnit)
	{
		// TODO: 将这个函数移植Renderer中
		ToggleTexture(true, texUnit);

		IDeviceTexture* texture;
		if (texState.texture)
		{
			texture = texState.texture;
			//}
			//else
			//{
			//	texture = GetTexture("no_material.bmp");
			//	AssertFatal(texture, "GLRenderer::BindTextureRenderState() : Unable to find 'no_material.bmp', put it in your data path.");
			//}
			texture->BindTexture();

			// TODO: 给一个单独的状态设置
			// NOTE: 参考http://www.opengl.org/wiki/Texture_Combiners
			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			//glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GetTextureEnvironmentMode(texState.envMode));

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GetTextureEnvironmentMode(texState.envMode));


			// 纹理环绕模式
			GLint wrapMode = GL_CLAMP;

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
			}

			GLenum target;
			switch (texture->GetType())
			{
			case TEXTURE_TYPE_CUBE:
				target = GL_TEXTURE_CUBE_MAP;
			case TEXTURE_TYPE_2D:
			default:
				target = GL_TEXTURE_2D;
			}

			glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapMode);

			// TODO: 纹理过滤模式(是否使用mipmap)应当由纹理类决定
			GLint minFilterType = GetFilterType(texState.minFilterType);
			GLint magFilterType = GetFilterType(texState.magFilterType);

			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilterType);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilterType);

			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();

			// 使用视点空间纹理坐标，用于生成Projective Decal
			switch (texState.genMode)
			{
			case GEN_MODE_EYE_LINEAR:
				{
					glDisable(GL_TEXTURE_CUBE_MAP);

					// 启用自动纹理坐标生成功能
					// 注：R与Q在普通投影纹理中可能不需要，但在做阴影映射时会用到
					glEnable(GL_TEXTURE_GEN_S);
					glEnable(GL_TEXTURE_GEN_T);
					glEnable(GL_TEXTURE_GEN_R);
					glEnable(GL_TEXTURE_GEN_Q);

					// 这里的视点空间矩阵的指定必须在对MODELVIEW矩阵乘以模型矩阵之前进行，
					// 否则纹理矩阵也会受到影响
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

					// 使Cube Mapping使用视点旋转，这个方法潜在可能影响到纹理坐标，需要注意
					glMatrixMode(GL_TEXTURE);
					//glPushMatrix();
					glLoadIdentity();

					Matrix4 m, invert;
					m.MakeIdentity();
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
		}
		//else
		//{
		//	glDisable(GL_TEXTURE_2D);
		//}

	}

	void GLRenderer::BindGpuProgram(IGpuProgram* program, GpuProgramType type)
	{
		if (g_GpuPluginHandle)
		{
			//Matrix4 mvp = m_ProjMatrix * m_ViewMatrix * m_ModelMatrix;
			//program->SetMatrix4Param("autoMatModelViewProj", mvp);

			//(g_BindGpuProgramFunc)(program, type);
		}
	}

	void GLRenderer::UnbindGpuProgram(GpuProgramType type)
	{
		if (g_GpuPluginHandle)
		{
			(g_UnbindGpuProgramFunc)(type);
		}
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

		// 默认返回Nearest模式
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

		// 传递其他非法参数，返回zero
		return BLEND_FACTOR_ZERO;
	}

	GLint GLRenderer::GetTextureEnvironmentMode(int mode)
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

	//void GLRenderer::SetupMaterialWhite()
	//{
	//	for (int i=1; i<8; i++)
	//	{
	//		ToggleTexture(false, i);
	//	}
	//	UnbindGpuProgram(GPU_VERTEX_PROGRAM);
	//	UnbindGpuProgram(GPU_FRAGMENT_PROGRAM);

	//	//ToggleTexture(true);

	//	ToggleLighting(false);
	//	ToggleDepthWriting(true);
	//	ToggleDepthTest(true);

	//	//IDeviceTexture* null_texture = GetTexture("#white");
	//	BindTextureRenderState(TextureRenderState(NULL));
	//}

	void GLRenderer::UnloadGpuPrograms()
	{
		GpuPrograms::iterator iter;
		for (iter=m_GpuPrograms.begin(); iter!=m_GpuPrograms.end(); iter++)
		{
			delete iter->second;
		}

		m_GpuPrograms.clear();
	}

	IRenderDevice* CreateRenderSystem()
	{
		return new GLRenderer;
	}
}
