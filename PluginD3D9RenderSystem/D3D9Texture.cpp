#include "D3D9Texture.h"

D3D9Texture::D3D9Texture()
: m_D3DTexture(NULL)
{

}

D3D9Texture::~D3D9Texture()
{
	if (m_D3DTexture)
		m_D3DTexture->Release();
}
