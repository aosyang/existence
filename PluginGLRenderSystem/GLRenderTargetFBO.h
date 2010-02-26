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
	class GLRenderTargetFBO : public BaseRenderTarget
	{
	public:
		GLRenderTargetFBO();
		~GLRenderTargetFBO();

		// ----- Overwrite IRenderTarget
		void SetTexture(DeviceTexture2D* texture);

		void BindRenderTarget();
		void UnbindRenderTarget();

		// ----- GLRenderTargetFBO methods

	private:
		GLuint			m_FBO;
		GLuint			rboId;
	};
}

#endif
