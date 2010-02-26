#include "Renderer.h"
#include "Log.h"

namespace Gen
{
	// TODO: 将API无关功能从RenderSystem插件中移到这里

	Renderer::Renderer()
		: m_RenderDevice(NULL),
		  m_WindowWidth(0),
		  m_WindowHeight(0),
		  m_MaxLightCount(0),
		  m_Active(false),
		  m_AmbientColor(0.2f, 0.2f, 0.2f, 1.0f)
	{
		m_ViewMatrix.MakeIdentity();
		m_ProjMatrix.MakeIdentity();
	}

	bool Renderer::Initialize(RenderWindowParam* windowParam)
	{
		// 初始化插件
		Log.MsgLn("Initializing RenderSystem");

		bool result = m_RenderDevice->Initialize(windowParam);
		ResizeRenderWindow(windowParam->width, windowParam->height);

		m_MaxLightCount = m_RenderDevice->GetMaxLightsNumber();

		Log.MsgLn(m_RenderDevice->GetFeatureString());

		return result;
	}

	void Renderer::Shutdown()
	{
		// 关闭渲染设备
		Log.MsgLn("Shutitng down renderer");
		m_RenderDevice->Shutdown();
		delete m_RenderDevice;
	}

	void Renderer::SetupLight(int index, ILight* light)
	{
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

	void Renderer::SetViewMatrix(const Matrix4& viewMat)
	{
		// 设置渲染设备的视图矩阵
		m_ViewMatrix = viewMat;
		m_RenderDevice->SetViewMatrix(viewMat);
	}

	void Renderer::SetProjectionMatrix(const Matrix4& projMat)
	{
		// 设置渲染设备的投影矩阵
		m_ProjMatrix = projMat;
		m_RenderDevice->SetProjectionMatrix(projMat);
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
		// 如果不指定视口的高度，认为使用全屏尺寸
		if (height)
			m_RenderDevice->SetViewport(left, bottom, width, height);
		else
			m_RenderDevice->SetViewport(left, bottom, m_WindowWidth, m_WindowHeight);
	}

	void Renderer::BeginRender()
	{
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
		
	void Renderer::RenderPrimitives(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, const Matrix4& transform)
	{
		m_RenderDevice->RenderVertexBuffer(vbuffer, ibuffer, transform);
	}

	void Renderer::SetupMaterial(Material* material)
	{
		if (material)
		{
			// 光照及材质属性
			// 注：使用Shader后这些都将失效
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

			// 深度写入
			m_RenderDevice->ToggleDepthWriting(material->GetDepthWriting());

			// 深度测试
			m_RenderDevice->ToggleDepthTest(material->GetDepthTest());

			// Alpha测试
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

				if (material->GetTextureLayerEnabled(i) && rs->texture)
				{
					// TODO: 启用纹理层之后，则对该纹理层进行操作？
					m_RenderDevice->ToggleTexture(true, i);
					m_RenderDevice->BindTextureRenderState(*rs, i);
				}
				else
				{
					m_RenderDevice->ToggleTexture(false, i);
				}
			}

			// TODO: Shader与材质属性不同时使用
			if (material->GetVertexProgram())
				m_RenderDevice->BindGpuProgram(material->GetVertexProgram(), GPU_VERTEX_PROGRAM);
			else
				m_RenderDevice->UnbindGpuProgram(GPU_VERTEX_PROGRAM);

			if (material->GetFragmentProgram())
				m_RenderDevice->BindGpuProgram(material->GetFragmentProgram(), GPU_FRAGMENT_PROGRAM);
			else
				m_RenderDevice->UnbindGpuProgram(GPU_FRAGMENT_PROGRAM);
		}
		else
		{
			for (int i=0; i<8; i++)
			{
				m_RenderDevice->ToggleTexture(false, i);
			}

			m_RenderDevice->UnbindGpuProgram(GPU_VERTEX_PROGRAM);
			m_RenderDevice->UnbindGpuProgram(GPU_FRAGMENT_PROGRAM);

			// 绑定默认材质
			m_RenderDevice->ToggleLighting(false);
			m_RenderDevice->SetVertexColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
			m_RenderDevice->ToggleDepthWriting(true);
			m_RenderDevice->ToggleDepthTest(true);
		}
	}

	void Renderer::BindTextureRenderState(const TextureRenderState& texState, unsigned int texUnit)
	{
		m_RenderDevice->BindTextureRenderState(texState, texUnit);
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

	// TODO: 替换以下函数实现
	void Renderer::ToggleDepthTest(bool enable)
	{
		m_RenderDevice->ToggleDepthTest(enable);
	}

	void Renderer::ToggleLighting(bool enable)
	{
		m_RenderDevice->ToggleLighting(enable);
	}

	void Renderer::RenderBox(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color, const Matrix4& transform)
	{
		SetupMaterial(NULL);
		m_RenderDevice->SetVertexColor(color);
		m_RenderDevice->RenderBox(vMin, vMax, transform);
	}

	void Renderer::RenderSphere(const Vector3f& point, float radius, const Color4f& color, unsigned int segment)
	{
		SetupMaterial(NULL);
		m_RenderDevice->SetVertexColor(color);
		m_RenderDevice->RenderSphere(point, radius, segment);
	}

	void Renderer::RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color)
	{
		SetupMaterial(NULL);
		m_RenderDevice->SetVertexColor(color);
		m_RenderDevice->RenderLine(begin, end);
	}

}
