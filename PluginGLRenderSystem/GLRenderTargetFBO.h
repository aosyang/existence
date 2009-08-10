//-----------------------------------------------------------------------------------
/// GLRenderTargetFBO.h OpenGL Framebuffer ObjectäÖÈ¾Ä¿±ê
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _GLRENDERTARGETFBO_H
#define _GLRENDERTARGETFBO_H

#include "IRenderTarget.h"
#include "GLTexture.h"

class GLRenderTargetFBO : public IRenderTarget
{
public:
	GLRenderTargetFBO();
	~GLRenderTargetFBO();

	// ----- Overwrite IRenderTarget
	void SetTexture(ITexture* texture);
	ITexture* GetTexture() const { return m_Texture; }

	void BindRenderTarget();
	void GenerateMipmap();

	// ----- GLRenderTargetFBO methods

private:
	GLTexture*		m_Texture;
	unsigned int	m_Width;
	unsigned int	m_Height;

	GLuint			m_FBO;
	GLuint			rboId;
};

#endif
