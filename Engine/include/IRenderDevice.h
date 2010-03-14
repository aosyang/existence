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

	/// @brief
	/// ��Ⱦ���ӿ�
	/// @remarks
	///		����ӿڰ�����������Ⱦ������Ҫʹ�õĹ���������������Ⱦ������̳�������ӿ�
	class IRenderDevice
	{
	public:
		virtual ~IRenderDevice() {}


		/// @brief
		/// ָ��Gpu������ļ���
		/// @param filename
		///		����ļ���
		/// @remarks
		///		�����������Ҫʹ��Gpu���ʱʹ�ã�����ļ������գ�����������κβ��
		virtual void SetGpuPluginName(const String& filename) = 0;

		/// @brief
		/// ��ʼ����Ⱦ��
		/// @param windowParam
		///		������Ϣ����������ߴ磬��ɫ����Լ�������
		/// @returns
		///		true��ʾ��Ⱦ���ɹ���ʼ�������򷵻�false
		/// @remarks
		///		��Ⱦ����ص�һ���Գ�ʼ���������������
		virtual bool Initialize(RenderWindowParam* windowParam) = 0;


		/// @brief
		/// �ر���Ⱦ��
		/// @remarks
		///		�ͷ���Ⱦ�����豸
		virtual void Shutdown() = 0;

		/// @brief
		///	��ȡӲ�������ִ�
		virtual const String GetFeatureString() const = 0;


		/// @brief
		/// ������ɫ��������������ɫ
		/// @param color
		///		������ɫ
		/// @remarks
		///		֮�������Ļ������ʱ��ʹ��ѡ����ɫ�������
		virtual void SetClearColor(const Color4f& color) = 0;


		/// @brief
		/// �����ӿڳߴ�
		/// @param left
		///		�ӿ��������
		/// @param bottom
		///		�ӿڵ׶�����
		/// @param width
		///		�ӿڿ��(��Ϊ0��ʾʹ�ô��ڿ��)
		/// @param height
		///		�ӿڸ߶�(��Ϊ0��ʾʹ�ô��ڸ߶�)
		/// @remarks
		///		�ӿڵ�����ԭ��λ����Ļ���·����������Ϸ�
		virtual void SetViewport(int left, int bottom, unsigned int width, unsigned int height) = 0;

		/// @brief
		/// ָ����Ⱦ�豸����ͼ����
		virtual void SetViewMatrix(const Matrix4& viewMat) = 0;

		/// @brief
		/// ָ����Ⱦ�豸��ͶӰ����
		virtual void SetProjectionMatrix(const Matrix4& projMat) = 0;

		/// @brief
		/// ���������
		/// @param color
		///		�Ƿ������ɫ������
		///	@param depth
		///		�Ƿ������Ȼ�����
		/// @param stencil
		///		�Ƿ����ģ�建����
		virtual void ClearBuffer(bool color = true, bool depth = true, bool stencil = false) = 0;

		// ״̬�л�

		/// @brief
		///	����/�ر������
		/// @param enable
		///		������ر������
		/// @param index
		///		��Ҫ�����������
		virtual void ToggleTexture(bool enable, unsigned int index = 0) = 0;

		/// @brief
		/// ����/�ر���Ȼ���д�����
		/// @param enable
		///		������ر���Ȼ���д��
		virtual void ToggleDepthWriting(bool enable) = 0;

		/// @brief
		/// ����/�ر���Ȼ������
		/// @param enable
		///		������ر���Ȼ������
		/// @remarks
		///		�ر�����Ȼ�����ԣ���Ⱦ���󽫲�����Ϊ���ڵ����ܾ����ƣ����ƽ�������ɻ���˳�����
		virtual void ToggleDepthTest(bool enable) = 0;

		/// @brief
		/// ����/�رչ���
		/// @param enable
		///		������رչ���
		/// @note
		///		����趨������Ӧ�̶�������Ⱦ
		virtual void ToggleLighting(bool enable) = 0;


		/// @brief
		/// ����/�ر�Alpha����
		/// @param enable
		///		������ر�Alpha����
		/// @remarks
		///		Alpha���ԵĽ�����������Ƿ���Ҫд�뻺����
		virtual void ToggleAlphaTest(bool enable) = 0;

		/// @brief
		/// ����Alpha�ο�ֵ
		/// @param ref
		///		Ҫ���õ�Alpha�ο�ֵ
		/// @remarks
		///		ÿ��Ҫд����Ļ�����������ض�����Alphaֵ������ο�ֵ�Ƚϣ������жϺ��������������������ؽ�����д�뻺����
		virtual void SetAlphaReference(float ref) = 0;

		/// @brief
		/// ����/�ر���ɫ���
		/// @param blend
		///		������ر���ɫ���
		/// @remarks
		///		��ɫ�������д�뻺��������ɫ�뻺������������ɫ���в�ͬ��ʽ�Ļ�ϣ������µĽ��д�뻺����
		virtual void ToggleBlending(bool blend) = 0;

		/// @brief
		/// ������ɫ�������
		/// @param src
		///		Դ�������(��Ҫд������ؽ�������)
		/// @param dst
		///		Ŀ��������(�뻺���������е����ؽ�������)
		virtual void SetBlendFactor(BlendFactor src, BlendFactor dst) = 0;


		/// ��ʼһ֡�Ļ��Ʋ���
		virtual void BeginRender() = 0;
		/// ����һ֡�Ļ��Ʋ���
		virtual void EndRender() = 0;
		/// ��ɻ��ƣ�����������
		virtual void SwapBuffer() = 0;

		/// @brief
		/// ��������Ⱦ״̬
		/// @param texState
		///		Ҫ�󶨵�������Ⱦ״̬��Ϣ
		/// @param texUnit
		///		Ҫ�󶨵�������Ԫ���
		/// @remarks
		///		������Ⱦ״ָ̬����������β�������������������ɵ���Ϣ
		///		��������������Ժ󣬽������������������ʼ��������һ��ָ��������Ԫ��ֱ���ֶ��޸�����Ԫ
		virtual void BindTextureRenderState(const TextureRenderState& texState, unsigned int texUnit = 0) = 0;

		/// @brief
		///	ʹ�ö��㼰����������Ⱦ������
		///	@param vbuffer
		///		��Ⱦʹ�õĶ��㻺��
		/// @param ibuffer
		///		��Ⱦʹ�õ���������
		/// @param transform
		///		��Ⱦʹ�õ�ģ�;���
		/// @remarks
		///		ʵ���������ʱ��Ҫ�ڵ��ý���֮ǰ�ָ�ģ�;���
		virtual void RenderVertexBuffer(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, const Matrix4& transform) = 0;


		/// @brief
		/// ��Ⱦ�߿򷽺�
		/// @param vMin
		///		����x��y��z��Сֵ���ɵ�����
		/// @param vMax
		///		����x��y��z���ֵ���ɵ�����
		/// @param transform
		///		���е�����任����
		virtual void RenderBox(const Vector3f& vMin, const Vector3f& vMax, const Matrix4& transform = Matrix4::IDENTITY) = 0;

		/// @brief
		///	��Ⱦ�߿�����
		///	@param point
		///		��������
		/// @param radius
		///		����뾶
		/// @param segment
		///		��������ʱ�ķֶ�����
		/// @remarks
		///		����Ϊ����ռ�
		virtual void RenderSphere(const Vector3f& point, float radius, unsigned int segment = 18) = 0;

		/// @brief
		///	��Ⱦ�߶�
		/// @param begin
		///		�߶���ʼ����
		/// @param end
		///		�߶���ֹ����
		/// @remarks
		///		����Ϊ����ռ�
		virtual void RenderLine(const Vector3f& begin, const Vector3f& end) = 0;


		/// @brief
		///	ָ����������ɫ
		/// @param color
		///		��������ɫ
		/// @remarks
		///		ָ���Ժ󽫻�Ӱ��ÿһ����Ⱦʱ���ʵĻ�����Ч��
		virtual void SetAmbientColor(const Color4f& color) = 0;

		// Textures

		// TODO: �������������Ϊ��������Mipmap��

		// ����һ������

		/// @brief
		///	����2D�豸����
		/// @returns
		///		�����豸����ָ��
		virtual IDeviceTexture* BuildTexture() = 0;

		/// @brief
		/// �����������豸����
		/// @returns
		///		�����豸����ָ��
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

		/// @brief
		/// ���ɶ��㻺��
		/// @returns
		///		�����µĶ��㻺��ָ��
		virtual IVertexBuffer* BuildVertexBuffer() = 0;

		/// @brief
		///	������������
		/// @returns
		///		�����µ���������ָ��
		virtual IIndexBuffer* BuildIndexBuffer() = 0;

		// Render Target

		// ������ȾĿ��

		/// @brief
		///	������ȾĿ��
		/// @returns
		///		�����µ���ȾĿ��ָ��
		virtual IRenderTarget* CreateRenderTarget() = 0;

		/// @brief
		/// ָ����Ⱦ��һ����ȾĿ��
		/// @param rt
		///		Ҫ��Ⱦ������ȾĿ��
		virtual void BindRenderTarget(IRenderTarget* rt) = 0;

		/// @brief
		///	ȡ����Ⱦ����ǰ����ȾĿ�꣬��Ⱦ�غ�̨����
		virtual void UnbindRenderTarget() = 0;
	};

	typedef IRenderDevice*(*RenderSystemFactoryCreateFunc)();
}

#endif
