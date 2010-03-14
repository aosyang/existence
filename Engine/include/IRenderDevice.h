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

	/// @brief
	/// 渲染器接口
	/// @remarks
	///		这个接口包括了所有渲染器所需要使用的公共方法，所有渲染器必须继承自这个接口
	class IRenderDevice
	{
	public:
		virtual ~IRenderDevice() {}


		/// @brief
		/// 指定Gpu插件的文件名
		/// @param filename
		///		插件文件名
		/// @remarks
		///		这个方法在需要使用Gpu插件时使用，如果文件名留空，将不会加载任何插件
		virtual void SetGpuPluginName(const String& filename) = 0;

		/// @brief
		/// 初始化渲染器
		/// @param windowParam
		///		窗体信息，包括窗体尺寸，颜色深度以及窗体句柄
		/// @returns
		///		true表示渲染器成功初始化，否则返回false
		/// @remarks
		///		渲染器相关的一次性初始化操作在这里完成
		virtual bool Initialize(RenderWindowParam* windowParam) = 0;


		/// @brief
		/// 关闭渲染器
		/// @remarks
		///		释放渲染器的设备
		virtual void Shutdown() = 0;

		/// @brief
		///	获取硬件特性字串
		virtual const String GetFeatureString() const = 0;


		/// @brief
		/// 设置颜色缓冲区的清屏颜色
		/// @param color
		///		清屏颜色
		/// @remarks
		///		之后清除屏幕缓冲区时将使用选定颜色进行填充
		virtual void SetClearColor(const Color4f& color) = 0;


		/// @brief
		/// 设置视口尺寸
		/// @param left
		///		视口左端坐标
		/// @param bottom
		///		视口底端坐标
		/// @param width
		///		视口宽度(设为0表示使用窗口宽度)
		/// @param height
		///		视口高度(设为0表示使用窗口高度)
		/// @remarks
		///		视口的坐标原点位于屏幕左下方，不是左上方
		virtual void SetViewport(int left, int bottom, unsigned int width, unsigned int height) = 0;

		/// @brief
		/// 指定渲染设备的视图矩阵
		virtual void SetViewMatrix(const Matrix4& viewMat) = 0;

		/// @brief
		/// 指定渲染设备的投影矩阵
		virtual void SetProjectionMatrix(const Matrix4& projMat) = 0;

		/// @brief
		/// 清除缓冲区
		/// @param color
		///		是否清除颜色缓冲区
		///	@param depth
		///		是否清除深度缓冲区
		/// @param stencil
		///		是否清除模板缓冲区
		virtual void ClearBuffer(bool color = true, bool depth = true, bool stencil = false) = 0;

		// 状态切换

		/// @brief
		///	开启/关闭纹理层
		/// @param enable
		///		开启或关闭纹理层
		/// @param index
		///		所要操作的纹理层
		virtual void ToggleTexture(bool enable, unsigned int index = 0) = 0;

		/// @brief
		/// 开启/关闭深度缓冲写入操作
		/// @param enable
		///		开启或关闭深度缓冲写入
		virtual void ToggleDepthWriting(bool enable) = 0;

		/// @brief
		/// 开启/关闭深度缓冲测试
		/// @param enable
		///		开启或关闭深度缓冲测试
		/// @remarks
		///		关闭了深度缓冲测试，渲染对象将不会因为被遮挡而拒绝绘制，绘制结果仅仅由绘制顺序决定
		virtual void ToggleDepthTest(bool enable) = 0;

		/// @brief
		/// 开启/关闭光照
		/// @param enable
		///		开启或关闭光照
		/// @note
		///		这个设定仅仅对应固定管线渲染
		virtual void ToggleLighting(bool enable) = 0;


		/// @brief
		/// 开启/关闭Alpha测试
		/// @param enable
		///		开启或关闭Alpha测试
		/// @remarks
		///		Alpha测试的结果决定象素是否需要写入缓冲区
		virtual void ToggleAlphaTest(bool enable) = 0;

		/// @brief
		/// 设置Alpha参考值
		/// @param ref
		///		要设置的Alpha参考值
		/// @remarks
		///		每个要写入屏幕缓冲区的象素都将其Alpha值与这个参考值比较，根据判断函数，不满足条件的象素将不会写入缓冲区
		virtual void SetAlphaReference(float ref) = 0;

		/// @brief
		/// 开启/关闭颜色混合
		/// @param blend
		///		开启或关闭颜色混合
		/// @remarks
		///		颜色混合允许写入缓冲区的颜色与缓冲区中已有颜色进行不同方式的混合，并将新的结果写入缓冲区
		virtual void ToggleBlending(bool blend) = 0;

		/// @brief
		/// 设置颜色混合因子
		/// @param src
		///		源混合因子(与要写入的象素进行运算)
		/// @param dst
		///		目标混合因子(与缓冲区中已有的象素进行运算)
		virtual void SetBlendFactor(BlendFactor src, BlendFactor dst) = 0;


		/// 开始一帧的绘制操作
		virtual void BeginRender() = 0;
		/// 结束一帧的绘制操作
		virtual void EndRender() = 0;
		/// 完成绘制，交换缓冲区
		virtual void SwapBuffer() = 0;

		/// @brief
		/// 绑定纹理渲染状态
		/// @param texState
		///		要绑定的纹理渲染状态信息
		/// @param texUnit
		///		要绑定到的纹理单元序号
		/// @remarks
		///		纹理渲染状态指定了纹理如何采样、纹理坐标如何生成等信息
		///		调用了这个方法以后，接下来的纹理操作将会始终针对最后一次指定的纹理单元，直到手动修改纹理单元
		virtual void BindTextureRenderState(const TextureRenderState& texState, unsigned int texUnit = 0) = 0;

		/// @brief
		///	使用顶点及索引缓冲渲染几何体
		///	@param vbuffer
		///		渲染使用的顶点缓冲
		/// @param ibuffer
		///		渲染使用的索引缓冲
		/// @param transform
		///		渲染使用的模型矩阵
		/// @remarks
		///		实现这个方法时需要在调用结束之前恢复模型矩阵
		virtual void RenderVertexBuffer(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, const Matrix4& transform) = 0;


		/// @brief
		/// 渲染线框方盒
		/// @param vMin
		///		方盒x、y、z最小值构成的向量
		/// @param vMax
		///		方盒x、y、z最大值构成的向量
		/// @param transform
		///		方盒的世界变换矩阵
		virtual void RenderBox(const Vector3f& vMin, const Vector3f& vMax, const Matrix4& transform = Matrix4::IDENTITY) = 0;

		/// @brief
		///	渲染线框球体
		///	@param point
		///		球心坐标
		/// @param radius
		///		球体半径
		/// @param segment
		///		绘制球体时的分段数量
		/// @remarks
		///		坐标为世界空间
		virtual void RenderSphere(const Vector3f& point, float radius, unsigned int segment = 18) = 0;

		/// @brief
		///	渲染线段
		/// @param begin
		///		线段起始坐标
		/// @param end
		///		线段终止坐标
		/// @remarks
		///		坐标为世界空间
		virtual void RenderLine(const Vector3f& begin, const Vector3f& end) = 0;


		/// @brief
		///	指定环境光颜色
		/// @param color
		///		环境光颜色
		/// @remarks
		///		指定以后将会影响每一次渲染时材质的环境光效果
		virtual void SetAmbientColor(const Color4f& color) = 0;

		// Textures

		// TODO: 添加纹理属性作为参数，如Mipmap等

		// 生成一个纹理

		/// @brief
		///	生成2D设备纹理
		/// @returns
		///		返回设备纹理指针
		virtual IDeviceTexture* BuildTexture() = 0;

		/// @brief
		/// 生成立方体设备纹理
		/// @returns
		///		返回设备纹理指针
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

		/// @brief
		/// 生成顶点缓冲
		/// @returns
		///		返回新的顶点缓冲指针
		virtual IVertexBuffer* BuildVertexBuffer() = 0;

		/// @brief
		///	生成索引缓冲
		/// @returns
		///		返回新的索引缓冲指针
		virtual IIndexBuffer* BuildIndexBuffer() = 0;

		// Render Target

		// 创建渲染目标

		/// @brief
		///	创建渲染目标
		/// @returns
		///		返回新的渲染目标指针
		virtual IRenderTarget* CreateRenderTarget() = 0;

		/// @brief
		/// 指定渲染到一个渲染目标
		/// @param rt
		///		要渲染到的渲染目标
		virtual void BindRenderTarget(IRenderTarget* rt) = 0;

		/// @brief
		///	取消渲染到当前的渲染目标，渲染回后台缓冲
		virtual void UnbindRenderTarget() = 0;
	};

	typedef IRenderDevice*(*RenderSystemFactoryCreateFunc)();
}

#endif
