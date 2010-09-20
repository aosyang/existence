//-----------------------------------------------------------------------------------
/// GLRenderSystem.h OpenGL��Ⱦϵͳ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _GLRENDERSYSTEM_H
#define _GLRENDERSYSTEM_H

#include "IRenderDevice.h"
#include "GLHardwareFeature.h"
#include "CgGLDevice.h"

#include "GL/glew.h"

#if defined __PLATFORM_LINUX
#include <GL/glx.h>
#endif	// #if defined __PLATFORM_LINUX

namespace Gen
{
	typedef IVertexBuffer*(*FactoryCreateVertexBufferFunc)();
	typedef IIndexBuffer*(*FactoryCreateIndexBufferFunc)();

	struct GpuProgramInfo
	{
		String	filename;
		String	entry;
		GpuProgramType type;

		bool operator<(const GpuProgramInfo& rhs) const
		{
			if (filename==rhs.filename)
			{
				if (entry==rhs.entry)
					return type<rhs.type;
				else
					return entry<rhs.entry;
			}

			return filename<rhs.filename;
		}
	};

	typedef std::map<GpuProgramInfo, IGpuProgram*> GpuPrograms;

	class GLRenderer : public IRenderDevice
	{
	public:
		GLRenderer();
		~GLRenderer();

		// ָ������
		//   ��ʹ��CreateRenderWindow���ֶ�ָ��������Խ���Ⱦ����Ϊһ�����еĴ���
		//   ����Ƕ�������������

		RenderWindowHandle GetRenderWindowHandle();
		//inline void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
		//inline HWND GetHwnd() { return m_hWnd; }
		//void* GetHwnd();

		bool Initialize(RenderWindowParam* windowParam);
		void Shutdown();

		const String GetFeatureString() const;

		void SetClearColor(const Color4f& color);

		void SetViewport(int left, int bottom, unsigned int width, unsigned int height);

		void SetModelMatrix(const Matrix4& modelMat);
		void SetViewMatrix(const Matrix4& viewMat);
		void SetProjectionMatrix(const Matrix4& projMat);

		const Matrix4& GetModelMatrix() const { return m_ModelMatrix; }

		void ClearBuffer(bool color, bool depth, bool stencil);

		void ToggleTexture(bool enable, unsigned int index = 0);
		void ToggleDepthWriting(bool enable);
		void ToggleDepthTest(bool enable);
		void ToggleLighting(bool enable);
		void ToggleAlphaTest(bool enable);
		void SetAlphaReference(float ref);
		void ToggleBlending(bool enable);
		void SetBlendFactor(BlendFactor src, BlendFactor dst);

		// �л���ָ������Ԫ
		void ActiveTextureUnit(unsigned int index);

		void BeginRender(IRenderTarget* rt);
		void EndRender();
		void SwapBuffer();

		void BindTextureRenderState(IDeviceTexture* deviceTex, const TextureRenderState& texState, unsigned int texUnit = 0);

		void RenderVertexBuffer(IVertexBuffer* vbuffer, PrimitiveType type);
		void RenderVertexBuffer(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, PrimitiveType type);

		void SetAmbientColor(const Color4f& color);

		IDeviceTexture* BuildTexture();
		IDeviceTexture* BuildCubeTexture();

		// Materials

		// TODO: ���ֻ��OpenGL��Ч������ȥ�����ַ���
		void SetVertexColor(const Color4f& color);
		void SetMaterialAmbientColor(const Color4f& color);
		void SetMaterialDiffuseColor(const Color4f& color);
		void SetMaterialSpecularColor(const Color4f& color);
		void SetMaterialEmissiveColor(const Color4f& color);
		void SetMaterialSpecularLevel(float level);

		// Shaders

		// ��ȡһ��Shader
		IGpuProgram* LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type);

		// ��ȡһ������Shader
		IGpuProgram* GetGpuProgram(const String& filename, const String& entry, GpuProgramType type);

		void EnableGpuProfile(GpuProfile profile);
		void DisableGpuProfile(GpuProfile profile);
		void BindGpuProgram(IGpuProgram* program);

		// ����cgfx����
		IEffect* LoadEffect(const String& filename);

		int GetMaxLightsNumber();

		void SetupLight(int index, ILight* light);

		IVertexBuffer* BuildVertexBuffer();
		IIndexBuffer* BuildIndexBuffer();

		// Render target
		IRenderTarget* CreateRenderTarget();
	protected:

		// ����OpenGL����
		//   ��Matrix4ת��ΪOpenGL��ʹ�õ�float[16]
		void BuildGLMatrix(const Matrix4& mat, float* glMat);

		//void SetupMaterialWhite();

		void UnloadGpuPrograms();
	protected:
		RenderWindowHandle	m_WindowHandle;
#if defined __PLATFORM_WIN32
		HDC		m_hDC;
		HGLRC	m_hRC;
#elif defined __PLATFORM_LINUX
		GLXContext      m_Context;
#endif	// #if defined __PLATFORM_WIN32

		Matrix4			m_ModelMatrix;
		Matrix4			m_ViewMatrix;

		GpuPrograms		m_GpuPrograms;				///< Shader�б�

		bool			m_DepthWriting;				///< �Ƿ������д��
		IRenderTarget*	m_RenderTarget;				///< ��Ⱦ����ǰʹ�õ���ȾĿ��

		GLHardwareFeature	m_HardwareFeature;		///< Ӳ�����Բ�ѯ

		FactoryCreateVertexBufferFunc	m_VertexBufferFactoryFunc;		///< ���㻺�幤������
		FactoryCreateIndexBufferFunc	m_IndexBufferFactoryFunc;		///< �������幤������

		CgGLDevice		m_CgGLDevice;
	};

	extern "C" DLLEXPORT IPlugin* CreatePluginInstance();
}

#endif
