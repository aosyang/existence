#include "GLCubeTexture.h"
#include "Debug.h"

namespace Gen
{
	GLCubeTexture::GLCubeTexture()
		: m_GLTextureID(0)
	{
		glGenTextures(1, &m_GLTextureID);
	}

	GLCubeTexture::~GLCubeTexture()
	{
		if (m_GLTextureID)
		{
			glDeleteTextures(1, &m_GLTextureID);
			m_GLTextureID = 0;
		}
	}

	void GLCubeTexture::Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data, int cubePos)
	{
		// FIXME: 对于8bit的纹理读取目前还有问题

		switch (bpp)
		{
		case 32:
			m_InternalFormat[cubePos] = 4;
			m_Format[cubePos] = GL_BGRA_EXT;
			break;
		case 24:
			m_InternalFormat[cubePos] = 3;
			m_Format[cubePos] = GL_BGR_EXT;
			break;
		case 8:
			m_InternalFormat[cubePos] = GL_R3_G3_B2;
			m_Format[cubePos] = GL_BGR;
			break;
		default:
			AssertFatal(0, "GLTexture::Create() : Unsupported texture color depth.");
			break;
		}

		// 绑定纹理id
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLTextureID);

		// 自动生成mipmap
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);

		int pow_of_two_width = up_to_pow_of_two(width);
		int pow_of_two_height = up_to_pow_of_two(height);

		if (GLTexture::m_sForcePowOfTwo && (width!=pow_of_two_width || height!=pow_of_two_height))
		{
			int w = pow_of_two_width;
			int h = pow_of_two_height;

			// 分配一块新的空间用于缩放纹理

			unsigned char* scaleImage = new unsigned char[w * h * bpp / 8];

			gluScaleImage(m_Format[cubePos], width, height, GL_UNSIGNED_BYTE, data, w, h, GL_UNSIGNED_BYTE, scaleImage);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubePos, 0, m_InternalFormat[cubePos], w, h,
				0, m_Format[cubePos], GL_UNSIGNED_BYTE, scaleImage);

			delete [] scaleImage;
		}
		else
		{
			//m_Width = width;
			//m_Height = height;

			//store the texture data for OpenGL use
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubePos, 0, m_InternalFormat[cubePos], width, height,
				0, m_Format[cubePos], GL_UNSIGNED_BYTE, data);
		}
	}

	void GLCubeTexture::ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data)
	{

	}


	void GLCubeTexture::BindTexture()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLTextureID);
	}
}

