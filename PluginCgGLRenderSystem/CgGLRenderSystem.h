//-----------------------------------------------------------------------------------
/// CgGLRenderSystem.h Cg OpenGL��Ⱦϵͳ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _CGGLRENDERSYSTEM_H
#define _CGGLRENDERSYSTEM_H

#include "IRenderer.h"

#include <windows.h>
#include "GL/glew.h"

#include <Cg/cg.h>    /* Can't include this?  Is Cg Toolkit installed! */
#include <Cg/cgGL.h>

struct ProgramId
{
	string filename;
	string entry;

	bool operator<(const ProgramId& rhs)
	{
		return (filename<rhs.filename) ? (true) : (entry<rhs.entry);
	}
};

enum CgProgramType
{
	CG_VERTEX_PROGRAM,
	CG_FRAGMENT_PROGRAM,
};

typedef map<ProgramId, CGprogram>	CgProgramsMap;

class CgGLRenderSystem : public IRenderer
{
public:
	CgGLRenderSystem();
	~CgGLRenderSystem();

	// ָ������
	//   ��ʹ��CreateRenderWindow���ֶ�ָ��������Խ���Ⱦ����Ϊһ�����еĴ���
	//   ����Ƕ�������������

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

	// ������Ⱦ���ڳߴ磬������ͶӰ�����ݺ��
	//   ʹ��Ĭ�ϲ�������ʱ��������ͶӰ�����ݺ��
	void ResizeRenderWindow(unsigned int width = 0, unsigned int height = 0);

	void SetProjectionMode(ProjectionMatrixMode mode);

	void ClearBuffer(bool color, bool depth, bool stencil);
	void ToggleDepthTest(bool enable);
	void ToggleLighting(bool enable);

	void BeginRender();
	void EndRender(bool swapBuffer);

	// ���ݶ�������������Ⱦ����
	void RenderMesh(const float* vertexArray,
					const float* normalArray,
					const float* textureCoordArray,
					unsigned int faceNumber,
					const unsigned int* faceArray,
					Material* material,
					//ILight* light,
					const Matrix4& transform);

	void RenderAABB(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color, const Matrix4& transform);
	void RenderSphere(const Vector3f& point, float radius, const Color4f& color, unsigned int segment);
	void RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color);

	void RenderScreenQuad(float left, float top, float right, float bottom, ITexture* texture);

	// ��Ļ����
	void RenderScreenQuad(int x1, int y1, int x2, int y2, ITexture* texture, const Color4f& color);

	void SetAmbientColor(const Color4f& color);
	const Color4f GetAmbientColor();

	ITexture* BuildTexture(const string& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);

	// ������Ӱӳ���õ���ͼ for test only
	ITexture* BuildDepthTexture(const string& textureName, unsigned int width, unsigned int height);

	ITexture* GetTexture(const string& textureName);

	int GetMaxLightsNumber();

	void SetupLight(int index, ILight* light);


	CGprogram LoadProgramFromFile(const string& filename, const string entry, CgProgramType type);
protected:

	// ����OpenGL����
	//   ��Matrix4ת��ΪOpenGL��ʹ�õ�float[16]
	void BuildGLMatrix(const Matrix4& mat, float* glMat);

	void BindTexture(ITexture* texture, const Matrix4* matModel=NULL);
	GLint GetFilterType(int type);
	void SetupMaterial(Material* material, const Matrix4* matModel=NULL);

	bool UnloadTexture(const string& textureName);
	void UnloadAllTextures();

	void CheckCgError();

protected:
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hRC;

	CGcontext		m_CgContext;
	CGprofile		m_CgVertexProfile,
					m_CgFragmentProfile;
	//CGprogram		m_CgVertexProgram,
	//				m_CgFragmentProgram;

	CgProgramsMap	m_CgPrograms;

	unsigned int	m_WindowWidth;
	unsigned int	m_WindowHeight;
	//unsigned int	m_Bits;

	bool			m_Active;

	Matrix4			m_ViewMatrix;
	Matrix4			m_ProjMatrix;

	Frustum*		m_Frustum;

	typedef map<string, ITexture*>	TextureList;
	TextureList		m_TextureList;
};

extern "C" __declspec(dllexport) IRenderer* CreateRenderSystem();

#endif
