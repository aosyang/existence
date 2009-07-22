//-----------------------------------------------------------------------------------
/// D3D9RenderSystem.h Direct3D 9渲染系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _D3D9RENDERSYSTEM_H
#define _D3D9RENDERSYSTEM_H

#include "IRenderer.h"

#include <d3dx9.h>

#include "D3D9VertexBuffer.h"

class D3D9Renderer : public IRenderer
{
	friend class D3D9VertexBuffer;
public:
	D3D9Renderer();
	~D3D9Renderer();


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
	void ToggleLight(bool enable, unsigned int index);

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

	ITexture* BuildTexture(const String& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);
	ITexture* BuildDepthTexture(const String& textureName, unsigned int width, unsigned int height) { return NULL; }

	ITexture* GetTexture(const String& textureName);

	int GetMaxLightsNumber();

	void SetupLight(int index, ILight* light);

	IVertexBuffer* BuildVertexBuffer();

protected:
	void BuildD3DMatrix(const Matrix4& mat, D3DMATRIX& d3dMat);

	void SetupMaterial(Material* material);

	bool UnloadTexture(const String& textureName);
	void UnloadAllTextures();
protected:
	HWND	m_hWnd;

	IDirect3D9*			m_Direct3D;
	IDirect3DDevice9*	m_D3DDevice;

	D3DCOLOR		m_ClearColor;

	unsigned int	m_WindowWidth;
	unsigned int	m_WindowHeight;
	//unsigned int	m_Bits;

	bool			m_Active;

	Matrix4			m_ViewMatrix;
	Matrix4			m_ProjMatrix;

	Frustum*		m_Frustum;

	typedef map<String, ITexture*>	TextureList;
	TextureList		m_TextureList;
};

extern "C" __declspec(dllexport) IRenderer* CreateRenderSystem();

#endif
