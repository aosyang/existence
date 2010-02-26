#ifndef _GLCUBETEXTURE_H
#define _GLCUBETEXTURE_H

#include "GLTexture.h"

namespace Gen
{
	class GLCubeTexture : public DeviceCubeTexture
	{
		friend class GLRenderer;
	public:
		~GLCubeTexture();

		void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data, int cubePos);
		void ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data);

		void BindTexture();

		TextureType GetTextureType() const { return TEXTURE_TYPE_CUBE; }

	private:
		GLCubeTexture();

		unsigned int	m_GLTextureID;
		GLint			m_InternalFormat[6];
		GLint			m_Format[6];
	};
}

#endif
