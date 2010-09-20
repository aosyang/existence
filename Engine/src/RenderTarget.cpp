//-----------------------------------------------------------------------------------
/// RenderTarget.cpp 渲染目标类，实现一个可以绘制像素的动态纹理。
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#include "RenderTarget.h"
#include "Renderer.h"

namespace Gen
{
	IMPLEMENT_RESOURCE_MANAGER(RenderTarget);

	RenderTarget::RenderTarget()
		: m_DeviceRT(NULL)
	{

	}

	RenderTarget::~RenderTarget()
	{
		SAFE_DELETE(m_DeviceRT);
	}

	RenderTarget* RenderTarget::CreateRenderTarget(const String& name, unsigned int width, unsigned int height, unsigned int bpp)
	{
		RenderTarget* rt = FactoryCreate(name);

		if (rt)
		{
			if (rt->BuildRenderTarget(width, height, bpp))
				return rt;
			else
			{
				DestroyObject(name);
			}
		}

		return NULL;
	}

	void RenderTarget::Resize(unsigned int width, unsigned int height)
	{
		if (m_DeviceRT) m_DeviceRT->Resize(width, height);
	}

	bool RenderTarget::BuildRenderTarget(unsigned int width, unsigned int height, unsigned int bpp)
	{
		m_DeviceRT = Renderer::Instance().BuildRenderTarget();

		// 未能通过API创建RT，返回false
		if (!m_DeviceRT) return false;

		// 根据尺寸创建RT
		m_DeviceRT->BuildRenderTarget(width, height, bpp);
		return true;
	}

}

