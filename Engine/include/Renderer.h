//-----------------------------------------------------------------------------------
/// Renderer.h �������Ⱦ����������ֱ�ӵ��ã���������Ⱦ�豸ִ����Ⱦ����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _RENDERER_H
#define _RENDERER_H

#include "Singleton.h"
#include "IRenderDevice.h"
#include "EString.h"

#include <vector>


namespace Gen
{
	class SceneObject;
	class RenderTarget;

	/// @brief
	///	��Ⱦ����
	/// @par
	///		��¼һ����Ⱦ��ԭ�Ӳ�����������Ⱦ�������������
	class RenderCommand
	{
	public:
		unsigned int	renderOrder;		///< ��Ⱦ˳��
		// TODO: ������Ҫ���һ���������÷���
		SceneObject*	sceneObj;			///< �����������ڵ������ù��յķ���
		BaseMaterial*	material;			///< ����
		IVertexBuffer*	vertexBuffer;		///< ���㻺��
		IIndexBuffer*	indexBuffer;		///< ��������
		PrimitiveType	primType;			///< ���������
		Matrix4			transform;			///< ����任

		RenderCommand();
	};

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
		void SetModelMatrix(const Matrix4& modelMat);
		void SetViewMatrix(const Matrix4& viewMat);
		void SetProjectionMatrix(const Matrix4& projMat);
		const Matrix4& GetModelMatrix() const;
		const Matrix4& GetViewMatrix() const;
		const Matrix4& GetProjectionMatrix() const;

		// ָ���ӿڷ�Χ
		void SetViewport(int left, int bottom, unsigned int width, unsigned int height);

		// ��ʼ��Ⱦ����ָ��RTʱ��Ⱦ����̨����
		void BeginRender(RenderTarget* rt = NULL);
		void EndRender();

		// ������Ļ����
		void SwapBuffer();

		/// @brief
		///	�ύ��Ⱦ����
		/// @param cmd
		///		��Ⱦ����
		void SubmitRenderCommand(const RenderCommand& cmd);

		/// @brief
		///	�����Ⱦ�����б�
		/// @remarks
		///		�����һ����Ⱦ����ĵ����Ժ󣬵����������
		void ClearRenderCommand();

		/// @brief
		///	����Ⱦ�����б��е�����������Ⱦ
		void CommandRender();

		// ��Ⱦ�����
		void RenderPrimitives(IVertexBuffer* vbuffer, PrimitiveType type);
		void RenderPrimitives(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, PrimitiveType type);

		void RenderScreenQuad(Material* mat);

		// ָ����Ⱦ����
		// �������������ݲ���������Ⱦ���Ĳ��ʼ��������״̬
		void SetupMaterial(Material* material);

		// ָ����Ⱦ����״̬
		void BindTextureRenderState(IDeviceTexture* deviceTex, TextureRenderState* texState = NULL, unsigned int texUnit = 0);

		// ����һ����ȾĿ��
		IRenderTarget* BuildRenderTarget();

		// ����һ��2D�豸����
		IDeviceTexture* BuildTexture2D();

		// ����һ������������
		IDeviceTexture* BuildCubeTexture();

		// ���㻺������������
		IVertexBuffer* BuildVertexBuffer();
		IIndexBuffer* BuildIndexBuffer();

		// ��ȡshader
		IGpuProgram* LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type);

		// ��ȡ��Ч�����ļ�
		IEffect* LoadEffect(const String& filename);

		void SetOverrideMaterial(BaseMaterial* mat);

		// TODO: ��Щ������ʱʹ��ԭ�з�ʽʵ�֣����滻
		void ToggleDepthTest(bool enable);
		void ToggleLighting(bool enable);
	private:
		Renderer();
	private:
		IRenderDevice*		m_RenderDevice;

		unsigned int		m_WindowWidth,
							m_WindowHeight;

		Matrix4				m_ModelMatrix;
		Matrix4				m_ViewMatrix;
		Matrix4				m_ProjMatrix;

		int					m_MaxLightCount;
		bool				m_Active;
		Color4f				m_AmbientColor;

		IVertexBuffer*		m_ScreenQuadVB;

		typedef std::vector<RenderCommand>	RenderCmdList;
		RenderCmdList		m_RenderCmdList;

		BaseMaterial*		m_OverrideMaterial;			///< ǿ��ʹ�õĲ��ʣ���������������Ժ�ǿ��CommandRender��Ⱦ�Ķ���ʹ��������ʽ�����Ⱦ
	};

}

#endif
