//-----------------------------------------------------------------------------------
/// SceneObject.cpp 场景对象类的实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "SceneObject.h"

SceneObject::SceneObject()
: m_Visible(true),
  m_Scene(NULL),
  m_ParentObject(NULL),
  m_KeepRotation(false),
  m_CollisionGroupMask(0)		// 默认不参与任何碰撞
{
}

SceneObject::~SceneObject()
{
	// 删除所有的可渲染子对象

	//RenderableObjectSet::iterator iter;
	//for (iter=m_RenderableObjects.begin(); iter!=m_RenderableObjects.end(); iter++)
	//{
	//	delete *iter;
	//}
}

void SceneObject::Update(unsigned long deltaTime)
{
	// 调用基类函数，重置AABB
	MovableObjectBase::Update(deltaTime);

	if (m_ParentObject)
	{
		// 如果父对象没有被更新，则进行更新
		if (!m_ParentObject->IsUpdated())
			m_ParentObject->Update(deltaTime);

		if (m_KeepRotation)
			m_WorldTransform.SetPosition((m_ParentObject->WorldTransform() * m_Transform).GetPosition());
		else
			m_WorldTransform = m_ParentObject->WorldTransform() * m_Transform;
	}
	else
	{
		m_WorldTransform = m_Transform;
	}

	// 重新根据obb计算aabb
	m_AABB.Expand(m_OBB, m_WorldTransform);


	SceneObjectSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->Update(deltaTime);

		// 将子对象的aabb也包括进去
		m_AABB.Expand((*iter)->m_AABB);
	}
}
//
//void SceneObject::AddRenderableObject(RenderableObjectBase* renderable)
//{
//	// 合法性判断
//	AssertFatal(m_RenderableObjects.find(renderable)==m_RenderableObjects.end(), "SceneObject::AddRenderableObject(): Scene object owns this renderable already.");
//
//	m_RenderableObjects.insert(renderable);
//}
//
//void SceneObject::RemoveRenderableObject(RenderableObjectBase* renderable)
//{
//	// 合法性判断
//	AssertFatal(m_RenderableObjects.find(renderable)!=m_RenderableObjects.end(), "SceneObject::RemoveRenderableObject(): Scene object doesn't own this renderable.");
//
//	m_RenderableObjects.erase(renderable);
//	delete renderable;
//}

bool SceneObject::IsCulled(Frustum* frustum)
{
	if (m_Visible)
		return MovableObjectBase::IsCulled(frustum);

	return true;
}

void SceneObject::SetSceneGraphRecursively(SceneGraph* scene)
{
	m_Scene = scene;

	SceneObjectSet::iterator iter;
	for (iter=m_ChildrenObjects.begin(); iter!=m_ChildrenObjects.end(); iter++)
	{
		(*iter)->SetSceneGraphRecursively(scene);
	}
}

void SceneObject::CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum)
{
	SceneObjectSet::iterator iter;
	for (iter=m_ChildrenObjects.begin(); iter!=m_ChildrenObjects.end(); iter++)
	{
		(*iter)->CollectRenderableObject(renderableObjs, frustum);
	}
}

void SceneObject::RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup)
{
	CollisionResult result;

	if ((collisionGroup & m_CollisionGroupMask) &&
		//(type)
		IntersectsRay(ray, result))
	{
		sceneObjects.push_back(result);
	}

	SceneObjectSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->RayPickingSceneObject(ray, sceneObjects, type, collisionGroup);
	}
}

bool SceneObject::IntersectsRay(const Ray& ray, CollisionResult& info)
{
	// AABB检测
	Vector3f p;
	float d;
	bool result = ray.IntersectsBox(m_AABB.worldMin, m_AABB.worldMax, p, d);

	if (result)
	{
		info.point = p;
		info.distance = d;

		// TODO: 法线方向不要这样写
		info.normal = Vector3f(0.0f, 1.0f, 0.0f);
		info.obj = this;
	}

	return result;
}

void SceneObject::AttachChildObject(SceneObject* child, bool keepRotation, bool keepWorldTransform)
{
	// 检查child是否已经存在于列表中
	if (m_ChildrenObjects.find(child) != m_ChildrenObjects.end())
	{
		return;
	}

	m_ChildrenObjects.insert(child);

	// 如果已有父对象，解除关系
	if (child->m_ParentObject)
	{
		child->DetachFromParent(keepWorldTransform);
	}
	child->SetParentObject(this, keepRotation);

	// 递归指定物体的场景
	child->SetSceneGraphRecursively(m_Scene);

	// 维持子对象在世界空间中的变换
	if (keepWorldTransform)
	{
		// TODO: Implement this...
		//child->Transform() = child->WorldTransform() * child->;
	}
	else // 维持对象当前的局部空间变换
	{
		child->m_WorldTransform = WorldTransform() * child->Transform();
	}

}

void SceneObject::DetachFromParent(bool keepWorldTransform)
{
	AssertFatal(m_ParentObject, "SceneObject::DetachFromParent(): Object has got no parent.");

	if (keepWorldTransform)
	{
		// TODO: 修改Transform
	}

	// 从父对象的子对象列表中移除自己
	m_ParentObject->m_ChildrenObjects.erase(this);
	m_ParentObject = NULL;
	SetSceneGraph(NULL);
}

void SceneObject::DumpToLogRecursively(int level)
{
	String msg, name;
	for (int i=0; i<level; i++)
		msg += "   ";
	name.Format("+- %s", GetTypeName().Data());
	msg += name;
	Log.MsgLn(msg);

	SceneObjectSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->DumpToLogRecursively(level+1);
	}
}


void SceneObject::SetParentObject(SceneObject* parent, bool keepRotation)
{
	m_ParentObject = parent;
	m_KeepRotation = keepRotation;
}
