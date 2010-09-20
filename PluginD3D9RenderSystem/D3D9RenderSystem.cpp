//-----------------------------------------------------------------------------------
/// D3D9RenderSystem.cpp Direct3D 9渲染系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "D3D9RenderSystem.h"
#include "D3D9Texture.h"
#include "Debug.h"

namespace Gen
{
#define COLOR4F_TO_D3DCOLOR(color) D3DCOLOR_COLORVALUE(color.r, color.g, color.b, color.a)
#define D3DCOLOR_TO_COLOR4F(d3dcolor) \
	Color4f(float((0xFF0000 & (d3dcolor)) >> 16)/0xFF, \
	float((0xFF0000 & (d3dcolor)) >> 8)/0xFF, \
	float((0xFF0000 & (d3dcolor)))/0xFF, \
	float((0xFF0000 & (d3dcolor)) >> 24)/0xFF)
#define COLOR4F_TO_D3DCOLORVALUE(d3dcolor, color) \
	d3dcolor.r = color.r; \
	d3dcolor.g = color.g; \
	d3dcolor.b = color.b; \
	d3dcolor.a = color.a;
#define VECTOR3F_TO_D3DVECTOR(d3dvector, vector3f) \
	d3dvector.x = vector3f.x; \
	d3dvector.y = vector3f.y; \
	d3dvector.z = vector3f.z; \

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

	IDirect3DDevice9* g_d3ddevice;
	//IDirect3DTexture9* g_NullTexture;

	D3D9Renderer::D3D9Renderer()
		: m_Direct3D(NULL),
		m_ClearColor(D3DCOLOR_XRGB(0, 0, 0))
	{
	}

	D3D9Renderer::~D3D9Renderer()
	{
	}

	void D3D9Renderer::SetGpuPluginName(const String& filename)
	{

	}

	bool D3D9Renderer::Initialize(RenderWindowParam* windowParam)
	{
		// 初始化Direct3D9
		AssertFatal(m_Direct3D = Direct3DCreate9(D3D_SDK_VERSION), "D3D9Renderer::Initialize() : Failed to create Direct3D9.");

		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof( d3dpp ) );
		d3dpp.Windowed = TRUE;						// true?
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		UINT adapter = D3DADAPTER_DEFAULT;
		D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;

//#define USE_NVPERFHUD

#ifdef USE_NVPERFHUD

		for (UINT i=0; i<m_Direct3D->GetAdapterCount(); i++)
		{
			D3DADAPTER_IDENTIFIER9 identifier;
			HRESULT result;

			result = m_Direct3D->GetAdapterIdentifier(adapter, 0, &identifier);
			if (strstr(identifier.Description, "PerfHUD")!=0)
			{
				adapter = i;
				deviceType = D3DDEVTYPE_REF;
				break;
			}
		}
#endif
		// 创建D3D设备
		AssertFatal(SUCCEEDED(m_Direct3D->CreateDevice(	adapter, deviceType, windowParam->handle, \
														D3DCREATE_SOFTWARE_VERTEXPROCESSING, \
														&d3dpp, &m_D3DDevice)), "D3D9Renderer::Initialize() : Failed to create device.");

		g_d3ddevice = m_D3DDevice;

		m_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		return true;
	}

	void D3D9Renderer::Shutdown()
	{
		// 释放D3D设备
		SAFE_RELEASE(m_D3DDevice);
		SAFE_RELEASE(m_Direct3D);
	}

	const String D3D9Renderer::GetFeatureString() const
	{
		return String();
	}

	void D3D9Renderer::SetClearColor(const Color4f& color)
	{
		m_ClearColor = COLOR4F_TO_D3DCOLOR(color);
	}

	void D3D9Renderer::SetViewport(int left, int bottom, unsigned int width, unsigned int height)
	{
		//D3DVIEWPORT9 viewport = { left, m_WindowHeight - bottom, width, height, 0.0f, 1.0f };
		//if (!height)
		//{
		//	viewport.Width = m_WindowWidth;
		//	viewport.Height = m_WindowHeight;
		//}

		//HRESULT hr = m_D3DDevice->SetViewport(&viewport);
		//AssertFatal(SUCCEEDED(hr), "D3D9Renderer::SetViewport(): Unknown error");
	}
	
	void D3D9Renderer::SetViewMatrix(const Matrix4& viewMat)
	{
		D3DXMATRIXA16 d3dMat;
		BuildD3DMatrix(viewMat, d3dMat);
		m_D3DDevice->SetTransform(D3DTS_VIEW, &d3dMat);
	}

	void D3D9Renderer::SetProjectionMatrix(const Matrix4& projMat)
	{
		// TODO: D3D投影矩阵与OpenGL投影矩阵范围不同
		D3DXMATRIXA16 d3dMat;
		BuildD3DMatrix(projMat, d3dMat);
		m_D3DDevice->SetTransform(D3DTS_PROJECTION, &d3dMat);
	}

	void D3D9Renderer::ClearBuffer(bool color, bool depth, bool stencil)
	{
		DWORD flag = 0;
		if (color) flag |= D3DCLEAR_TARGET;
		if (depth) flag |= D3DCLEAR_ZBUFFER;
		if (stencil) flag |= D3DCLEAR_STENCIL;
		AssertFatal(SUCCEEDED(m_D3DDevice->Clear(0, NULL, flag, m_ClearColor, 1.0f, 0)), "D3D9Renderer::ClearBuffer() : Failed to clear buffer.");
	}

	void D3D9Renderer::ToggleTexture(bool enable, unsigned int index)
	{

	}

	void D3D9Renderer::ToggleDepthWriting(bool enable)
	{

	}


	void D3D9Renderer::ToggleDepthTest(bool enable)
	{
		static bool depthTest = false;

		if (enable && !depthTest)
		{
			m_D3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			depthTest = true;
		}
		else if (!enable && depthTest)
		{
			m_D3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			depthTest = false;
		}
	}

	void D3D9Renderer::ToggleLighting(bool enable)
	{
		m_D3DDevice->SetRenderState(D3DRS_LIGHTING, enable);
	}

	void D3D9Renderer::ToggleAlphaTest(bool enable)
	{
		m_D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, enable);
	}

	void D3D9Renderer::SetAlphaReference(float ref)
	{
		DWORD dwRef = DWORD(0xFF * ref);
		m_D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, dwRef);
	}

	void D3D9Renderer::ToggleBlending(bool blend)
	{
		m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, blend);
	}

	void D3D9Renderer::SetBlendFactor(BlendFactor src, BlendFactor dst)
	{
		m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, GetBlendFactor(src));
		m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, GetBlendFactor(dst));
	}

	void D3D9Renderer::ToggleLight(bool enable, unsigned int index)
	{
		static bool light[8] = { false };
		if (enable && !light[index])
		{
			m_D3DDevice->LightEnable(index, TRUE);
			light[index] = true;
		}
		else if (!enable && light[index])
		{
			m_D3DDevice->LightEnable(index, FALSE);
			light[index] = false;
		}
	}


	void D3D9Renderer::BeginRender()
	{
		AssertFatal(SUCCEEDED(m_D3DDevice->BeginScene()), "D3D9Renderer::BeginRender() : Failed to begin rendering.");

		//D3DXMATRIXA16 viewMat;
		//BuildD3DMatrix(m_ViewMatrix, viewMat);

		//m_D3DDevice->SetTransform(D3DTS_VIEW, &viewMat);
	}

	void D3D9Renderer::EndRender()
	{
		m_D3DDevice->EndScene();
	}

	void D3D9Renderer::SwapBuffer()
	{
		m_D3DDevice->Present(NULL, NULL, NULL, NULL);
	}

	void D3D9Renderer::BindTextureRenderState(const TextureRenderState& texState, unsigned int texUnit)
	{
		if (texState.texture)
			texState.texture->BindTexture();
	}

	void D3D9Renderer::RenderVertexBuffer(IVertexBuffer* vbuffer, PrimitiveType type, const Matrix4& transform)
	{
		D3DXMATRIXA16 worldMat;
		BuildD3DMatrix(transform, worldMat);

		m_D3DDevice->SetTransform(D3DTS_WORLD, &worldMat);

		D3D9VertexBuffer* d3d9VB = static_cast<D3D9VertexBuffer*>(vbuffer);

		d3d9VB->SetAsVertexDataSource();
		d3d9VB->RenderPrimitive(type);
	}

	void D3D9Renderer::RenderVertexBuffer(IVertexBuffer* vbuffer, IIndexBuffer* ibuffer, const Matrix4& transform)
	{
		D3DXMATRIXA16 worldMat;
		BuildD3DMatrix(transform, worldMat);


		m_D3DDevice->SetTransform(D3DTS_WORLD, &worldMat);

		D3D9VertexBuffer* d3d9VB = static_cast<D3D9VertexBuffer*>(vbuffer);
		D3D9IndexBuffer* d3d9IB = static_cast<D3D9IndexBuffer*>(ibuffer);

		d3d9VB->SetAsVertexDataSource();
		d3d9IB->SetVertexNum(d3d9VB->GetVertexNum());
		d3d9IB->RenderPrimitive();
	}

	void D3D9Renderer::SetAmbientColor(const Color4f& color)
	{
		m_D3DDevice->SetRenderState(D3DRS_AMBIENT, COLOR4F_TO_D3DCOLOR(color));
	}

	IDeviceTexture* D3D9Renderer::BuildTexture()
	{
		return new D3D9Texture;
	}

	IDeviceTexture* D3D9Renderer::BuildCubeTexture()
	{
		return NULL;
	}

	void D3D9Renderer::SetVertexColor(const Color4f& color)
	{
		//m_D3DDevice->SetRenderState
	}

	void D3D9Renderer::SetMaterialAmbientColor(const Color4f& color)
	{

	}

	void D3D9Renderer::SetMaterialDiffuseColor(const Color4f& color)
	{

	}

	void D3D9Renderer::SetMaterialSpecularColor(const Color4f& color)
	{

	}

	void D3D9Renderer::SetMaterialEmissiveColor(const Color4f& color)
	{

	}

	void D3D9Renderer::SetMaterialSpecularLevel(float level)
	{

	}

	IGpuProgram* D3D9Renderer::LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type)
	{
		return NULL;
	}

	IGpuProgram* D3D9Renderer::GetGpuProgram(const String& filename, const String& entry, GpuProgramType type)
	{
		return NULL;
	}
	void D3D9Renderer::BindGpuProgram(IGpuProgram* program, GpuProgramType type)
	{

	}

	void D3D9Renderer::UnbindGpuProgram(GpuProgramType type)
	{

	}

	int D3D9Renderer::GetMaxLightsNumber()
	{
		return 8;
	}

	void D3D9Renderer::SetupLight(int index, ILight* light)
	{
		if (light)
		{
			D3DLIGHT9 d3dlight;
			ZeroMemory( &d3dlight, sizeof( D3DLIGHT9 ) );

			switch(light->GetLightType())
			{
			case LIGHT_TYPE_POINT:
			case LIGHT_TYPE_SPOT:
			case LIGHT_TYPE_DIRECTIONAL:
				{
					d3dlight.Type = D3DLIGHT_DIRECTIONAL;
					Vector3f dir = light->GetDirection();
					dir.normalize();
					VECTOR3F_TO_D3DVECTOR(d3dlight.Direction, dir);
					break;
				}
			default:
				break;
			}

			VECTOR3F_TO_D3DVECTOR(d3dlight.Position, light->GetPosition());
			COLOR4F_TO_D3DCOLORVALUE(d3dlight.Ambient, light->GetAmbientColor());
			COLOR4F_TO_D3DCOLORVALUE(d3dlight.Diffuse, light->GetDiffuseColor());
			COLOR4F_TO_D3DCOLORVALUE(d3dlight.Specular, light->GetSpecularColor());

			d3dlight.Attenuation0 = light->GetConstantAttenuation();
			d3dlight.Attenuation1 = light->GetLinearAttenuation();
			d3dlight.Attenuation2 = light->GetQuadraticAttenuation();

			d3dlight.Range = light->GetRange();

			ToggleLight(true, index);
		}
		else
		{
			ToggleLight(false, index);
		}
	}

	IVertexBuffer* D3D9Renderer::BuildVertexBuffer()
	{
		return new D3D9VertexBuffer;
	}

	IIndexBuffer* D3D9Renderer::BuildIndexBuffer()
	{
		return new D3D9IndexBuffer;
	}


	IRenderTarget* D3D9Renderer::CreateRenderTarget()
	{
		return NULL;
	}

	void D3D9Renderer::BindRenderTarget(IRenderTarget* rt)
	{

	}

	void D3D9Renderer::UnbindRenderTarget()
	{

	}


	void D3D9Renderer::BuildD3DMatrix(const Matrix4& mat, D3DMATRIX& d3dMat)
	{
		for (int y=0; y<4; y++)
			for (int x=0; x<4; x++)
			{
				d3dMat.m[y][x] = mat.m[x][y];
			}
	}

	// 从引擎的混合因子转换为D3D9混合因子
	DWORD D3D9Renderer::GetBlendFactor(BlendFactor factor)
	{
		switch (factor)
		{
		case BLEND_FACTOR_ZERO:
			return D3DBLEND_ZERO;
		case BLEND_FACTOR_ONE:
			return D3DBLEND_ONE;
		case BLEND_FACTOR_SRC_COLOR:
			return D3DBLEND_SRCCOLOR;
		case BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
			return D3DBLEND_INVSRCCOLOR;
		case BLEND_FACTOR_SRC_ALPHA:
			return D3DBLEND_SRCALPHA;
		case BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
			return D3DBLEND_INVSRCALPHA;
		case BLEND_FACTOR_DST_ALPHA:
			return D3DBLEND_DESTALPHA;
		case BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
			return D3DBLEND_INVDESTALPHA;
		case BLEND_FACTOR_DST_COLOR:
			return D3DBLEND_DESTCOLOR;
		case BLEND_FACTOR_ONE_MINUS_DST_COLOR:
			return D3DBLEND_INVDESTCOLOR;
		case BLEND_FACTOR_SRC_ALPHA_SATURATE:
			return D3DBLEND_SRCALPHASAT;
		}

		// 不会运行到这里
		return NULL;
	}

	void D3D9Renderer::SetupMaterial(Material* material)
	{
		if (material)
		{
			if (material->GetLighting())
				m_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			else
				m_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			D3DMATERIAL9 mat;

			// Set the RGBA for diffuse reflection.
			memcpy(&mat.Diffuse, &material->GetDiffuse().r, sizeof(float)*4);

			// Set the RGBA for ambient reflection.
			memcpy(&mat.Ambient, &material->GetAmbient().r, sizeof(float)*4);

			// Set the color and sharpness of specular highlights.
			memcpy(&mat.Specular, &material->GetSpecular().r, sizeof(float)*4);
			mat.Power = material->GetSpecularLevel();

			// Set the RGBA for emissive color.
			memcpy(&mat.Emissive, &material->GetEmissive().r, sizeof(float)*4);

			m_D3DDevice->SetMaterial(&mat);

			ToggleDepthTest(material->GetDepthTest());

			TextureRenderState* rs = material->GetTextureRenderState(0);
			if (rs && rs->texture)
			{
				IDirect3DTexture9* d3dtex = static_cast<D3D9Texture*>(rs->texture)->GetD3DTexture();
				m_D3DDevice->SetTexture(0, d3dtex);
			}
			else
			{
				//m_D3DDevice->SetTexture(0, g_NullTexture);
			}
		}
		else
		{
			m_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			ToggleDepthWriting(true);
			ToggleDepthTest(true);
			//m_D3DDevice->SetTexture(0, g_NullTexture);
		}
	}

	IPlugin* CreatePluginInstance()
	{
		return new D3D9Renderer;
	}
}
