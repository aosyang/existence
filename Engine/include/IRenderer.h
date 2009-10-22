//-----------------------------------------------------------------------------------
/// IRenderer.h 渲染器接口，提供渲染器的公共方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _IRENDERER_H
#define _IRENDERER_H

#include "ITexture.h"
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
class IRenderer
{
public:
	virtual ~IRenderer() {}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 设定渲染器处于激活/非激活状态
	/// 
	/// \param active
	/// true为设置渲染器激活，false为设置渲染器非激活
	/// 
	/// \remarks
	/// 当程序被切换到后台时候，防止渲染器在后台渲染继续占用CPU，使用这个方法通知渲染器目前的激活状态
	//-----------------------------------------------------------------------------------
	virtual void SetActive(bool active) = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 获取渲染器的激活状态
	/// 
	/// \returns
	/// true表示渲染器目前处于激活状态，反之则非激活
	//-----------------------------------------------------------------------------------
	virtual bool GetActive() = 0;

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

	//virtual void SetViewMatrix(const Matrix4& mat) = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 访问渲染器的视矩阵
	/// 
	/// \returns
	/// 视矩阵的引用
	/// 
	/// \remarks
	/// 返回值为视矩阵的引用，可以直接赋值以修改视矩阵
	//-----------------------------------------------------------------------------------
	virtual Matrix4& ViewMatrix() = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 访问渲染器的投影矩阵
	/// 
	/// \returns
	/// 投影矩阵的引用
	/// 
	/// \remarks
	/// 返回值为投影矩阵的引用，可以直接赋值以修改投影矩阵
	//-----------------------------------------------------------------------------------
	virtual Matrix4& ProjectionMatrix() = 0;

	//virtual void NotifyUpdateProjectionMatrix() = 0;

	// 调整渲染窗口尺寸，并重设投影矩阵纵横比
	//   使用默认参数调用时将仅重置投影矩阵纵横比
	virtual void ResizeRenderWindow(unsigned int width = 0, unsigned int height = 0) = 0;

	virtual void SetProjectionMode(ProjectionMatrixMode mode) = 0;

	virtual void ClearBuffer(bool color = true, bool depth = true, bool stencil = false) = 0;

	// 状态切换
	virtual void ToggleTexture(bool enable, unsigned int index = 0) = 0;
	virtual void ToggleDepthWriting(bool enable) = 0;
	virtual void ToggleDepthTest(bool enable) = 0;
	virtual void ToggleLighting(bool enable) = 0;

	virtual void BeginRender() = 0;
	virtual void EndRender() = 0;
	virtual void SwapBuffer() = 0;

	virtual void RenderVertexBuffer(IVertexBuffer* vbuffer, Material* material, const Matrix4& transform) = 0;

	virtual void RenderBox(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), const Matrix4& transform = Matrix4::IDENTITY) = 0;
	virtual void RenderSphere(const Vector3f& point, float radius, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), unsigned int segment = 18) = 0;
	virtual void RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f)) = 0;

	virtual void RenderScreenQuad(float left, float top, float right, float bottom, ITexture* texture, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f)) = 0;
	virtual void RenderScreenQuad(int x1, int y1, int x2, int y2, ITexture* texture, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f)) = 0;

	// TODO: Implement these...
	//virtual void RenderAnimatedMesh();
	//virtual void RenderIndexedVertexData();
	//virtual void RenderScreenQuad(float x1, float y1, float x2, float y2);

	virtual void SetAmbientColor(const Color4f& color) = 0;
	virtual const Color4f GetAmbientColor() = 0;

	// Textures

	// TODO: 添加纹理属性作为参数，如Mipmap等
	virtual ITexture* BuildTexture(const String& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data) = 0;
	virtual ITexture* BuildCubeTexture(const String& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data[6]) = 0;
	virtual ITexture* BuildDepthTexture(const String& textureName, unsigned int width, unsigned int height) = 0;

	virtual ITexture* GetTexture(const String& textureName) = 0;

	// Shaders

	// 读取一个Shader
	virtual IGpuProgram* LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type) = 0;

	// 获取一个已有Shader
	virtual IGpuProgram* GetGpuProgram(const String& filename, const String& entry, GpuProgramType type) = 0;

	// Lighting

	virtual int GetMaxLightsNumber() = 0;

	virtual void SetupLight(int index, ILight*) = 0;

	// Vertex Buffer

	virtual IVertexBuffer* BuildVertexBuffer() = 0;

	// Render Target

	// 创建渲染目标
	virtual IRenderTarget* CreateRenderTarget() = 0;
	virtual void SetRenderTarget(IRenderTarget* rt) = 0;
};

typedef IRenderer*(*RenderSystemFactoryCreateFunc)();

#endif
