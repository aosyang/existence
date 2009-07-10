#ifndef _GLCUBETEXTURE_H
#define _GLCUBETEXTURE_H

#include "GLTexture.h"

enum CubeMapSide
{
	CUBE_MAP_POSITIVE_X = 0,
	CUBE_MAP_NEGATIVE_X,
	CUBE_MAP_POSITIVE_Y,
	CUBE_MAP_NEGATIVE_Y,
	CUBE_MAP_POSITIVE_Z,
	CUBE_MAP_NEGATIVE_Z,
};

class GLCubeTexture : public BaseTexture
{
public:
	GLCubeTexture();
	~GLCubeTexture();

	void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data, int cubePos);
	void ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data);

	void SetVertexProgram(const string& filename, const string& entry) {}
	void SetFragmentProgram(const string& filename, const string& entry) {}

	void BindTexture();

	int GetTarget() const { return GL_TEXTURE_CUBE_MAP; }

private:
	unsigned int	m_GLTextureID;
	GLint			m_InternalFormat[6];
	GLint			m_Format[6];
};

#endif
