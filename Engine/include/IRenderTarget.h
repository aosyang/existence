//-----------------------------------------------------------------------------------
/// IRenderTarget.h ��ȾĿ��ӿ�
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
	class IRenderTarget
	{
	public:
		virtual ~IRenderTarget() {}

		// ָ��RT����ȾĿ������
		virtual void SetTexture(DeviceTexture2D* texture) = 0;
		virtual DeviceTexture2D* GetTexture() const = 0;

		virtual void BindRenderTarget() = 0;
		virtual void UnbindRenderTarget() = 0;
		virtual void GenerateMipmap() = 0;
	};

}

#endif