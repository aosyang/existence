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
#include "GLBaseRenderTarget.h"
#include "GLTexture.h"

namespace Gen
{
	class GLRenderTargetFBO : public IRenderTarget
	{
	public:
		GLRenderTargetFBO();
		~GLRenderTargetFBO();

		// ----- Overwrite IDeviceTexture
		void BindTexture();

		TextureType GetType() const { return TEXTURE_TYPE_2D; }

		// ----- Overwrite IRenderTarget
		void BuildRenderTarget(unsigned int width, unsigned int height, unsigned int bpp);

		void BindRenderTarget();
		void UnbindRenderTarget();

		void Resize(unsigned int width, unsigned int height);

		// ----- GLRenderTargetFBO methods

		void Destroy();

	private:
		GLuint	m_FBO;						///< Frame Buffer Object
		GLuint	m_DepthBuffer;				///< Depth Render Buffer
		GLuint	m_FrameBufferTexture;		///< Frame Buffer Texture
	};
}

#endif
