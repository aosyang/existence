#ifndef _D3D9TEXTURE_H
#define _D3D9TEXTURE_H

#include "ITexture.h"
#include <d3dx9.h>

namespace Gen
{
	class D3D9Texture : public BaseTexture
	{
	public:
		D3D9Texture();
		~D3D9Texture();

		// ----- ITexture Methods

		void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);
		void ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data);

		void SetVertexProgram(const String& filename, const String& entry) {}
		void SetFragmentProgram(const String& filename, const String& entry) {}

		void BindTexture();

		TextureType GetTextureType() const { return TEXTURE_TYPE_2D; }

		// ----- D3D9Texture Methods

		void SetD3DTexture(IDirect3DTexture9* texture) { m_D3DTexture = texture; }
		IDirect3DTexture9* GetD3DTexture() { return m_D3DTexture; }
	private:
		IDirect3DTexture9*	m_D3DTexture;
	};
}

#endif
