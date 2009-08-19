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

	//m_Transform.SetRotation(m_Rotation.GetRotationMatrix());

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
}

// ��Ⱦ������Ϣ
void BaseSceneObject::DebugRender()
{
	// render obb, for test only

	// ��ȾOBB
	renderer->RenderBox(m_OBB.localMin, m_OBB.localMax, Color4f(0.0f, 1.0f, 0.0f), m_WorldTransform);

	// ��ȾAABB
	renderer->RenderBox(m_AABB.worldMin, m_AABB.worldMax, Color4f(1.0f, 1.0f, 0.0f));
}

void BaseSceneObject::PrepareRenderObjects(SceneObjectList& objects, const RenderView& view)
{
	// Base method, just add object to the list...
	// this should be overrided in derived classes

	if (!IsCulled(view))
		objects.push_back(this);
	
	// �ݹ�����Ӷ���
	ChildrenSceneObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->PrepareRenderObjects(objects, view);
	}
}

void BaseSceneObject::CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, int type, int collisionGroup)
{
	// TODO: �����������������ཻ������
	CollisionInfo info;

	if ((collisionGroup & m_CollisionGroupMask) && (type & GetCollisionType()) && IntersectsRay(ray, info))
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

bool BaseSceneObject::IntersectsRay(const Ray& ray, CollisionInfo& info)
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

void BaseSceneObject::SetRotation(const Quaternion& rot)
{
	m_Rotation = rot;
	m_Transform.SetRotation(rot.GetRotationMatrix());
}

void BaseSceneObject::RotateLocal(const Quaternion& quat)
{
	m_Rotation *= quat;
	m_Transform.SetRotation(m_Rotation.GetRotationMatrix());
}

void BaseSceneObject::TranslateLocal(const Vector3f& trans)
{
	m_Transform.TranslateWorld(trans);
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


//-----------------------------------------------------------------------------------
/// \brief
/// Ϊ��������ָ��������
/// 
/// \param parent
/// �µĸ�����
/// 
/// \param keepRotation
/// �Ӷ����Ƿ�����������ת����
//-----------------------------------------------------------------------------------
void BaseSceneObject::SetParentObject(BaseSceneObject* parent, bool keepRotation)
{
	m_ParentObject = parent;
	m_KeepRotation = keepRotation;
}


bool BaseSceneObject::IsCulled(const RenderView& view)
{
	// ���ɼ�
	if (!m_Visible) return true;

	// û��ָ���ӽ���
	if (!view.frustum) return false;

	// AABB���ӽ����ཻ�ж�
	if (view.frustum->IntersectsAABB(m_AABB)) return false;

	return true;
}

