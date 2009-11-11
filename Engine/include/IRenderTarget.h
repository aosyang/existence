//-----------------------------------------------------------------------------------
/// IRenderTarget.h 渲染目标接口
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

		// 指定RT的渲染目的纹理
		virtual void SetTexture(ITexture* texture) = 0;
		virtual ITexture* GetTexture() const = 0;

		virtual void BindRenderTarget() = 0;
		virtual void GenerateMipmap() = 0;
	};
}

#endif