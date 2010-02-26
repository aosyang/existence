#include "GLRenderTargetTexture.h"

namespace Gen
{
	void GLRenderTargetTexture::BindRenderTarget()
	{
		// do nothing
	}

	void GLRenderTargetTexture::UnbindRenderTarget()
	{
		m_Texture->BindTexture();

		// 将后台缓冲复制到纹理中
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_Width, m_Height);
	}
}
