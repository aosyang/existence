//-----------------------------------------------------------------------------------
/// GLRenderer.h OpenGL��Ⱦ��
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

	// ������Ⱦ���ڳߴ磬������ͶӰ�����ݺ��
	//   ʹ��Ĭ�ϲ�������ʱ��������ͶӰ�����ݺ��
	void ResizeRenderWindow(unsigned int width = 0, unsigned int height = 0);

	// ָ������
	//   ��ʹ��CreateRenderWindow���ֶ�ָ��������Խ���Ⱦ����Ϊһ�����еĴ���
	//   ����Ƕ�������������
	inline void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
	inline HWND GetHwnd() { return m_hWnd; }

	void SetActive(bool active) { m_Active = active; }
	bool GetActive() { return m_Active; }

	//RenderList& GetRenderList() { return m_RenderList; }
	void AddRenderObject(RenderObject* ro);
	void RemoveRenderObject(RenderObject* ro);

	bool Initialize();

	void SetViewMatrix(const Matrix4& mat) { m_ViewMatrix = mat; }

	// ��Ⱦ����
	//   ����ֵΪfalse˵����Ⱦ����δ����
	bool Render();

	// Win32��Ϣ�ص�����
	//static WNDPROC CALLBACK WndProc(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM	lParam);

protected:

	// ����OpenGL����
	//   ��Matrix4ת��ΪOpenGL��ʹ�õ�float[16]
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
