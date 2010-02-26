#ifndef _RENDERER_H
#define _RENDERER_H

#include "Singleton.h"
#include "IRenderDevice.h"
#include "EString.h"

namespace Gen
{

	// �����������Ⱦ����ͨ���������API����ʵ�ֻ�ͼ����
	class Renderer : public Singleton<Renderer>
	{
		friend class Singleton<Renderer>;
	public:

		// ��ʼ����Ⱦ��
		bool Initialize(RenderWindowParam* windowParam);

		// �ر���Ⱦ��
		void Shutdown();

		int GetMaxLightCount() const { return m_MaxLightCount; }

		void SetupLight(int index, ILight* light);

		// ����/��ȡ��Ⱦ����ǰ�Ƿ񼤻�
		inline void SetActive(bool active) { m_Active = active; }
		inline bool GetActive() const { return m_Active; }

		// ����/��ȡ��Ⱦ����ǰʹ�õ���Ⱦ�豸
		inline void SetRenderDevice(IRenderDevice* device) { m_RenderDevice = device; }
		inline IRenderDevice* GetRenderDevice() { return m_RenderDevice; }

		// �޸���Ⱦ����ߴ�
		void ResizeRenderWindow(unsigned int width = 0, unsigned int height = 0);

		void SetAmbientColor(const Color4f& color);
		Color4f GetAmbientColor() const;

		// �����Ļ����
		void ClearBuffer(bool color = true, bool depth = true, bool stencil = false);

		// ����������ɫ
		void SetClearColor(const Color4f& color);
		
		// ����
		void SetViewMatrix(const Matrix4& viewMat);
		void SetProjectionMatrix(const Matrix4& projMat);
		const Matrix4& GetViewMatrix() const;
		const Matrix4& GetProjectionMatrix() const;

		// ָ���ӿڷ�Χ
		void SetViewport(int left, int bottom, unsigned int width, unsigned int height);

		void BeginRender();
		void EndRender();

		// ������Ļ����
		void SwapBuffer();

		// ��Ⱦ�����
		void RenderPrimitives(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, const Matrix4& transform);

		// ָ����Ⱦ����
		// �������������ݲ���������Ⱦ���Ĳ��ʼ��������״̬
		void SetupMaterial(Material* material);

		// ָ����Ⱦ����״̬
		void BindTextureRenderState(const TextureRenderState& texState, unsigned int texUnit = 0);

		// ����һ��2D�豸����
		IDeviceTexture* BuildTexture2D();

		// ����һ������������
		IDeviceTexture* BuildCubeTexture();

		// ���㻺������������
		IVertexBuffer* BuildVertexBuffer();
		IIndexBuffer* BuildIndexBuffer();

		// TODO: ��Щ������ʱʹ��ԭ�з�ʽʵ�֣����滻
		void ToggleDepthTest(bool enable);
		void ToggleLighting(bool enable);

		void RenderBox(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), const Matrix4& transform = Matrix4::IDENTITY);
		void RenderSphere(const Vector3f& point, float radius, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), unsigned int segment = 18);
		void RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f));
	private:
		Renderer();
	private:
		IRenderDevice*		m_RenderDevice;

		unsigned int		m_WindowWidth,
							m_WindowHeight;

		Matrix4				m_ViewMatrix;
		Matrix4				m_ProjMatrix;

		int					m_MaxLightCount;
		bool				m_Active;
		Color4f				m_AmbientColor;
	};

}

#endif
