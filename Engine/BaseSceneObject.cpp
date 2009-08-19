//-----------------------------------------------------------------------------------
/// BaseSceneObject.cpp 场景对象基类，包含场景对象基本方法的实现
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
	// 注：对于每个对象的更新顺序
	// 
	// 更新自身矩阵
	// 更新自身状态
	// 更新子对象

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

	// 重新根据obb计算aabb
	m_AABB.Reset();
	m_AABB.Expand(m_OBB, m_WorldTransform);


	ChildrenSceneObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->Update(deltaTime);

		// 将子对象的aabb也包括进去
		m_AABB.Expand((*iter)->m_AABB);
	}
}

void BaseSceneObject::Render()
{
	if (Engine::Instance().GetDebugRender() || m_DebugRender)
		DebugRender();
}

// 渲染调试信息
void BaseSceneObject::DebugRender()
{
	// render obb, for test only

	// 渲染OBB
	renderer->RenderBox(m_OBB.localMin, m_OBB.localMax, Color4f(0.0f, 1.0f, 0.0f), m_WorldTransform);

	// 渲染AABB
	renderer->RenderBox(m_AABB.worldMin, m_AABB.worldMax, Color4f(1.0f, 1.0f, 0.0f));
}

void BaseSceneObject::PrepareRenderObjects(SceneObjectList& objects, const RenderView& view)
{
	// Base method, just add object to the list...
	// this should be overrided in derived classes

	if (!IsCulled(view))
		objects.push_back(this);
	
	// 递归添加子对象
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
	// TODO: 这里添加射线与对象相交检测代码
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
	// 使用包围球作为射线拣选的依据
	//bool result = ray.IntersectsSphere(this->m_WorldTransform.GetPosition(), m_BoundingSphereRadius);
	//if (result)
	//{
	//	info.point = this->m_WorldTransform.GetPosition();
	//	// this hacks...
	//	info.normal = Vector3f(0.0f, 1.0f, 0.0f);
	//	info.squaredDistance = (info.point - ray.origin).SquaredLength();
	//	info.obj = this;
	//}

	// 使用AABB拣选对象
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

void BaseSceneObject::DetachFromParent(bool keepWorldTransform)
{
	if (keepWorldTransform)
	{
		// TODO: 修改Transform
	}

	// 从父对象的子对象列表中移除自己
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
/// 为场景物体指定父对象
/// 
/// \param parent
/// 新的父对象
/// 
/// \param keepRotation
/// 子对象是否采用自身的旋转矩阵
//-----------------------------------------------------------------------------------
void BaseSceneObject::SetParentObject(BaseSceneObject* parent, bool keepRotation)
{
	m_ParentObject = parent;
	m_KeepRotation = keepRotation;
}


bool BaseSceneObject::IsCulled(const RenderView& view)
{
	// 不可见
	if (!m_Visible) return true;

	// 没有指定视截体
	if (!view.frustum) return false;

	// AABB与视截体相交判定
	if (view.frustum->IntersectsAABB(m_AABB)) return false;

	return true;
}

