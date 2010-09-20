//-----------------------------------------------------------------------------------
/// IRenderTarget.h 渲染目标接口
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IRENDERTARGET_H
#define _IRENDERTARGET_H

#include "IDeviceTexture.h"

namespace Gen
{
	class IRenderTarget : public IDeviceTexture
	{
	public:

		// 注：不要在BeginRender和EndRender范围内调用这个函数
		virtual void BuildRenderTarget(unsigned int width, unsigned int height, unsigned int bpp) = 0;

		virtual void BindRenderTarget() = 0;
		virtual void UnbindRenderTarget() = 0;
		//virtual void GenerateMipmap() = 0;

		virtual void Resize(unsigned int width, unsigned int height) = 0;
	};

}

#endif