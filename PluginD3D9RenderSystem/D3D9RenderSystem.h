//-----------------------------------------------------------------------------------
/// D3D9RenderSystem.h Direct3D 9渲染系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _D3D9RENDERSYSTEM_H
#define _D3D9RENDERSYSTEM_H

#include "IRenderDevice.h"

#include <d3dx9.h>

#include "D3D9VertexBuffer.h"

namespace Gen
{
	class D3D9Renderer : public IRenderDevice
	{
		friend class D3D9VertexBuffer;
	public:
		D3D9Renderer();
		~D3D9Renderer();


		RenderWindowHandle GetRenderWindowHandle();

		void SetGpuPluginName(const String& filename);

		bool Initialize(RenderWindowParam* windowParam);
		void Shutdown();

		const String GetFeatureString() const;

		void SetClearColor(const Color4f& color);

		void SetViewport(int left, int bottom, unsigned int width, unsigned int height);

		// 指定视图矩阵
		void SetViewMatrix(const Matrix4& viewMat);

		// 指定投影矩阵
		void SetProjectionMatrix(const Matrix4& projMat);

		void ClearBuffer(bool color, bool depth, bool stencil);

		void ToggleTexture(bool enable, unsigned int index = 0);
		void ToggleDepthWriting(bool enable);
		void ToggleDepthTest(bool enable);
		void ToggleLighting(bool enable);
		void ToggleAlphaTest(bool enable);
		void SetAlphaReference(float ref);
		void ToggleBlending(bool blend);
		void SetBlendFactor(BlendFactor src, BlendFactor dst);

		void BeginRender();
		void EndRender();
		void SwapBuffer();

		void BindTextureRenderState(const TextureRenderState& texState, unsigned int texUnit = 0);

		void RenderVertexBuffer(IVertexBuffer* vbuffer, PrimitiveType type, const Matrix4& transform);
		void RenderVertexBuffer(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, const Matrix4& transform);

		void SetAmbientColor(const Color4f& color);

		IDeviceTexture* BuildTexture();
		IDeviceTexture* BuildCubeTexture();

		// Materials
		void SetVertexColor(const Color4f& color);
		void SetMaterialAmbientColor(const Color4f& color);
		void SetMaterialDiffuseColor(const Color4f& color);
		void SetMaterialSpecularColor(const Color4f& color);
		void SetMaterialEmissiveColor(const Color4f& color);
		void SetMaterialSpecularLevel(float level);

		// Shaders

		// 读取一个Shader
		IGpuProgram* LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type);

		// 获取一个已有Shader
		IGpuProgram* GetGpuProgram(const String& filename, const String& entry, GpuProgramType type);

		void BindGpuProgram(IGpuProgram* program, GpuProgramType type);
		void UnbindGpuProgram(GpuProgramType type);

		int GetMaxLightsNumber();

		void SetupLight(int index, ILight* light);

		IVertexBuffer* BuildVertexBuffer();
		IIndexBuffer* BuildIndexBuffer();

		// Render target
		IRenderTarget* CreateRenderTarget();
		void BindRenderTarget(IRenderTarget* rt);
		void UnbindRenderTarget();
	protected:
		void BuildD3DMatrix(const Matrix4& mat, D3DMATRIX& d3dMat);

		DWORD GetBlendFactor(BlendFactor factor);

		void SetupMaterial(Material* material);
		void ToggleLight(bool enable, unsigned int index);
	protected:
		IDirect3D9*			m_Direct3D;
		IDirect3DDevice9*	m_D3DDevice;

		// 清屏颜色
		D3DCOLOR			m_ClearColor;
	};

	extern "C" DLLEXPORT IPlugin* CreatePluginInstance();
}

#endif
