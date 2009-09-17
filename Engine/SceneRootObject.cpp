//-----------------------------------------------------------------------------------
/// SceneRootObject.cpp 根对象，场景所有对象挂载在这个对象上
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
//
//#include "SceneRootObject.h"
//#include "Engine.h"
//#include <algorithm>
//
//void SceneRootObject::Update(unsigned long deltaTime)
//{
//	ChildrenSceneObjectsSet::iterator iter;
//	for (iter = m_ChildrenObjects.begin();
//		iter != m_ChildrenObjects.end();
//		iter++)
//	{
//		(*iter)->Update(deltaTime);
//	}
//}
//
//bool BaseSceneObjectComparer(BaseSceneObject* lhs,BaseSceneObject* rhs)
//{
//	return lhs->GetRenderOrder() < rhs->GetRenderOrder();
//}
//
//
//void SceneRootObject::Render()
//{
//	//ChildrenSceneObjectsSet::iterator iter;
//	//for (iter = m_ChildrenObjects.begin();
//	//	iter != m_ChildrenObjects.end();
//	//	iter++)
//	//{
//	//	(*iter)->Render();
//	//	//if (Engine::Instance().GetDebugRender())
//	//	//	(*iter)->DebugRender();
//	//}
//
//	// Sorting all the items...
//	sort(m_RenderObjects.begin(), m_RenderObjects.end(), BaseSceneObjectComparer);
//
//	SceneObjectList::iterator iter;
//	for (iter = m_RenderObjects.begin();
//		iter != m_RenderObjects.end();
//		iter++)
//	{
//		(*iter)->Render();
//	}
//}
////
////void SceneRootObject::CollectRenderObjects(const RenderView& view)
////{
////	// 清除渲染对象列表
////	m_RenderObjects.clear();
////
////	// 递归遍历每一个对象，加入到渲染列表中
////	ChildrenSceneObjectsSet::iterator iter;
////	for (iter = m_ChildrenObjects.begin();
////		iter != m_ChildrenObjects.end();
////		iter++)
////	{
////		(*iter)->PrepareRenderObjects(m_RenderObjects, view);
////	}
////}
//
//void SceneRootObject::AttachChildObject(BaseSceneObject* object)
//{
//	// 检查重复添加
//	if (m_ChildrenObjects.find(object) != m_ChildrenObjects.end())
//	{
//		return;
//	}
//
//	m_ChildrenObjects.insert(object);
//}
//
//bool SceneRootObject::DetachChildObject(BaseSceneObject* object)
//{
//	if (m_ChildrenObjects.find(object) != m_ChildrenObjects.end())
//	{
//		m_ChildrenObjects.erase(object);
//
//		return true;
//	}
//
//	return false;
//}
//
////
////void SceneRootObject::CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, int type, int collisionGroup)
////{
////	ChildrenSceneObjectsSet::iterator iter;
////	for (iter = m_ChildrenObjects.begin();
////		iter != m_ChildrenObjects.end();
////		iter++)
////	{
////		(*iter)->CollectRayPickingSceneObject(ray, baseSceneObjects, type, collisionGroup);
////	}
////}
////
