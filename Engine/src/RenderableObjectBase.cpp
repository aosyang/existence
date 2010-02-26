//-----------------------------------------------------------------------------------
/// RenderableObjectBase.cpp 可渲染对象基类的实现代码
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "RenderableObjectBase.h"
#include "LightingManager.h"
#include "LightableObject.h"
#include "Engine.h"
#include "Renderer.h"

namespace Gen
{
	RenderableObjectBase::RenderableObjectBase(SceneGraph* scene)
	: SceneObject(scene),
	  m_Visible(true),
	  m_RenderOrder(100),
	  m_LightableObject(NULL)
	{
	}

	RenderableObjectBase::~RenderableObjectBase()
	{
	}

	void RenderableObjectBase::Destroy()
	{
		DestroyLightableObject();
	}

	void RenderableObjectBase::RenderSingleObject()
	{
		// 如果为这个对象指定了光照物体，设置光照状态并进行渲染
		if (m_LightableObject)
		{
			m_LightableObject->SetupLights();
		}
		else	// 否则关闭所有光照状态
		{
			// Disable all lights
			int maxLightNum = Renderer::Instance().GetMaxLightCount();

			for (int i=0; i<maxLightNum; i++)
			{
				Renderer::Instance().SetupLight(i, NULL);
			}
		}
	}

	void RenderableObjectBase::SetRenderOrder(unsigned int order)
	{
		m_RenderOrder = order;
	}


	void RenderableObjectBase::CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum)
	{
		if (!IsCulled(frustum))
		{
			renderableObjs.push_back(this);

			SceneObject::CollectRenderableObject(renderableObjs, frustum);
		}
	}

	void RenderableObjectBase::CreateLightableObject()
	{
		m_LightableObject = new LightableObject();
		m_LightableObject->SetRenderableObject(this);
		LightingManager::Instance().AddLightableObject(m_LightableObject);
	}

	void RenderableObjectBase::DestroyLightableObject()
	{
		if (m_LightableObject)
		{
			LightingManager::Instance().RemoveLightableObject(m_LightableObject);

			SAFE_DELETE(m_LightableObject);
		}
	}
}
