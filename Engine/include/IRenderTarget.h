//-----------------------------------------------------------------------------------
/// IRenderTarget.h ��ȾĿ��ӿ�
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IRENDERTARGET_H
#define _IRENDERTARGET_H

#include "ITexture.h"

namespace Gen
{
	class IRenderTarget
	{
	public:
		virtual ~IRenderTarget() {}

		// ָ��RT����ȾĿ������
		virtual void SetTexture(ITexture* texture) = 0;
		virtual ITexture* GetTexture() const = 0;

		virtual void BindRenderTarget() = 0;
		virtual void GenerateMipmap() = 0;
	};
}

#endif