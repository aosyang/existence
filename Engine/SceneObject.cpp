//-----------------------------------------------------------------------------------
/// SceneObject.cpp �����������ʵ��
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
  m_CollisionGroupMask(0)		// Ĭ�ϲ������κ���ײ
{
}

SceneObject::~SceneObject()
{
	// ɾ�����еĿ���Ⱦ�Ӷ���

	//RenderableObjectSet::iterator iter;
	//for (iter=m_RenderableObjects.begin(); iter!=m_RenderableObjects.end(); iter++)
	//{
	//	delete *iter;
	//}
}

void SceneObject::Update(unsigned long deltaTime)
{
	// ���û��ຯ��������AABB
	MovableObjectBase::Update(deltaTime);

	if (m_ParentObject)
	{
		// ���������û�б����£�����и���
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

	// ���¸���obb����aabb
	m_AABB.Expand(m_OBB, m_WorldTransform);


	SceneObjectSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->Update(deltaTime);

		// ���Ӷ����aabbҲ������ȥ
		m_AABB.Expand((*iter)->m_AABB);
	}
}
//
//void SceneObject::AddRenderableObject(RenderableObjectBase* renderable)
//{
//	// �Ϸ����ж�
//	AssertFatal(m_RenderableObjects.find(renderable)==m_RenderableObjects.end(), "SceneObject::AddRenderableObject(): Scene object owns this renderable already.");
//
//	m_RenderableObjects.insert(renderable);
//}
//
//void SceneObject::RemoveRenderableObject(RenderableObjectBase* renderable)
//{
//	// �Ϸ����ж�
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
	// AABB���
	Vector3f p;
	float d;
	bool result = ray.IntersectsBox(m_AABB.worldMin, m_AABB.worldMax, p, d);

	if (result)
	{
		info.point = p;
		info.distance = d;

		// TODO: ���߷���Ҫ����д
		info.normal = Vector3f(0.0f, 1.0f, 0.0f);
		info.obj = this;
	}

	return result;
}

void SceneObject::AttachChildObject(SceneObject* child, bool keepRotation, bool keepWorldTransform)
{
	// ���child�Ƿ��Ѿ��������б���
	if (m_ChildrenObjects.find(child) != m_ChildrenObjects.end())
	{
		return;
	}

	m_ChildrenObjects.insert(child);

	// ������и����󣬽����ϵ
	if (child->m_ParentObject)
	{
		child->DetachFromParent(keepWorldTransform);
	}
	child->SetParentObject(this, keepRotation);

	// �ݹ�ָ������ĳ���
	child->SetSceneGraphRecursively(m_Scene);

	// ά���Ӷ���������ռ��еı任
	if (keepWorldTransform)
	{
		// TODO: Implement this...
		//child->Transform() = child->WorldTransform() * child->;
	}
	else // ά�ֶ���ǰ�ľֲ��ռ�任
	{
		child->m_WorldTransform = WorldTransform() * child->Transform();
	}

}

void SceneObject::DetachFromParent(bool keepWorldTransform)
{
	AssertFatal(m_ParentObject, "SceneObject::DetachFromParent(): Object has got no parent.");

	if (keepWorldTransform)
	{
		// TODO: �޸�Transform
	}

	// �Ӹ�������Ӷ����б����Ƴ��Լ�
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
