//-----------------------------------------------------------------------------------
/// IRenderer.h ��Ⱦ���ӿڣ��ṩ��Ⱦ���Ĺ�������
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
/// ��Ⱦ���ӿ�
/// 
/// ������������Ⱦ������Ҫʹ�õĹ�������
/// 
/// \remarks
/// ������Ⱦ������̳�������ӿ�
//-----------------------------------------------------------------------------------
class IRenderer
{
public:
	virtual ~IRenderer() {}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// �趨��Ⱦ�����ڼ���/�Ǽ���״̬
	/// 
	/// \param active
	/// trueΪ������Ⱦ�����falseΪ������Ⱦ���Ǽ���
	/// 
	/// \remarks
	/// �������л�����̨ʱ�򣬷�ֹ��Ⱦ���ں�̨��Ⱦ����ռ��CPU��ʹ���������֪ͨ��Ⱦ��Ŀǰ�ļ���״̬
	//-----------------------------------------------------------------------------------
	virtual void SetActive(bool active) = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ��ȡ��Ⱦ���ļ���״̬
	/// 
	/// \returns
	/// true��ʾ��Ⱦ��Ŀǰ���ڼ���״̬����֮��Ǽ���
	//-----------------------------------------------------------------------------------
	virtual bool GetActive() = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ָ��Gpu������ļ���
	/// 
	/// \param filename
	/// ����ļ���
	/// 
	/// ָ���Ժ󽫱������gpu�������
	/// 
	/// \remarks
	/// �����������Ҫʹ��Gpu���ʱʹ�ã�����ļ������գ�����������κβ��
	//-----------------------------------------------------------------------------------
	virtual void SetGpuPluginName(const String& filename) = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ��ʼ����Ⱦ��
	/// 
	/// \param windowParam
	/// ������Ϣ����������ߴ磬��ɫ����Լ�������
	/// 
	/// \returns
	/// true��ʾ��Ⱦ���ɹ���ʼ��
	/// 
	/// \remarks
	/// ��Ⱦ����ص�һ���Գ�ʼ���������������
	//-----------------------------------------------------------------------------------
	virtual bool Initialize(RenderWindowParam* windowParam) = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// �ر���Ⱦ��
	/// 
	/// ����ͨ����Ⱦ����������Դ���ᱻ�ͷ�
	/// 
	/// \remarks
	/// ȷ������Ⱦ���ر�ǰ��ɾ������ʹ��������Ⱦ����������Դ�Ķ��󣬷�����ܵ��±���
	//-----------------------------------------------------------------------------------
	virtual void Shutdown() = 0;

	virtual const String GetFeatureString() const = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ����������ɫ
	/// 
	/// \param color
	/// ������ɫ
	/// 
	/// �����ɫ������ʹ�õ���ɫ
	//-----------------------------------------------------------------------------------
	virtual void SetClearColor(const Color4f& color) = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// �����ӿڳߴ�
	/// 
	/// \param left
	/// �ӿ��������
	/// 
	/// \param bottom
	/// �ӿڵ׶�����
	/// 
	/// \param width
	/// �ӿڿ��(��Ϊ0��ʾʹ�ô��ڿ��)
	/// 
	/// \param height
	/// �ӿڸ߶�(��Ϊ0��ʾʹ�ô��ڸ߶�)
	/// 
	/// \remarks
	/// �ӿڵ�����ԭ��λ����Ļ���·����������Ϸ�
	//-----------------------------------------------------------------------------------
	virtual void SetViewport(int left, int bottom, unsigned int width, unsigned int height) = 0;

	//virtual void SetViewMatrix(const Matrix4& mat) = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ������Ⱦ�����Ӿ���
	/// 
	/// \returns
	/// �Ӿ��������
	/// 
	/// \remarks
	/// ����ֵΪ�Ӿ�������ã�����ֱ�Ӹ�ֵ���޸��Ӿ���
	//-----------------------------------------------------------------------------------
	virtual Matrix4& ViewMatrix() = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ������Ⱦ����ͶӰ����
	/// 
	/// \returns
	/// ͶӰ���������
	/// 
	/// \remarks
	/// ����ֵΪͶӰ��������ã�����ֱ�Ӹ�ֵ���޸�ͶӰ����
	//-----------------------------------------------------------------------------------
	virtual Matrix4& ProjectionMatrix() = 0;

	//virtual void NotifyUpdateProjectionMatrix() = 0;

	// ������Ⱦ���ڳߴ磬������ͶӰ�����ݺ��
	//   ʹ��Ĭ�ϲ�������ʱ��������ͶӰ�����ݺ��
	virtual void ResizeRenderWindow(unsigned int width = 0, unsigned int height = 0) = 0;

	virtual void SetProjectionMode(ProjectionMatrixMode mode) = 0;

	virtual void ClearBuffer(bool color = true, bool depth = true, bool stencil = false) = 0;

	// ״̬�л�
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

	// TODO: �������������Ϊ��������Mipmap��
	virtual ITexture* BuildTexture(const String& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data) = 0;
	virtual ITexture* BuildCubeTexture(const String& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data[6]) = 0;
	virtual ITexture* BuildDepthTexture(const String& textureName, unsigned int width, unsigned int height) = 0;

	virtual ITexture* GetTexture(const String& textureName) = 0;

	// Shaders

	// ��ȡһ��Shader
	virtual IGpuProgram* LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type) = 0;

	// ��ȡһ������Shader
	virtual IGpuProgram* GetGpuProgram(const String& filename, const String& entry, GpuProgramType type) = 0;

	// Lighting

	virtual int GetMaxLightsNumber() = 0;

	virtual void SetupLight(int index, ILight*) = 0;

	// Vertex Buffer

	virtual IVertexBuffer* BuildVertexBuffer() = 0;

	// Render Target

	// ������ȾĿ��
	virtual IRenderTarget* CreateRenderTarget() = 0;
	virtual void SetRenderTarget(IRenderTarget* rt) = 0;
};

typedef IRenderer*(*RenderSystemFactoryCreateFunc)();

#endif
