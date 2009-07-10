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
#include "Platform.h"			// TODO: Remove Platform::WndProc from platform.h
#include "Material.h"
#include "Matrix4.h"
#include "Color4f.h"
#include "Frustum.h"
#include "ILight.h"
#include "IRenderTarget.h"
#include "IVertexBuffer.h"

#include <map>
#include <string>

using namespace std;

enum ProjectionMatrixMode
{
	PROJECTION_MODE_PERSPECTIVE,
	PROJECTION_MODE_ORTHO
};

class IRenderer
{
public:
	virtual ~IRenderer() {}

	virtual void SetActive(bool active) = 0;
	virtual bool GetActive() = 0;

	virtual bool Initialize(RenderWindowParam* windowParam) = 0;
	virtual void Shutdown() = 0;

	virtual void SetClearColor(const Color4f& color) = 0;

	//virtual void SetViewMatrix(const Matrix4& mat) = 0;
	virtual Matrix4& ViewMatrix() = 0;
	virtual Matrix4& ProjectionMatrix() = 0;

	//virtual void NotifyUpdateProjectionMatrix() = 0;

	virtual void SetFrustum(Frustum* frustum) = 0;
	virtual Frustum* GetFrustum() const = 0;

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
	virtual void EndRender(bool swapBuffer = false) = 0;

	virtual void RenderVertexBuffer(IVertexBuffer* vbuffer, Material* material, const Matrix4& transform) = 0;

	virtual void RenderAABB(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), const Matrix4& transform = Matrix4::IDENTITY) = 0;
	virtual void RenderSphere(const Vector3f& point, float radius, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), unsigned int segment = 18) = 0;
	virtual void RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f)) = 0;

	virtual void RenderScreenQuad(float left, float top, float right, float bottom, ITexture* texture) = 0;
	virtual void RenderScreenQuad(int x1, int y1, int x2, int y2, ITexture* texture, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f)) = 0;

	// TODO: Implement these...
	//virtual void RenderAnimatedMesh();
	//virtual void RenderIndexedVertexData();
	//virtual void RenderScreenQuad(float x1, float y1, float x2, float y2);

	virtual void SetAmbientColor(const Color4f& color) = 0;
	virtual const Color4f GetAmbientColor() = 0;

	// Textures

	// TODO: 添加纹理属性作为参数，如Mipmap等
	virtual ITexture* BuildTexture(const string& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data) = 0;
	virtual ITexture* BuildCubeTexture(const string& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data[6]) = 0;
	virtual ITexture* BuildDepthTexture(const string& textureName, unsigned int width, unsigned int height) = 0;

	virtual ITexture* GetTexture(const string& textureName) = 0;

	// Lighting

	virtual int GetMaxLightsNumber() = 0;

	virtual void SetupLight(int index, ILight*) = 0;

	// Vertex Buffer

	virtual IVertexBuffer* BuildVertexBuffer() = 0;

	// Render Target
	//virtual void SetRenderTarget(IRenderTarget* rt) = 0;
	//virtual IRenderTarget* CreateRenderTarget(unsigned int width, unsigned int height) = 0;
};

typedef IRenderer*(*RenderSystemFactoryCreateFunc)();

#endif
