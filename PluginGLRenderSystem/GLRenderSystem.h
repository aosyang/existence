//-----------------------------------------------------------------------------------
/// GLRenderSystem.h OpenGL渲染系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _GLRENDERSYSTEM_H
#define _GLRENDERSYSTEM_H

#include "IRenderer.h"
#include "GLHardwareFeature.h"

#include <windows.h>
#include "GL/glew.h"

typedef IVertexBuffer*(*FactoryCreateVertexBufferFunc)();

class GLRenderer : public IRenderer
{
public:
	GLRenderer();
	~GLRenderer();

	// 指定窗体
	//   不使用CreateRenderWindow而手动指定窗体可以将渲染器设为一个已有的窗体
	//   用于嵌入其他窗体程序

	RenderWindowHandle GetRenderWindowHandle();
	//inline void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
	//inline HWND GetHwnd() { return m_hWnd; }
	//void* GetHwnd();

	void SetActive(bool active) { m_Active = active; }
	bool GetActive() { return m_Active; }

	bool Initialize(RenderWindowParam* windowParam);
	void Shutdown();

	void SetClearColor(const Color4f& color);

	//void SetViewMatrix(const Matrix4& mat) { m_ViewMatrix = mat; }
	Matrix4& ViewMatrix() { return m_ViewMatrix; }
	Matrix4& ProjectionMatrix() { return m_ProjMatrix; }

	//void NotifyUpdateProjectionMatrix();

	void SetFrustum(Frustum* frustum);
	Frustum* GetFrustum() const { return m_Frustum; }

	// 调整渲染窗口尺寸，并重设投影矩阵纵横比
	//   使用默认参数调用时将仅重置投影矩阵纵横比
	void ResizeRenderWindow(unsigned int width = 0, unsigned int height = 0);

	void SetProjectionMode(ProjectionMatrixMode mode);

	void ClearBuffer(bool color, bool depth, bool stencil);

	void ToggleTexture(bool enable, unsigned int index = 0);
	void ToggleDepthWriting(bool enable);
	void ToggleDepthTest(bool enable);
	void ToggleLighting(bool enable);
	void ToggleBlend(bool enable);

	void BeginRender();
	void EndRender(bool swapBuffer);

	void RenderVertexBuffer(IVertexBuffer* vbuffer, Material* material, const Matrix4& transform);

	void RenderAABB(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color, const Matrix4& transform);
	void RenderSphere(const Vector3f& point, float radius, const Color4f& color, unsigned int segment);
	void RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color);

	void RenderScreenQuad(float left, float top, float right, float bottom, ITexture* texture);

	// 屏幕坐标
	void RenderScreenQuad(int x1, int y1, int x2, int y2, ITexture* texture, const Color4f& color);

	void SetAmbientColor(const Color4f& color);
	const Color4f GetAmbientColor();

	ITexture* BuildTexture(const string& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);
	ITexture* BuildCubeTexture(const string& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data[6]);

	// 构造阴影映射用的贴图 for test only
	ITexture* BuildDepthTexture(const string& textureName, unsigned int width, unsigned int height);

	ITexture* GetTexture(const string& textureName);

	int GetMaxLightsNumber();

	void SetupLight(int index, ILight* light);

	IVertexBuffer* BuildVertexBuffer();

	// Render target

protected:

	// 创建OpenGL矩阵
	//   从Matrix4转换为OpenGL所使用的float[16]
	void BuildGLMatrix(const Matrix4& mat, float* glMat);

	void BindTextureRenderState(const texRenderState_t& texState);
	GLint GetFilterType(int type);
	GLint GetBlendFactor(int factor);
	GLint GetEnvMode(int mode);
	void SetupMaterial(Material* material);

	bool UnloadTexture(const string& textureName);
	void UnloadAllTextures();
protected:
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hRC;

	unsigned int	m_WindowWidth;
	unsigned int	m_WindowHeight;
	//unsigned int	m_Bits;

	bool			m_Active;

	Matrix4			m_ViewMatrix;
	Matrix4			m_ProjMatrix;

	Frustum*		m_Frustum;

	typedef map<string, ITexture*>	TextureList;
	TextureList		m_TextureList;

	bool			m_DepthWriting;

	GLHardwareFeature	m_HardwareFeature;

	FactoryCreateVertexBufferFunc	m_VertexBufferFactoryFunc;
};

extern "C" __declspec(dllexport) IRenderer* CreateRenderSystem();

#endif
