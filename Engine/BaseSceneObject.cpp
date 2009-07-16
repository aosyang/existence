//-----------------------------------------------------------------------------------
/// BaseSceneObject.cpp ����������࣬���������������������ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "BaseSceneObject.h"
#include "Engine.h"

BaseSceneObject::BaseSceneObject()
: m_ParentObject(NULL),
  m_KeepRotation(false),
  m_DebugRender(false),
  m_BoundingSphereRadius(1.0f),
  m_Visible(true),
  m_Scene(NULL),
  m_RenderOrder(100),
  m_CollisionGroupMask(-1)
{
	m_OBB.localMin = Vector3f(-0.5f, -0.5f, -0.5f);
	m_OBB.localMax = Vector3f(0.5f, 0.5f, 0.5f);

	m_Transform.Identity();
	m_WorldTransform.Identity();
}

BaseSceneObject::~BaseSceneObject()
{
}

void BaseSceneObject::Update(unsigned long deltaTime)
{
	// ע������ÿ������ĸ���˳��
	// 
	// �����������
	// ��������״̬
	// �����Ӷ���

	if (m_ParentObject)
	{
		if (m_KeepRotation)
			m_WorldTransform.SetPosition((m_ParentObject->m_WorldTransform * m_Transform).GetPosition());
		else
			m_WorldTransform = m_ParentObject->m_WorldTransform * m_Transform;
	}
	else
	{
		m_WorldTransform = m_Transform;
	}

	// ���¸���obb����aabb
	m_AABB.Reset();
	m_AABB.Expand(m_OBB, m_WorldTransform);


	ChildrenSceneObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->Update(deltaTime);

		// ���Ӷ����aabbҲ������ȥ
		m_AABB.Expand((*iter)->m_AABB);
	}
}

void BaseSceneObject::Render()
{
	if (Engine::Instance().GetDebugRender() || m_DebugRender)
		DebugRender();

	// TODO: ��ʹ�õݹ���õ�Render����

	// ������Ҳ��Ҫ����������෽����ʵ�ֱ�����Ⱦ
	//ChildrenSceneObjectsSet::iterator iter;
	//for (iter = m_ChildrenObjects.begin();
	//	iter != m_ChildrenObjects.end();
	//	iter++)
	//{
	//	(*iter)->Render();
	//}

	// for test only, turn off debug render each frame
	//m_DebugRender = false;
}

void BaseSceneObject::DebugRender()
{
	// render obb, for test only
	renderer->RenderAABB(m_OBB.localMin, m_OBB.localMax, Color4f(0.0f, 1.0f, 0.0f), m_WorldTransform);
	renderer->RenderAABB(m_AABB.worldMin, m_AABB.worldMax, Color4f(1.0f, 1.0f, 0.0f));
}

void BaseSceneObject::PrepareRenderObjects(ChildrenSceneObjectsSet& objects)
{
	// Base method, just add object to the list...
	// this should be overrided in derived classes

	if (!m_Visible) return;

	objects.insert(this);
}

void BaseSceneObject::CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, int type, int collisionGroup)
{
	// TODO: �����������������ཻ������
	CollisionInfo info;

	if ((collisionGroup & m_CollisionGroupMask) && (type & GetCollisionType()) && IntersectsRay(ray, info, type))
	{
		baseSceneObjects.push_back(info);
	}

	ChildrenSceneObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->CollectRayPickingSceneObject(ray, baseSceneObjects, type, collisionGroup);
	}
}

bool BaseSceneObject::IntersectsRay(const Ray& ray, CollisionInfo& info, int type)
{
	// ʹ�ð�Χ����Ϊ���߼�ѡ������
	//bool result = ray.IntersectsSphere(this->m_WorldTransform.GetPosition(), m_BoundingSphereRadius);
	//if (result)
	//{
	//	info.point = this->m_WorldTransform.GetPosition();
	//	// this hacks...
	//	info.normal = Vector3f(0.0f, 1.0f, 0.0f);
	//	info.squaredDistance = (info.point - ray.origin).SquaredLength();
	//	info.obj = this;
	//}

	// ʹ��AABB��ѡ����
	Vector3f p;
	float d;
	bool result = ray.IntersectsBox(m_AABB.worldMin, m_AABB.worldMax, p, d);

	if (result)
	{
		info.point = p;
		info.squaredDistance = d * d;
		info.normal = Vector3f(0.0f, 1.0f, 0.0f);
		info.obj = this;
	}

	return result;
}


void BaseSceneObject::AttachChildObject(BaseSceneObject* child, bool keepRotation, bool keepWorldTransform)
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

void BaseSceneObject::DetachFromParent(bool keepWorldTransform)
{
	if (keepWorldTransform)
	{
		// TODO: �޸�Transform
	}

	// �Ӹ�������Ӷ����б����Ƴ��Լ�
	m_ParentObject->m_ChildrenObjects.erase(this);
	m_ParentObject = NULL;
}

void BaseSceneObject::SetPosition(const Vector3f& pos)
{
	m_Transform.SetPosition(pos);
}

void BaseSceneObject::SetRotation(const Matrix3& rot)
{
	m_Transform.SetRotation(rot);
}

void BaseSceneObject::SetParentObject(BaseSceneObject* parent, bool keepRotation)
{
	m_ParentObject = parent;
	m_KeepRotation = keepRotation;
}

Matrix4& BaseSceneObject::Transform()
{
	return m_Transform;
}

const Matrix4& BaseSceneObject::WorldTransform() const
{
	return m_WorldTransform;
}

void BaseSceneObject::SetVisibleRecursively(bool visible)
{
	ChildrenSceneObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->SetVisibleRecursively(visible);
	}
}

