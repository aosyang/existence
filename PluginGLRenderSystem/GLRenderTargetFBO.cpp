//-----------------------------------------------------------------------------------
/// GLRenderTargetFBO.cpp OpenGL Framebuffer Object��ȾĿ��
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
		// ����FBO
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// ��������õ���Ⱦ����
		glGenRenderbuffers(1, &m_DepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
		// ָ������Ĵ�С�Լ��û���Ⱦ���
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		// �������Ⱦ������FBO��
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

		// ������Ⱦ֡�����õ�����
		glGenTextures(1, &m_FrameBufferTexture);
		glBindTexture(GL_TEXTURE_2D, m_FrameBufferTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		// NOTE: �������ʼ��ʹ��32λ��ɫ����
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		// ָ���������ģʽ(��Ҫ��������FBO״̬ʱ��ʧ��)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// ����ɫ�����������
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FrameBufferTexture, 0);

		// ����Ƿ񴴽��ɹ�
		AssertFatal(GL_FRAMEBUFFER_COMPLETE==glCheckFramebufferStatus(GL_FRAMEBUFFER), "GLRenderTargetFBO::BuildRenderTarget() : Failed to create fbo!");

		// �ָ�Ĭ����Ⱦ����̨����
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// ��Ⱦ�����RT��
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
