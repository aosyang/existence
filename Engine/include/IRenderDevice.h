//-----------------------------------------------------------------------------------
/// IRenderDevice.h ��Ⱦ���ӿڣ��ṩ��Ⱦ���Ĺ�������
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
	/// ��Ⱦ���ӿ�
	/// 
	/// ������������Ⱦ������Ҫʹ�õĹ�������
	/// 
	/// \remarks
	/// ������Ⱦ������̳�������ӿ�
	//-----------------------------------------------------------------------------------
	class IRenderDevice
	{
	public:
		virtual ~IRenderDevice() {}

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

		// ָ����ͼ����
		virtual void SetViewMatrix(const Matrix4& viewMat) = 0;

		// ָ��ͶӰ����
		virtual void SetProjectionMatrix(const Matrix4& projMat) = 0;

		virtual void ClearBuffer(bool color = true, bool depth = true, bool stencil = false) = 0;

		// ״̬�л�
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

		// TODO: �������������Ϊ��������Mipmap��

		// ����һ������
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

		// ��ȡһ��Shader
		virtual IGpuProgram* LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type) = 0;

		// ��ȡһ������Shader
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

		// ������ȾĿ��
		virtual IRenderTarget* CreateRenderTarget() = 0;
		virtual void BindRenderTarget(IRenderTarget* rt) = 0;
		virtual void UnbindRenderTarget() = 0;
	};

	typedef IRenderDevice*(*RenderSystemFactoryCreateFunc)();
}

#endif
