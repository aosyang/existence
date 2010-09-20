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

namespace Gen
{
	GLRenderTargetFBO::GLRenderTargetFBO()
	: m_FBO(0),
	  m_DepthBuffer(0),
	  m_FrameBufferTexture(0)

	{
	}

	GLRenderTargetFBO::~GLRenderTargetFBO()
	{
		Destroy();
	}

	void GLRenderTargetFBO::BindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, m_FrameBufferTexture);
	}

	void GLRenderTargetFBO::BuildRenderTarget(unsigned int width, unsigned int height, unsigned int bpp)
	{
		// 创建FBO
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// 生成深度用的渲染缓冲
		glGenRenderbuffers(1, &m_DepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
		// 指定缓冲的大小以及用户渲染深度
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		// 将深度渲染缓冲与FBO绑定
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

		// 生成渲染帧缓冲用的纹理
		glGenTextures(1, &m_FrameBufferTexture);
		glBindTexture(GL_TEXTURE_2D, m_FrameBufferTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		// NOTE: 这里假设始终使用32位颜色缓冲
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		// 指定纹理过滤模式(重要，否则检测FBO状态时会失败)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 将颜色缓冲与纹理绑定
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FrameBufferTexture, 0);

		// 检测是否创建成功
		AssertFatal(GL_FRAMEBUFFER_COMPLETE==glCheckFramebufferStatus(GL_FRAMEBUFFER), "GLRenderTargetFBO::BuildRenderTarget() : Failed to create fbo!");

		// 恢复默认渲染到后台缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// 渲染到这个RT上
	void GLRenderTargetFBO::BindRenderTarget()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER, m_FBO);
		//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	}

	void GLRenderTargetFBO::UnbindRenderTarget()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	}

	void GLRenderTargetFBO::Resize(unsigned int width, unsigned int height)
	{
		if (width!=0 && height!=0)
		{
			Destroy();
			BuildRenderTarget(width, height, 32);
		}
	}

	void GLRenderTargetFBO::Destroy()
	{
		if (m_FBO)
		{
			glDeleteFramebuffers(1, &m_FBO);
			m_FBO = 0;
		}

		if (m_DepthBuffer)
		{
			glDeleteRenderbuffers(1, &m_DepthBuffer);
			m_DepthBuffer = 0;
		}

		if (m_FrameBufferTexture)
		{
			glDeleteTextures(1, &m_FrameBufferTexture);
			m_FrameBufferTexture = 0;
		}
	}
}
