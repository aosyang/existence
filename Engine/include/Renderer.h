//-----------------------------------------------------------------------------------
/// Renderer.h 引擎端渲染器，由引擎直接调用，并调用渲染设备执行渲染操作
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _RENDERER_H
#define _RENDERER_H

#include "Singleton.h"
#include "IRenderDevice.h"
#include "EString.h"

#include <vector>


namespace Gen
{
	class SceneObject;
	class RenderTarget;

	/// @brief
	///	渲染命令
	/// @par
	///		记录一个渲染的原子操作，方便渲染器对其进行排序
	class RenderCommand
	{
	public:
		unsigned int	renderOrder;		///< 渲染顺序
		// TODO: 这里需要添加一个光照设置方法
		SceneObject*	sceneObj;			///< 场景对象，用于调用设置光照的方法
		BaseMaterial*	material;			///< 材质
		IVertexBuffer*	vertexBuffer;		///< 顶点缓冲
		IIndexBuffer*	indexBuffer;		///< 索引缓冲
		PrimitiveType	primType;			///< 多边形类型
		Matrix4			transform;			///< 世界变换

		RenderCommand();
	};

	// 用于引擎的渲染器，通过插件调用API函数实现绘图功能
	class Renderer : public Singleton<Renderer>
	{
		friend class Singleton<Renderer>;
	public:

		// 初始化渲染器
		bool Initialize(RenderWindowParam* windowParam);

		// 关闭渲染器
		void Shutdown();

		int GetMaxLightCount() const { return m_MaxLightCount; }

		void SetupLight(int index, ILight* light);

		// 设置/获取渲染器当前是否激活
		inline void SetActive(bool active) { m_Active = active; }
		inline bool GetActive() const { return m_Active; }

		// 设置/获取渲染器当前使用的渲染设备
		inline void SetRenderDevice(IRenderDevice* device) { m_RenderDevice = device; }
		inline IRenderDevice* GetRenderDevice() { return m_RenderDevice; }

		// 修改渲染窗体尺寸
		void ResizeRenderWindow(unsigned int width = 0, unsigned int height = 0);

		void SetAmbientColor(const Color4f& color);
		Color4f GetAmbientColor() const;

		// 清除屏幕缓冲
		void ClearBuffer(bool color = true, bool depth = true, bool stencil = false);

		// 设置清屏颜色
		void SetClearColor(const Color4f& color);
		
		// 矩阵
		void SetModelMatrix(const Matrix4& modelMat);
		void SetViewMatrix(const Matrix4& viewMat);
		void SetProjectionMatrix(const Matrix4& projMat);
		const Matrix4& GetModelMatrix() const;
		const Matrix4& GetViewMatrix() const;
		const Matrix4& GetProjectionMatrix() const;

		// 指定视口范围
		void SetViewport(int left, int bottom, unsigned int width, unsigned int height);

		// 开始渲染，不指定RT时渲染到后台缓冲
		void BeginRender(RenderTarget* rt = NULL);
		void EndRender();

		// 更新屏幕缓冲
		void SwapBuffer();

		/// @brief
		///	提交渲染命令
		/// @param cmd
		///		渲染命令
		void SubmitRenderCommand(const RenderCommand& cmd);

		/// @brief
		///	清除渲染命令列表
		/// @remarks
		///		当完成一次渲染命令的调用以后，调用这个方法
		void ClearRenderCommand();

		/// @brief
		///	将渲染命令列表中的内容依次渲染
		void CommandRender();

		// 渲染多边形
		void RenderPrimitives(IVertexBuffer* vbuffer, PrimitiveType type);
		void RenderPrimitives(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, PrimitiveType type);

		void RenderScreenQuad(Material* mat);

		// 指定渲染材质
		// 这个方法将会根据材质设置渲染器的材质及纹理绘制状态
		void SetupMaterial(Material* material);

		// 指定渲染纹理状态
		void BindTextureRenderState(IDeviceTexture* deviceTex, TextureRenderState* texState = NULL, unsigned int texUnit = 0);

		// 生成一个渲染目标
		IRenderTarget* BuildRenderTarget();

		// 生成一个2D设备纹理
		IDeviceTexture* BuildTexture2D();

		// 生成一个立方体纹理
		IDeviceTexture* BuildCubeTexture();

		// 顶点缓冲与索引缓冲
		IVertexBuffer* BuildVertexBuffer();
		IIndexBuffer* BuildIndexBuffer();

		// 读取shader
		IGpuProgram* LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type);

		// 读取特效材质文件
		IEffect* LoadEffect(const String& filename);

		void SetOverrideMaterial(BaseMaterial* mat);

		// TODO: 这些函数临时使用原有方式实现，逐渐替换
		void ToggleDepthTest(bool enable);
		void ToggleLighting(bool enable);
	private:
		Renderer();
	private:
		IRenderDevice*		m_RenderDevice;

		unsigned int		m_WindowWidth,
							m_WindowHeight;

		Matrix4				m_ModelMatrix;
		Matrix4				m_ViewMatrix;
		Matrix4				m_ProjMatrix;

		int					m_MaxLightCount;
		bool				m_Active;
		Color4f				m_AmbientColor;

		IVertexBuffer*		m_ScreenQuadVB;

		typedef std::vector<RenderCommand>	RenderCmdList;
		RenderCmdList		m_RenderCmdList;

		BaseMaterial*		m_OverrideMaterial;			///< 强制使用的材质，定义了这个材质以后将强制CommandRender渲染的对象使用这个材质进行渲染
	};

}

#endif
