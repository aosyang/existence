//-----------------------------------------------------------------------------------
/// GLTexture.h OpenGL纹理类，实现了OpenGL版本的ITexture
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _GLTEXTURE_H
#define _GLTEXTURE_H

#include "ITexture.h"

#include "GL/glew.h"

class GLRenderer;

int up_to_pow_of_two(int n);

class GLTexture : public BaseTexture
{
	friend class GLRenderer;
public:
	~GLTexture();

	// ----- ITexture Methods

	void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);
	void ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data);

	// do nothing...
	// they work for GPU textures only
	void SetVertexProgram(const String& filename, const String& entry) {}
	void SetFragmentProgram(const String& filename, const String& entry) {}

	void BindTexture();

	TextureType GetTextureType() const { return TEXTURE_TYPE_2D; }

	// ----- GLTexture Methods

	inline unsigned int GetGLTextureID() const { return m_GLTextureID; }
	inline unsigned int* GetGLTextureIDPtr() { return &m_GLTextureID; }

	static bool		s_ForcePowOfTwo;			///< 将纹理缩放为2的n次幂，以解决硬件不支持非pow_of_2的情况
private:
	GLTexture();

	//int				m_WrapType;
	//
	//int				m_MinFilterType;
	//int				m_MagFilterType;

	//bool			m_UseEyeSpaceTexCoord;
	//Matrix4			m_EyeSpaceMatrix;

	unsigned int	m_GLTextureID;
	GLint			m_InternalFormat;
	GLint			m_Format;

};


#endif
