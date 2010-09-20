//-----------------------------------------------------------------------------------
/// Renderer.cpp �������Ⱦ����ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "Renderer.h"
#include "SceneObject.h"
#include "Log.h"
#include "RenderTarget.h"

#include <algorithm>


namespace Gen
{
	RenderCommand::RenderCommand()
	{
		// �����б�������
		memset(this, 0, sizeof(RenderCommand));
	}

	// ��Ⱦ����������
	bool RenderCmdComparer(const RenderCommand& lhs, const RenderCommand& rhs)
	{
		if (lhs.renderOrder == rhs.renderOrder)
		{
			if (lhs.material == rhs.material)
			{
				return lhs.sceneObj < rhs.sceneObj;
			}
			else
				return lhs.material < rhs.material;
		}

		return lhs.renderOrder < rhs.renderOrder;
	}


	// TODO: ��API�޹ع��ܴ�RenderSystem������Ƶ�����

	Renderer::Renderer()
		: m_RenderDevice(NULL),
		  m_WindowWidth(0),
		  m_WindowHeight(0),
		  m_MaxLightCount(0),
		  m_Active(false),
		  m_AmbientColor(0.2f, 0.2f, 0.2f, 1.0f),
		  m_ScreenQuadVB(NULL)
	{
		m_ModelMatrix.MakeIdentity();
		m_ViewMatrix.MakeIdentity();
		m_ProjMatrix.MakeIdentity();
	}

	bool Renderer::Initialize(RenderWindowParam* windowParam)
	{
		// ��ʼ�����
		Log.MsgLn("Initializing RenderSystem");

		if (!m_RenderDevice->Initialize(windowParam)) return false;
		
		ResizeRenderWindow(windowParam->width, windowParam->height);

		m_MaxLightCount = m_RenderDevice->GetMaxLightsNumber();

		Log.MsgLn(m_RenderDevice->GetFeatureString());

		// ����ScreenQuad�Ķ��㻺��
		m_ScreenQuadVB = BuildVertexBuffer();

		float quadVertex[] =
		{
			-1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,

			1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
		};

		//float quadTexcoord[] =
		//{
		//	
		//}

		m_ScreenQuadVB->CreateBuffer(VFormat_Position,
									 quadVertex, NULL, NULL, NULL, 6);

		return true;
	}

	void Renderer::Shutdown()
	{
		SAFE_DELETE(m_ScreenQuadVB);

		// �ر���Ⱦ�豸
		Log.MsgLn("Shutitng down renderer");
		m_RenderDevice->Shutdown();
		delete m_RenderDevice;
	}

	void Renderer::SetupLight(int index, ILight* light)
	{
		// TODO: APIʹ�õ��ǹ̶����մ������
		//		 Ҫ�����ս���Shader������Ҫ��Shader�ṩ������ٽ���Դ��Ϣ
		m_RenderDevice->SetupLight(index, light);
	}

	void Renderer::ResizeRenderWindow(unsigned int width, unsigned int height)
	{
		if (width != 0 && height != 0)
		{
			m_WindowWidth = width;
			m_WindowHeight = height;
		}

		if (m_WindowHeight == 0) m_WindowHeight = 1;
	}

	void Renderer::SetAmbientColor(const Color4f& color)
	{
		m_AmbientColor = color;
		m_RenderDevice->SetAmbientColor(color);
	}

	Color4f Renderer::GetAmbientColor() const
	{
		return m_AmbientColor;
	}

	void Renderer::ClearBuffer(bool color, bool depth, bool stencil)
	{
		m_RenderDevice->ClearBuffer(color, depth, stencil);
	}

	void Renderer::SetClearColor(const Color4f& color)
	{
		m_RenderDevice->SetClearColor(color);
	}

	void Renderer::SetModelMatrix(const Matrix4& modelMat)
	{
		m_ModelMatrix = modelMat;
		m_RenderDevice->SetModelMatrix(modelMat);
	}

	void Renderer::SetViewMatrix(const Matrix4& viewMat)
	{
		// ������Ⱦ�豸����ͼ����
		m_ViewMatrix = viewMat;
		m_RenderDevice->SetViewMatrix(viewMat);
	}

	void Renderer::SetProjectionMatrix(const Matrix4& projMat)
	{
		// ������Ⱦ�豸��ͶӰ����
		m_ProjMatrix = projMat;
		m_RenderDevice->SetProjectionMatrix(projMat);
	}

	const Matrix4& Renderer::GetModelMatrix() const
	{
		return m_ModelMatrix;
	}

	const Matrix4& Renderer::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	const Matrix4& Renderer::GetProjectionMatrix() const
	{
		return m_ProjMatrix;
	}

	void Renderer::SetViewport(int left, int bottom, unsigned int width, unsigned int height)
	{
		// �����ָ���ӿڵĸ߶ȣ���Ϊʹ��ȫ���ߴ�
		if (height)
			m_RenderDevice->SetViewport(left, bottom, width, height);
		else
			m_RenderDevice->SetViewport(left, bottom, m_WindowWidth, m_WindowHeight);
	}

	void Renderer::BeginRender(RenderTarget* rt)
	{
		if (rt)
			m_RenderDevice->BeginRender(rt->GetDeviceRT());
		else
			m_RenderDevice->BeginRender();
	}

	void Renderer::EndRender()
	{
		m_RenderDevice->EndRender();
	}

	void Renderer::SwapBuffer()
	{
		m_RenderDevice->SwapBuffer();
	}

	void Renderer::SubmitRenderCommand(const RenderCommand& cmd)
	{
		m_RenderCmdList.push_back(cmd);
	}

	void Renderer::ClearRenderCommand()
	{
		m_RenderCmdList.clear();
	}

	void Renderer::CommandRender()
	{
		// ����Ⱦָ�����������Ҫ��������Ϊ��Ⱦ˳�򣬴�Ҫ����Ϊ���ʣ��Ժ����������������
		sort(m_RenderCmdList.begin(), m_RenderCmdList.end(), RenderCmdComparer);

		// ����������Pass��
		unsigned int maxPassCount = 0;

		if (m_OverrideMaterial)
			maxPassCount = m_OverrideMaterial->GetPassCount();
		else
		{
			for (RenderCmdList::iterator iter=m_RenderCmdList.begin(); iter!=m_RenderCmdList.end(); iter++)
			{
				unsigned int n;
				if (iter->material && (n=iter->material->GetPassCount())>maxPassCount)
					maxPassCount = n;
			}
		}

		for (unsigned p=0; p<maxPassCount; p++)
		{
			for (RenderCmdList::iterator iter=m_RenderCmdList.begin(); iter!=m_RenderCmdList.end(); iter++)
			{
				BaseMaterial* material = m_OverrideMaterial;
				if (!material)
					material = iter->material;

				// �����ǰ������Pass�Ѿ������ò��ʵ�Pass���ޣ�����
				if (material && (material->GetPassCount()-1<p)) 
					continue;

				SetModelMatrix(Matrix4::IDENTITY);
				if (iter->sceneObj)
					iter->sceneObj->SetupLightForRendering();

				// ����ģ�;���
				SetModelMatrix(iter->transform);

				// ���ò���
				if (material)
					material->UsePassState(p);
				else
					SetupMaterial(NULL);

				// ��Ⱦ�����
				if (iter->indexBuffer)
					RenderPrimitives(iter->vertexBuffer, iter->indexBuffer, iter->primType);
				else
					RenderPrimitives(iter->vertexBuffer, iter->primType);

				// �ָ���������
				if (material) material->ResetPassState(p);
			}
		}
	}

	void Renderer::RenderPrimitives(IVertexBuffer* vbuffer, PrimitiveType type)
	{
		m_RenderDevice->RenderVertexBuffer(vbuffer, type);
	}

	void Renderer::RenderPrimitives(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, PrimitiveType type)
	{
		m_RenderDevice->RenderVertexBuffer(vbuffer, ibuffer, type);
	}

	void Renderer::RenderScreenQuad(Material* mat)
	{
		SetModelMatrix(Matrix4::IDENTITY);
		SetupMaterial(mat);
		m_RenderDevice->RenderVertexBuffer(m_ScreenQuadVB, PRIM_TRIANGLES);
	}

	void Renderer::SetupMaterial(Material* material)
	{
		if (material)
		{
			// ���ռ���������
			// ע��ʹ��Shader����Щ����ʧЧ
			// ͬʱ�ܵ�Ӱ��Ļ���BindTextureRenderState�Լ�SetupLight����
			// ------------------------------- fixed pipeline -------------------------------
			bool lighting = material->GetLighting();

			m_RenderDevice->ToggleLighting(lighting);
			if (!lighting)
			{
				m_RenderDevice->SetVertexColor(material->GetColor());
			}

			m_RenderDevice->SetMaterialAmbientColor(material->GetAmbient());
			m_RenderDevice->SetMaterialDiffuseColor(material->GetDiffuse());
			m_RenderDevice->SetMaterialSpecularColor(material->GetSpecular());
			m_RenderDevice->SetMaterialEmissiveColor(material->GetEmissive());
			m_RenderDevice->SetMaterialSpecularLevel(material->GetSpecularLevel());

			// ------------------------------- fixed pipeline -------------------------------

			// ���д��
			m_RenderDevice->ToggleDepthWriting(material->GetDepthWriting());

			// ��Ȳ���
			m_RenderDevice->ToggleDepthTest(material->GetDepthTest());

			// Alpha����
			if (material->GetAlphaTest())
			{
				m_RenderDevice->ToggleAlphaTest(true);
				m_RenderDevice->SetAlphaReference(material->GetAlphaRef());
			}
			else
				m_RenderDevice->ToggleAlphaTest(false);

			// Blending
			if (material->GetBlending())
			{
				m_RenderDevice->ToggleBlending(true);
				m_RenderDevice->SetBlendFactor(	material->GetSrcBlendFactor(),
												material->GetDstBlendFactor());
			}
			else
				m_RenderDevice->ToggleBlending(false);

			for (int i=0; i<8; i++)
			{
				TextureRenderState* rs = material->GetTextureRenderState(i);

				IDeviceTexture* deviceTex = material->GetDeviceTexture(i);
				if (material->GetTextureLayerEnabled(i) && deviceTex)
				{
					// TODO: ���������֮����Ը��������в�����
					m_RenderDevice->ToggleTexture(true, i);
					m_RenderDevice->BindTextureRenderState(deviceTex, *rs, i);
				}
				else
				{
					m_RenderDevice->ToggleTexture(false, i);
				}
			}

			// TODO: Shader��������Բ�ͬʱʹ��
			IGpuProgram* vp = material->GetVertexProgram();
			IGpuProgram* fp = material->GetFragmentProgram();
			if (vp)
			{
				m_RenderDevice->EnableGpuProfile(GPU_PROFILE_VERTEX);
				vp->UpdateParameters();
				m_RenderDevice->BindGpuProgram(material->GetVertexProgram());
			}
			else
				m_RenderDevice->DisableGpuProfile(GPU_PROFILE_VERTEX);

			if (fp)
			{
				m_RenderDevice->EnableGpuProfile(GPU_PROFILE_FRAGMENT);
				fp->UpdateParameters();
				m_RenderDevice->BindGpuProgram(material->GetFragmentProgram());
			}
			else
				m_RenderDevice->DisableGpuProfile(GPU_PROFILE_FRAGMENT);
		}
		else
		{
			for (int i=0; i<8; i++)
			{
				m_RenderDevice->ToggleTexture(false, i);
			}

			m_RenderDevice->DisableGpuProfile(GPU_PROFILE_VERTEX);
			m_RenderDevice->DisableGpuProfile(GPU_PROFILE_FRAGMENT);

			// ��Ĭ�ϲ���
			m_RenderDevice->ToggleLighting(false);
			m_RenderDevice->SetVertexColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
			m_RenderDevice->ToggleDepthWriting(true);
			m_RenderDevice->ToggleDepthTest(true);
		}
	}

	void Renderer::BindTextureRenderState(IDeviceTexture* deviceTex, TextureRenderState* texState, unsigned int texUnit)
	{
		if (!texState)
		{
			// ������ʱ��������Ⱦ״̬
			TextureRenderState rs;
			m_RenderDevice->BindTextureRenderState(deviceTex, rs, texUnit);
		}
		else
			m_RenderDevice->BindTextureRenderState(deviceTex, *texState, texUnit);
	}

	IRenderTarget* Renderer::BuildRenderTarget()
	{
		return m_RenderDevice->CreateRenderTarget();
	}

	IDeviceTexture* Renderer::BuildTexture2D()
	{
		return m_RenderDevice->BuildTexture();
	}

	IDeviceTexture* Renderer::BuildCubeTexture()
	{
		return m_RenderDevice->BuildCubeTexture();
	}

	IVertexBuffer* Renderer::BuildVertexBuffer()
	{
		return m_RenderDevice->BuildVertexBuffer();
	}

	IIndexBuffer* Renderer::BuildIndexBuffer()
	{
		return m_RenderDevice->BuildIndexBuffer();
	}

	IGpuProgram* Renderer::LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type)
	{
		return m_RenderDevice->LoadGpuProgram(filename, entry, type);
	}

	IEffect* Renderer::LoadEffect(const String& filename)
	{
		return m_RenderDevice->LoadEffect(filename);
	}

	void Renderer::SetOverrideMaterial(BaseMaterial* mat)
	{
		m_OverrideMaterial = mat;
	}

	// TODO: �滻���º���ʵ��
	void Renderer::ToggleDepthTest(bool enable)
	{
		m_RenderDevice->ToggleDepthTest(enable);
	}

	void Renderer::ToggleLighting(bool enable)
	{
		m_RenderDevice->ToggleLighting(enable);
	}

}
