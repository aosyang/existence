//-----------------------------------------------------------------------------------
/// GLRenderer.h OpenGL渲染器
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _GLRENDERER_H
#define _GLRENDERER_H

#include "Engine.h"
#include <windows.h>

#include "IRenderer.h"
#include "Singleton.h"
#include "RenderList.h"
#include "Matrix4.h"

class GLRenderer : public IRenderer
{
	friend Singleton<GLRenderer>;
public:
	GLRenderer();
	~GLRenderer();

	bool CreateRenderWindow(char* title, unsigned int width, unsigned int height, unsigned int bits, bool fullscreen);
	void DestroyRenderWindow();

	// 调整渲染窗口尺寸，并重设投影矩阵纵横比
	//   使用默认参数调用时将仅重置投影矩阵纵横比
	void ResizeRenderWindow(unsigned int width = 0, unsigned int height = 0);

	// 指定窗体
	//   不使用CreateRenderWindow而手动指定窗体可以将渲染器设为一个已有的窗体
	//   用于嵌入其他窗体程序
	inline void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
	inline HWND GetHwnd() { return m_hWnd; }

	void SetActive(bool active) { m_Active = active; }
	bool GetActive() { return m_Active; }

	//RenderList& GetRenderList() { return m_RenderList; }
	void AddRenderObject(RenderObject* ro);
	void RemoveRenderObject(RenderObject* ro);

	bool Initialize();

	void SetViewMatrix(const Matrix4& mat) { m_ViewMatrix = mat; }

	// 渲染场景
	//   返回值为false说明渲染窗口未激活
	bool Render();

	// Win32消息回调函数
	//static WNDPROC CALLBACK WndProc(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM	lParam);

protected:

	// 创建OpenGL矩阵
	//   从Matrix4转换为OpenGL所使用的float[16]
	void BuildGLMatrix(const Matrix4& mat, float* glMat);

	void BindTexture(ITexture* texture);
protected:
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hRC;

	unsigned int	m_WindowWidth;
	unsigned int	m_WindowHeight;
	unsigned int	m_Bits;

	bool			m_FullScreen;

	char			m_TitleName[256];

	bool			m_Active;

	RenderList		m_RenderList;

	Matrix4			m_ViewMatrix;
};

#endif
