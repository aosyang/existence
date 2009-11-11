#include "D3D9Texture.h"

namespace Gen
{
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

	}

	void D3D9Texture::ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data)
	{

	}

	void D3D9Texture::BindTexture()
	{

	}
}
