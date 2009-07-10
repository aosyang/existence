//-----------------------------------------------------------------------------------
/// SceneRootObject.cpp �����󣬳������ж�����������������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "SceneRootObject.h"
#include "Engine.h"

void SceneRootObject::Update(unsigned long deltaTime)
{
	ChildrenSceneObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->Update(deltaTime);
	}
}

bool BaseSceneObjectComparer(BaseSceneObject* lhs,BaseSceneObject* rhs)
{
	return lhs->GetRenderOrder() < rhs->GetRenderOrder();
}


void SceneRootObject::Render()
{
	//ChildrenSceneObjectsSet::iterator iter;
	//for (iter = m_ChildrenObjects.begin();
	//	iter != m_ChildrenObjects.end();
	//	iter++)
	//{
	//	(*iter)->Render();
	//	//if (Engine::Instance().GetDebugRender())
	//	//	(*iter)->DebugRender();
	//}

	// Sorting all the items...
	stable_sort(m_RenderObjects.begin(), m_RenderObjects.end(), BaseSceneObjectComparer);

	ChildrenSceneObjectsSet::iterator iter;
	for (iter = m_RenderObjects.begin();
		iter != m_RenderObjects.end();
		iter++)
	{
		(*iter)->Render();
	}
}

void SceneRootObject::CollectRenderObjects()
{
	// �����Ⱦ�����б�
	m_RenderObjects.clear();

	// ����ÿһ�����󣬼��뵽��Ⱦ�б���
	ChildrenSceneObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->PrepareRenderObjects(m_RenderObjects);
	}
}

void SceneRootObject::AttachChildObject(BaseSceneObject* object)
{
	// ����ظ����
	if (m_ChildrenObjects.find(object) != m_ChildrenObjects.end())
	{
		return;
	}

	m_ChildrenObjects.insert(object);
}

bool SceneRootObject::DetachChildObject(BaseSceneObject* object)
{
	if (m_ChildrenObjects.find(object) != m_ChildrenObjects.end())
	{
		m_ChildrenObjects.erase(object);

		return true;
	}

	return false;
}


void SceneRootObject::CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, CollisionType type)
{
	ChildrenSceneObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->CollectRayPickingSceneObject(ray, baseSceneObjects, type);
	}
}

