#ifndef _D3D9TEXTURE_H
#define _D3D9TEXTURE_H

#include "ITexture.h"
#include <d3dx9.h>

class D3D9Texture : public ITexture
{
public:
	D3D9Texture();
	~D3D9Texture();

	void SetVertexProgram(const string& filename, const string& entry) {}
	void SetFragmentProgram(const string& filename, const string& entry) {}

	void SetD3DTexture(IDirect3DTexture9* texture) { m_D3DTexture = texture; }
	IDirect3DTexture9* GetD3DTexture() { return m_D3DTexture; }
private:
	IDirect3DTexture9*	m_D3DTexture;
};

#endif
