#ifndef _RENDERER_H
#define _RENDERER_H

#include "Singleton.h"
#include "IRenderDevice.h"
#include "EString.h"

namespace Gen
{

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
		void SetViewMatrix(const Matrix4& viewMat);
		void SetProjectionMatrix(const Matrix4& projMat);
		const Matrix4& GetViewMatrix() const;
		const Matrix4& GetProjectionMatrix() const;

		// 指定视口范围
		void SetViewport(int left, int bottom, unsigned int width, unsigned int height);

		void BeginRender();
		void EndRender();

		// 更新屏幕缓冲
		void SwapBuffer();

		// 渲染多边形
		void RenderPrimitives(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, const Matrix4& transform);

		// 指定渲染材质
		// 这个方法将会根据材质设置渲染器的材质及纹理绘制状态
		void SetupMaterial(Material* material);

		// 指定渲染纹理状态
		void BindTextureRenderState(const TextureRenderState& texState, unsigned int texUnit = 0);

		// 生成一个2D设备纹理
		IDeviceTexture* BuildTexture2D();

		// 生成一个立方体纹理
		IDeviceTexture* BuildCubeTexture();

		// 顶点缓冲与索引缓冲
		IVertexBuffer* BuildVertexBuffer();
		IIndexBuffer* BuildIndexBuffer();

		// TODO: 这些函数临时使用原有方式实现，逐渐替换
		void ToggleDepthTest(bool enable);
		void ToggleLighting(bool enable);

		void RenderBox(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), const Matrix4& transform = Matrix4::IDENTITY);
		void RenderSphere(const Vector3f& point, float radius, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), unsigned int segment = 18);
		void RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f));
	private:
		Renderer();
	private:
		IRenderDevice*		m_RenderDevice;

		unsigned int		m_WindowWidth,
							m_WindowHeight;

		Matrix4				m_ViewMatrix;
		Matrix4				m_ProjMatrix;

		int					m_MaxLightCount;
		bool				m_Active;
		Color4f				m_AmbientColor;
	};

}

#endif
