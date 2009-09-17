//-----------------------------------------------------------------------------------
/// GLRenderTargetFBO.cpp OpenGL Framebuffer Object渲染目标
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "GLRenderTargetFBO.h"
#include "Debug.h"

#include "GL/glew.h"

GLRenderTargetFBO::GLRenderTargetFBO()
: m_Texture(NULL),
  m_FBO(0),
  rboId(0)
{
	glGenFramebuffersEXT(1, &m_FBO);
}

GLRenderTargetFBO::~GLRenderTargetFBO()
{
}

// 指定RT使用的纹理，并根据纹理设置RT尺寸
void GLRenderTargetFBO::SetTexture(ITexture* texture)
{
	AssertFatal(texture->GetTextureType()==TEXTURE_TYPE_2D, "GLRenderTargetFBO::SetTexture(): GLRenderTargetFBO supports only target GL_TEXTURE_2D for now.");
	m_Texture = static_cast<GLTexture*>(texture);

	m_Width = texture->GetWidth();
	m_Height = texture->GetHeight();

	if (m_Texture)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FBO);

		glGenRenderbuffersEXT(1, &rboId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetGLTextureID(), 0);

		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rboId);

		// 恢复渲染到后台缓冲
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

// 渲染到这个RT上
void GLRenderTargetFBO::BindRenderTarget()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER, m_FBO);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
}

void GLRenderTargetFBO::GenerateMipmap()
{
	if (!m_Texture) return;

	glBindTexture(GL_TEXTURE_2D, m_Texture->GetGLTextureID());
	glGenerateMipmapEXT(GL_TEXTURE_2D);
}
