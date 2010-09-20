//-----------------------------------------------------------------------------------
/// GLRenderSystem.cpp OpenGL��Ⱦϵͳ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------


#include "Plugin.h"
#include "Debug.h"
#include "EString.h"
#include "GLTexture.h"
#include "GLCubeTexture.h"
#include "MathUtil.h"
#include "GLEnums.h"
#include "ILight.h"

#include "GLRenderSystem.h"
#include "GLVertexArray.h"
#include "GLVertexBufferObject.h"
#include "GLRenderTargetFBO.h"
#include "GLRenderTargetTexture.h"
#include "CgEffect.h"

#include "GL/glu.h"

namespace Gen
{

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
		m_ModelMatrix.MakeIdentity();
		m_ViewMatrix.MakeIdentity();
	}

	GLRenderer::~GLRenderer()
	{
	}

	RenderWindowHandle GLRenderer::GetRenderWindowHandle()
	{
		return m_WindowHandle;
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

		// TODO: Ŀǰֻ��Դ���ģʽ��Ч
		XVisualInfo *vi;

		vi = glXChooseVisual((Display*)m_WindowHandle.display, m_WindowHandle.screen, attrListDbl);
		if (vi == NULL)
		{
			vi = glXChooseVisual((Display*)m_WindowHandle.display, m_WindowHandle.screen, attrListSgl);
		}

		m_Context = glXCreateContext((Display*)m_WindowHandle.display, vi, 0, GL_TRUE);

		glXMakeCurrent((Display*)m_WindowHandle.display, m_WindowHandle.window, m_Context);

#endif	// #if defined __PLATFORM_WIN32

		// ע��glewInit�������wglMakeCurrent֮����У������޷��ɹ���ʼ��
		AssertFatal(GLEW_OK == glewInit(), "GLRenderer::Initialize() : Failed to init glew.");
		m_HardwareFeature.DetectFeature();

		glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
		glClearDepth(1.0f);										// Depth Buffer Setup

		glEnable(GL_CULL_FACE);

		ToggleTexture(true);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					// Set Line Antialiasing
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

		//glLineWidth(5.0f);
		//glEnable(GL_POLYGON_OFFSET_LINE);

		// ����ش���
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


//#define FORCE_VERTEX_ARRAY

		// ���Ӳ��֧�֣�ʹ��VBO������ʹ�ö�������
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

		// ��ʼ��CgGL
		m_CgGLDevice.Initialize();
		CgGLDevice::m_sGLRenderer = this;

		return true;
	}

	void GLRenderer::Shutdown()
	{
		UnloadGpuPrograms();
		m_CgGLDevice.Shutdown();

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

	void GLRenderer::SetModelMatrix(const Matrix4& modelMat)
	{
		glMatrixMode(GL_MODELVIEW);

		float glMat[16];
		BuildGLMatrix(m_ViewMatrix * modelMat, glMat);
		glLoadMatrixf(glMat);

		m_ModelMatrix = modelMat;
	}

	void GLRenderer::SetViewMatrix(const Matrix4& viewMat)
	{
		glMatrixMode(GL_MODELVIEW);

		float glMat[16];
		BuildGLMatrix(viewMat * m_ModelMatrix, glMat);
		glLoadMatrixf(glMat);

		// hack: ��RenderState����Renderer���Ժ�ɾ������
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
		// Hack: �����и�δ�ܽ�������⣬��glx���������Ȼ�������
#if defined __PLATFORM_LINUX
		depth = false;
#endif	// #if defined __PLATFORM_LINUX

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
			// TODO: ָ���ȽϷ�ʽ
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

	// �л���ָ������Ԫ
	void GLRenderer::ActiveTextureUnit(unsigned int index)
	{
		static unsigned int texUnit = 0;

		if (index!=texUnit)
		{
			glActiveTexture(GL_TEXTURE0 + index);
			texUnit = index;
		}
	}

	void GLRenderer::BeginRender(IRenderTarget* rt)
	{

		// �����ǰ��ȾĿ����ָ����ȾĿ��һ�£������Ķ�
		if (m_RenderTarget!=rt)
		{

			if (rt)
			{
				// �����RT��Ϊ��ȾĿ��
				rt->BindRenderTarget();
			}
			else
			{
				// ȡ������ȾĿ�꣬ʹ�����һ�ε�RT�ָ���Ⱦ����Ļ������
				if (m_RenderTarget) m_RenderTarget->UnbindRenderTarget();
			}

			m_RenderTarget = rt;
		}
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

	void GLRenderer::RenderVertexBuffer(IVertexBuffer* vbuffer, PrimitiveType type)
	{
		vbuffer->SetAsVertexDataSource();
		vbuffer->RenderPrimitive(type);
	}

	void GLRenderer::RenderVertexBuffer(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, PrimitiveType type)
	{
		vbuffer->SetAsVertexDataSource();
		ibuffer->RenderPrimitive(type);
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
		GpuProgramInfo info;
		info.filename = filename;
		info.entry = entry;
		info.type = type;

		// ����Ѿ����������Դ
		if (m_GpuPrograms.find(info)!=m_GpuPrograms.end())
		{
			return NULL;
		}

		IGpuProgram* program = m_CgGLDevice.LoadGpuProgramFromFile(filename, entry, type);
		m_GpuPrograms[info] = program;

		return program;
	}

	// ��ȡһ������Shader
	IGpuProgram* GLRenderer::GetGpuProgram(const String& filename, const String& entry, GpuProgramType type)
	{
		GpuProgramInfo info;
		info.filename = filename;
		info.entry = entry;
		info.type = type;

		if (m_GpuPrograms.find(info)==m_GpuPrograms.end())
			return NULL;

		return m_GpuPrograms[info];
	}

	void GLRenderer::EnableGpuProfile(GpuProfile profile)
	{
		m_CgGLDevice.EnableProfile(profile);
	}
	
	void GLRenderer::DisableGpuProfile(GpuProfile profile)
	{
		m_CgGLDevice.DisableProfile(profile);
	}

	void GLRenderer::BindGpuProgram(IGpuProgram* program)
	{
		m_CgGLDevice.BindCgProgram(program);
	}

	IEffect* GLRenderer::LoadEffect(const String& filename)
	{
		return m_CgGLDevice.LoadCgEffectFromFile(filename);
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

	IIndexBuffer* GLRenderer::BuildIndexBuffer()
	{
		return (*m_IndexBufferFactoryFunc)();
	}

	// ����һ���µ���ȾĿ��
	IRenderTarget* GLRenderer::CreateRenderTarget()
	{
		if (m_HardwareFeature.supportedFBO)
			return new GLRenderTargetFBO();

		return NULL;
		//return new GLRenderTargetTexture();
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

	void GLRenderer::BindTextureRenderState(IDeviceTexture* deviceTex, const TextureRenderState& texState, unsigned int texUnit)
	{
		// TODO: �����������ֲRenderer��
		ToggleTexture(true, texUnit);

		IDeviceTexture* texture;
		if (deviceTex)
		{
			texture = deviceTex;
			//}
			//else
			//{
			//	texture = GetTexture("no_material.bmp");
			//	AssertFatal(texture, "GLRenderer::BindTextureRenderState() : Unable to find 'no_material.bmp', put it in your data path.");
			//}
			texture->BindTexture();

			// TODO: ��һ��������״̬����
			// NOTE: �ο�http://www.opengl.org/wiki/Texture_Combiners
			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			//glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GetTextureEnvironmentMode(texState.envMode));

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GetTextureEnvironmentMode(texState.envMode));


			// ������ģʽ
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

			// TODO: �������ģʽ(�Ƿ�ʹ��mipmap)Ӧ�������������
			GLint minFilterType = GetFilterType(texState.minFilterType);
			GLint magFilterType = GetFilterType(texState.magFilterType);

			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilterType);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilterType);

			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();

			// ʹ���ӵ�ռ��������꣬��������Projective Decal
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

	IPlugin* CreatePluginInstance()
	{
		return new GLRenderer;
	}
}
