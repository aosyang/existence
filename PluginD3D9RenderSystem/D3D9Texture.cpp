#include "D3D9Texture.h"

namespace Gen
{
	extern IDirect3DDevice9* g_d3ddevice;

	D3D9Texture::D3D9Texture()
		: m_D3DTexture(NULL)
	{

	}

	D3D9Texture::~D3D9Texture()
	{
		if (m_D3DTexture)
			m_D3DTexture->Release();
	}

	void D3D9Texture::Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data)
	{
		D3DFORMAT format;

		// 像素颜色数
		int colorNum = 0;

		switch (bpp)
		{
		case 32:
			format = D3DFMT_A8B8G8R8;
			colorNum = 4;
			break;
		case 24:
			format = D3DFMT_R8G8B8;
			colorNum = 3;
			break;
		}

		if (FAILED(D3DXCreateTexture(g_d3ddevice, width, height, 0, 0, format, D3DPOOL_MANAGED, &m_D3DTexture)))
			return;

		D3DSURFACE_DESC desc;
		m_D3DTexture->GetLevelDesc(0, &desc);

		// 如果是空纹理，不在这里指定数据
		if (!data) return;

		D3DLOCKED_RECT rect;
		m_D3DTexture->LockRect(0, &rect, 0, 0);

		// 指定纹理数据
		unsigned char* target = (unsigned char*)rect.pBits;
		unsigned char* source = data;

		for (int y=0; y<height; ++y)
		{
			for (int x=0; x<width; ++x)
			{
				for (int i=0; i<colorNum; i++)
				{
					target[i] = source[i];
				}
				target += colorNum;
				source += colorNum;
			}
		}

		m_D3DTexture->UnlockRect(0);
	}

	void D3D9Texture::ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data)
	{

	}

	void D3D9Texture::BindTexture()
	{
		g_d3ddevice->SetTexture(0, m_D3DTexture);
	}
}
