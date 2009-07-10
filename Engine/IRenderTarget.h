//-----------------------------------------------------------------------------------
/// IRenderTarget.h 渲染目标接口
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IRENDERTARGET_H
#define _IRENDERTARGET_H

class IRenderTarget
{
public:
	virtual ITexture* GetTexture() = 0;

};

#endif