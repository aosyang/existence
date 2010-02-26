//-----------------------------------------------------------------------------------
/// GLTexture.cpp OpenGL材质类
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "GLTexture.h"
#include "Debug.h"
#include <math.h>

namespace Gen
{
	bool GLTexture::m_sForcePowOfTwo = false;

	int up_to_pow_of_two(int n)
	{
		if (n==1) return 1;

		int l = int(log(float(n - 1)) / log(2.0f));
		return 1 << (l+1);
	}

	GLTexture::GLTexture()
		: m_GLTextureID(0)
	{
	}

	GLTexture::~GLTexture()
	{
		if (m_GLTextureID)
		{
			glDeleteTextures(1, &m_GLTextureID);
			m_GLTextureID = 0;
		}
	}

	void GLTexture::Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data)
	{
		glGenTextures(1, &m_GLTextureID);

		m_Bpp = bpp;

		//GLint internalFormat;		// 每像素颜色数(RGB为3， RGBA为4)
		//GLint format;				// 像素格式(RGB的顺序以及是否含有Alpha通道)

		// FIXME: 对于8bit的纹理读取目前还有问题

		switch (bpp)
		{
		case 32:
			m_InternalFormat = 4;
			m_Format = GL_BGRA_EXT;
			break;
		case 24:
			m_InternalFormat = 3;
			m_Format = GL_BGR_EXT;
			break;
		case 8:
			m_InternalFormat = GL_R3_G3_B2;
			m_Format = GL_BGR;
			break;
		default:
			AssertFatal(0, "GLTexture::Create() : Unsupported texture color depth.");
			break;
		}

		// 绑定纹理id
		glBindTexture(GL_TEXTURE_2D, m_GLTextureID);

		// 自动生成mipmap
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		int pow_of_two_width = up_to_pow_of_two(width);
		int pow_of_two_height = up_to_pow_of_two(height);

		if (m_sForcePowOfTwo && (width!=pow_of_two_width || height!=pow_of_two_height))
		{
			m_Width = pow_of_two_width;
			m_Height = pow_of_two_height;

			// 分配一块新的空间用于缩放纹理

			unsigned char* scaleImage = new unsigned char[m_Width * m_Height * bpp / 8];

			gluScaleImage(m_Format, width, height, GL_UNSIGNED_BYTE, data, m_Width, m_Height, GL_UNSIGNED_BYTE, scaleImage);

			glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height,
						 0, m_Format, GL_UNSIGNED_BYTE, scaleImage);

			delete [] scaleImage;
		}
		else
		{
			m_Width = width;
			m_Height = height;

			//store the texture data for OpenGL use
			glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height,
						 0, m_Format, GL_UNSIGNED_BYTE, data);
		}

		// 生成mipmap
		//gluBuild2DMipmaps(GL_TEXTURE_2D, m_InternalFormat, width, height, m_Format, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture::ModifyRectData(int xoffset, int yoffset, int width, int height, void* data)
	{
		glBindTexture(GL_TEXTURE_2D, m_GLTextureID);

		// 修改纹理部分区域
		glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, m_Format,GL_UNSIGNED_BYTE, data);

		// Note: glGetTexImage可以用于读取纹理的数据
	}

	void GLTexture::BindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, m_GLTextureID);
	}
}

