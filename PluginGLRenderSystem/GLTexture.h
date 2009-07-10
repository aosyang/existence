//-----------------------------------------------------------------------------------
/// GLTexture.h OpenGL�����࣬ʵ����OpenGL�汾��ITexture
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _GLTEXTURE_H
#define _GLTEXTURE_H

#include "ITexture.h"

#include "GL/glew.h"

int up_to_pow_of_two(int n);

class GLTexture : public BaseTexture
{
public:
	GLTexture();
	~GLTexture();

	void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);
	void ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data);

	// do nothing...
	// they work for GPU textures only
	void SetVertexProgram(const string& filename, const string& entry) {}
	void SetFragmentProgram(const string& filename, const string& entry) {}

	// ----- GLTexture Methods

	inline unsigned int GetGLTextureID() const { return m_GLTextureID; }
	inline unsigned int* GetGLTextureIDPtr() { return &m_GLTextureID; }

	void BindTexture();

	int GetTarget() const { return GL_TEXTURE_2D; }

	static bool		s_ForcePowOfTwo;			///< ����������Ϊ2��n���ݣ��Խ��Ӳ����֧�ַ�pow_of_2�����
private:
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
