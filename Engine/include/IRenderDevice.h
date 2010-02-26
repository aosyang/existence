//-----------------------------------------------------------------------------------
/// IRenderDevice.h 渲染器接口，提供渲染器的公共方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _IRENDERER_H
#define _IRENDERER_H

#include "IDeviceTexture.h"
#include "Material.h"
#include "Matrix4.h"
#include "Color4f.h"
#include "Frustum.h"
#include "ILight.h"
#include "IRenderTarget.h"
#include "IVertexBuffer.h"
#include "EString.h"

#include <map>

using namespace std;

namespace Gen
{
	enum ProjectionMatrixMode
	{
		PROJECTION_MODE_PERSPECTIVE,
		PROJECTION_MODE_ORTHO
	};

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 渲染器接口
	/// 
	/// 包括了所有渲染器所需要使用的公共方法
	/// 
	/// \remarks
	/// 所有渲染器必须继承自这个接口
	//-----------------------------------------------------------------------------------
	class IRenderDevice
	{
	public:
		virtual ~IRenderDevice() {}

		//-----------------------------------------------------------------------------------
		/// \brief
		/// 指定Gpu插件的文件名
		/// 
		/// \param filename
		/// 插件文件名
		/// 
		/// 指定稍后将被载入的gpu插件名称
		/// 
		/// \remarks
		/// 这个方法在需要使用Gpu插件时使用，如果文件名留空，将不会加载任何插件
		//-----------------------------------------------------------------------------------
		virtual void SetGpuPluginName(const String& filename) = 0;

		//-----------------------------------------------------------------------------------
		/// \brief
		/// 初始化渲染器
		/// 
		/// \param windowParam
		/// 窗体信息，包括窗体尺寸，颜色深度以及窗体句柄
		/// 
		/// \returns
		/// true表示渲染器成功初始化
		/// 
		/// \remarks
		/// 渲染器相关的一次性初始化操作在这里完成
		//-----------------------------------------------------------------------------------
		virtual bool Initialize(RenderWindowParam* windowParam) = 0;

		//-----------------------------------------------------------------------------------
		/// \brief
		/// 关闭渲染器
		/// 
		/// 所有通过渲染器创建的资源将会被释放
		/// 
		/// \remarks
		/// 确保在渲染器关闭前先删除所有使用了由渲染器创建的资源的对象，否则可能导致崩溃
		//-----------------------------------------------------------------------------------
		virtual void Shutdown() = 0;

		virtual const String GetFeatureString() const = 0;

		//-----------------------------------------------------------------------------------
		/// \brief
		/// 设置清屏颜色
		/// 
		/// \param color
		/// 清屏颜色
		/// 
		/// 清除颜色缓冲所使用的颜色
		//-----------------------------------------------------------------------------------
		virtual void SetClearColor(const Color4f& color) = 0;

		//-----------------------------------------------------------------------------------
		/// \brief
		/// 设置视口尺寸
		/// 
		/// \param left
		/// 视口左端坐标
		/// 
		/// \param bottom
		/// 视口底端坐标
		/// 
		/// \param width
		/// 视口宽度(设为0表示使用窗口宽度)
		/// 
		/// \param height
		/// 视口高度(设为0表示使用窗口高度)
		/// 
		/// \remarks
		/// 视口的坐标原点位于屏幕左下方，不是左上方
		//-----------------------------------------------------------------------------------
		virtual void SetViewport(int left, int bottom, unsigned int width, unsigned int height) = 0;

		// 指定视图矩阵
		virtual void SetViewMatrix(const Matrix4& viewMat) = 0;

		// 指定投影矩阵
		virtual void SetProjectionMatrix(const Matrix4& projMat) = 0;

		virtual void ClearBuffer(bool color = true, bool depth = true, bool stencil = false) = 0;

		// 状态切换
		virtual void ToggleTexture(bool enable, unsigned int index = 0) = 0;
		virtual void ToggleDepthWriting(bool enable) = 0;
		virtual void ToggleDepthTest(bool enable) = 0;
		virtual void ToggleLighting(bool enable) = 0;
		virtual void ToggleAlphaTest(bool enable) = 0;
		virtual void SetAlphaReference(float ref) = 0;
		virtual void ToggleBlending(bool blend) = 0;
		virtual void SetBlendFactor(BlendFactor src, BlendFactor dst) = 0;

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;
		virtual void SwapBuffer() = 0;

		virtual void BindTextureRenderState(const TextureRenderState& texState, unsigned int texUnit = 0) = 0;

		virtual void RenderVertexBuffer(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, const Matrix4& transform) = 0;

		virtual void RenderBox(const Vector3f& vMin, const Vector3f& vMax, const Matrix4& transform = Matrix4::IDENTITY) = 0;
		virtual void RenderSphere(const Vector3f& point, float radius, unsigned int segment = 18) = 0;
		virtual void RenderLine(const Vector3f& begin, const Vector3f& end) = 0;

		virtual void SetAmbientColor(const Color4f& color) = 0;

		// Textures

		// TODO: 添加纹理属性作为参数，如Mipmap等

		// 生成一个纹理
		virtual IDeviceTexture* BuildTexture() = 0;
		virtual IDeviceTexture* BuildCubeTexture() = 0;
		virtual IDeviceTexture* BuildDepthTexture(const String& textureName, unsigned int width, unsigned int height) = 0;

		//virtual IDeviceTexture* GetTexture(const String& textureName) = 0;

		// Materials
		virtual void SetVertexColor(const Color4f& color) = 0;
		virtual void SetMaterialAmbientColor(const Color4f& color) = 0;
		virtual void SetMaterialDiffuseColor(const Color4f& color) = 0;
		virtual void SetMaterialSpecularColor(const Color4f& color) = 0;
		virtual void SetMaterialEmissiveColor(const Color4f& color) = 0;
		virtual void SetMaterialSpecularLevel(float level) = 0;

		// Shaders

		// 读取一个Shader
		virtual IGpuProgram* LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type) = 0;

		// 获取一个已有Shader
		virtual IGpuProgram* GetGpuProgram(const String& filename, const String& entry, GpuProgramType type) = 0;

		virtual void BindGpuProgram(IGpuProgram* program, GpuProgramType type) = 0;
		virtual void UnbindGpuProgram(GpuProgramType type) = 0;

		// Lighting

		virtual int GetMaxLightsNumber() = 0;

		virtual void SetupLight(int index, ILight*) = 0;

		// Vertex Buffer

		virtual IVertexBuffer* BuildVertexBuffer() = 0;
		virtual IIndexBuffer* BuildIndexBuffer() = 0;

		// Render Target

		// 创建渲染目标
		virtual IRenderTarget* CreateRenderTarget() = 0;
		virtual void BindRenderTarget(IRenderTarget* rt) = 0;
		virtual void UnbindRenderTarget() = 0;
	};

	typedef IRenderDevice*(*RenderSystemFactoryCreateFunc)();
}

#endif
