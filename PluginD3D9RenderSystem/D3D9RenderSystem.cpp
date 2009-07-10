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

IDirect3DDevice9* g_d3ddevice;


IDirect3DTexture9* g_NullTexture;


D3D9Renderer::D3D9Renderer()
: m_Direct3D(NULL),
  m_ClearColor(D3DCOLOR_XRGB(0, 0, 0))
{
	m_ViewMatrix.Identity();
	//Debug::EnableBreakOnAlloc();
}

D3D9Renderer::~D3D9Renderer()
{
}

bool D3D9Renderer::Initialize(RenderWindowParam* windowParam)
{
	m_hWnd = windowParam->handle;

	AssertFatal(m_Direct3D = Direct3DCreate9(D3D_SDK_VERSION), "D3D9Renderer::Initialize() : Failed to create Direct3D9.");

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;						// true?
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	AssertFatal(SUCCEEDED(m_Direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, \
												   D3DCREATE_SOFTWARE_VERTEXPROCESSING, \
												   &d3dpp, &m_D3DDevice)), "D3D9Renderer::Initialize() : Failed to create device.");

	if (FAILED(D3DXCreateTextureFromFile(m_D3DDevice, L"../../../data/no_material.bmp", &g_NullTexture)))
	{
		AssertFatal(0, "For test only: Unable to load no_material.bmp!")
	}

	g_d3ddevice = m_D3DDevice;

	m_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	return true;
}

void D3D9Renderer::Shutdown()
{
	UnloadAllTextures();

	if (m_D3DDevice) m_D3DDevice->Release();
	if (m_Direct3D) m_Direct3D->Release();
}

void D3D9Renderer::SetClearColor(const Color4f& color)
{
	m_ClearColor = COLOR4F_TO_D3DCOLOR(color);
}

void D3D9Renderer::SetFrustum(Frustum* frustum)
{
	m_Frustum = frustum;
	m_ProjMatrix = m_Frustum->ProjectionMatrix();
}

void D3D9Renderer::ResizeRenderWindow(unsigned int width, unsigned int height)
{
	if (width != 0 && height != 0)
	{
		m_WindowWidth = width;
		m_WindowHeight = height;
	}

	if (m_WindowHeight == 0) m_WindowHeight = 1;

	// TODO: Resize D3D viewport here...
	//glViewport(0, 0, m_WindowWidth, m_WindowHeight);	//Reset The Current Viewport

}
void D3D9Renderer::SetProjectionMode(ProjectionMatrixMode mode)
{
	switch (mode)
	{
	case PROJECTION_MODE_PERSPECTIVE:
		{
			D3DXMATRIXA16 projMat;
			BuildD3DMatrix(m_ProjMatrix, projMat);

			m_D3DDevice->SetTransform(D3DTS_PROJECTION, &projMat);
			return;
		}
	case PROJECTION_MODE_ORTHO:
		{
			//float w = (GLfloat)m_WindowWidth/(GLfloat)m_WindowHeight;
			//gluOrtho2D(-w, w, -1.0f, 1.0f);
			return;
		}
	default:
		AssertFatal(0, "D3D9Renderer::SetProjectionMode() : Invalid projection matrix mode.");
	}
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

	D3DXMATRIXA16 viewMat;
	BuildD3DMatrix(m_ViewMatrix, viewMat);

	m_D3DDevice->SetTransform(D3DTS_VIEW, &viewMat);
}

void D3D9Renderer::EndRender(bool swapBuffer)
{
	m_D3DDevice->EndScene();
	if (swapBuffer)
		m_D3DDevice->Present(NULL, NULL, NULL, NULL);
}

void D3D9Renderer::RenderVertexBuffer(IVertexBuffer* vbuffer, Material* material, const Matrix4& transform)
{
	D3DXMATRIXA16 worldMat;
	BuildD3DMatrix(transform, worldMat);


	m_D3DDevice->SetTransform(D3DTS_WORLD, &worldMat);

	SetupMaterial(material);

	vbuffer->RenderBuffer();
}


void D3D9Renderer::RenderAABB(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color, const Matrix4& transform)
{

}

void D3D9Renderer::RenderSphere(const Vector3f& point, float radius, const Color4f& color, unsigned int segment)
{

}

void D3D9Renderer::RenderLine(const Vector3f& begin, const Vector3f& end, const Color4f& color)
{

}

void D3D9Renderer::RenderScreenQuad(float left, float top, float right, float bottom, ITexture* texture)
{

}

void D3D9Renderer::RenderScreenQuad(int x1, int y1, int x2, int y2, ITexture* texture, const Color4f& color)
{

}

void D3D9Renderer::SetAmbientColor(const Color4f& color)
{

	m_D3DDevice->SetRenderState(D3DRS_AMBIENT, COLOR4F_TO_D3DCOLOR(color));
}

const Color4f D3D9Renderer::GetAmbientColor()
{
	DWORD ambientColor;
	m_D3DDevice->GetRenderState(D3DRS_AMBIENT, &ambientColor);
	return D3DCOLOR_TO_COLOR4F(ambientColor);
}

ITexture* D3D9Renderer::BuildTexture(const string& textureName, unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data)
{
	if(m_TextureList.find(textureName) != m_TextureList.end())
	{
		delete m_TextureList[textureName];
	}

	D3DFORMAT format;

	switch (bpp)
	{
	case 32:
		format = D3DFMT_A8B8G8R8;
		break;
	case 24:
		format = D3DFMT_R8G8B8;
		break;
	}

	IDirect3DTexture9* d3dtexture;
	if (FAILED(D3DXCreateTexture(m_D3DDevice, width, height, 0, 0, format, D3DPOOL_MANAGED, &d3dtexture)))
		return NULL;

	D3DSURFACE_DESC desc;
	d3dtexture->GetLevelDesc(0, &desc);

	D3DLOCKED_RECT rect;
	d3dtexture->LockRect(0, &rect, 0, 0);

	// TODO 这里有问题
	WORD* pDst = (WORD*)rect.pBits;   
	int DPitch = rect.Pitch>>1;   

	for (int y=0; y<height; ++y){   
		for (int x=0; x<width; ++x)   
		{     
			pDst[y*DPitch + x] = data[y*width + x];   
		}   
	}   

	d3dtexture->UnlockRect(0);

	D3D9Texture* d3dtex = new D3D9Texture();
	d3dtex->SetD3DTexture(d3dtexture);

	m_TextureList[textureName] = d3dtex;

	return d3dtex;
}

ITexture* D3D9Renderer::GetTexture(const string& textureName)
{
	TextureList::iterator iter;

	if ((iter = m_TextureList.find(textureName)) != m_TextureList.end())
		return iter->second;

	return NULL;
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
				VECTOR3F_TO_D3DVECTOR(d3dlight.Direction, dir)
				break;
			}
		default:
			break;
		}

		VECTOR3F_TO_D3DVECTOR(d3dlight.Position, light->GetPosition())
		COLOR4F_TO_D3DCOLORVALUE(d3dlight.Ambient, light->GetAmbientColor())
		COLOR4F_TO_D3DCOLORVALUE(d3dlight.Diffuse, light->GetDiffuseColor())
		COLOR4F_TO_D3DCOLORVALUE(d3dlight.Specular, light->GetSpecularColor())

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


void D3D9Renderer::BuildD3DMatrix(const Matrix4& mat, D3DMATRIX& d3dMat)
{
	for (int y=0; y<4; y++)
		for (int x=0; x<4; x++)
		{
			d3dMat.m[y][x] = mat.m[x][y];
		}
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

		m_D3DDevice->SetTexture(0, static_cast<D3D9Texture*>(material->GetTextureRenderState(0)->texture)->GetD3DTexture());
	}
}

bool D3D9Renderer::UnloadTexture(const string& textureName)
{
	bool result(true);
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if(m_TextureList.find(textureName) != m_TextureList.end())
	{
		delete m_TextureList[textureName];
		m_TextureList.erase(textureName);
	}
	//otherwise, unload failed
	else
	{
		result = false;
	}

	return result;
}

void D3D9Renderer::UnloadAllTextures()
{
	//start at the begginning of the texture map
	TextureList::iterator i = m_TextureList.begin();

	//Unload the textures untill the end of the texture map is found
	while(i != m_TextureList.end())
	{
		UnloadTexture(i->first);
		i = m_TextureList.begin();
	}

	//clear the texture map
	m_TextureList.clear();
}


IRenderer* CreateRenderSystem()
{
	return new D3D9Renderer;
}
