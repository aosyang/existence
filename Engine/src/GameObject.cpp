//-----------------------------------------------------------------------------------
/// GameObjectBase.cpp 游戏对象基类实现代码
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "GameObject.h"

namespace Gen
{
	GameObject::GameObject(SceneGraph* scene)
	: SceneObject(scene)
	{
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Destroy()
	{
	}

	//void GameObject::SetupLightForRendering()
	//{
	//}

	void GameObject::AddEffectiveLight(Light* light)
	{
	}

	void GameObject::ClearEffectiveLights()
	{
	}

	//
	//void GameObjectBase::CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum)
	//{
	//	renderableObjs.push_back(m_Renderable);
	//}
	//
}
