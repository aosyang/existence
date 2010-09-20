#ifndef _GLBASERENDERTARGET_H
#define _GLBASERENDERTARGET_H

#include "IRenderTarget.h"
#include "GLTexture.h"
#include "Debug.h"

namespace Gen
{
	class BaseRenderTarget : public IRenderTarget
	{
	public:
		BaseRenderTarget()
			: m_Texture(NULL)
		{
		}

		//void SetTexture(DeviceTexture2D* texture)
		//{
		//	AssertFatal(texture, "BaseRenderTarget::SetTexture(): Texture cannot be null");
		//	AssertFatal(texture->GetType()==TEXTURE_TYPE_2D, "BaseRenderTarget::SetTexture(): GLRenderTargetFBO supports only target GL_TEXTURE_2D for now.");
		//	m_Texture = static_cast<GLTexture*>(texture);

		//	m_Width = texture->GetWidth();
		//	m_Height = texture->GetHeight();
		//}

		DeviceTexture2D* GetTexture() const { return m_Texture; }

		void GenerateMipmap()
		{
			if (!m_Texture) return;

			glBindTexture(GL_TEXTURE_2D, m_Texture->GetGLTextureID());
			glGenerateMipmapEXT(GL_TEXTURE_2D);
		}
	protected:
		GLTexture*		m_Texture;
		//unsigned int	m_Width;
		//unsigned int	m_Height;
	};
}

#endif