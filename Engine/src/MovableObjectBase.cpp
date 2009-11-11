//-----------------------------------------------------------------------------------
/// MovableObjectBase.cpp 可移动对象基类的实现代码
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "MovableObjectBase.h"
#include "Debug.h"

namespace Gen
{
	MovableObjectBase::MovableObjectBase()
		: m_Updated(false)
	{
		// OBB默认尺寸，长宽高各为1
		m_OBB.localMin = Vector3f(-0.5f, -0.5f, -0.5f);
		m_OBB.localMax = Vector3f(0.5f, 0.5f, 0.5f);

		m_Transform.MakeIdentity();
		m_WorldTransform.MakeIdentity();
	}

	void MovableObjectBase::Destroy()
	{
	}

	void MovableObjectBase::Update(unsigned long deltaTime)
	{
		// 先标记自己为已更新，否则会造成循环引用
		//   注：尽管如此，循环引用依旧会造成不正确的结果
		m_Updated = true;
		m_AABB.Reset();
	}

	// 对象在这个周期内是否已经被更新过
	bool MovableObjectBase::IsUpdated()
	{
		return m_Updated;
	}

	void MovableObjectBase::SetPosition(const Vector3f& pos)
	{
		m_Transform.SetPosition(pos);
	}

	void MovableObjectBase::SetRotation(const Quaternion& rot)
	{
		m_Rotation = rot;
		m_Transform.SetRotation(rot.GetRotationMatrix());
	}

	void MovableObjectBase::RotateLocal(const Quaternion& quat)
	{
		m_Rotation *= quat;
		m_Transform.SetRotation(m_Rotation.GetRotationMatrix());
	}

	void MovableObjectBase::TranslateLocal(const Vector3f& trans)
	{
		m_Transform.TranslateLocal(trans);
	}

	void MovableObjectBase::TranslateParent(const Vector3f& trans)
	{
		m_Transform.TranslateWorld(trans);
	}

	const AABB& MovableObjectBase::GetAABB()
	{
		return m_AABB;
	}

	const OBB& MovableObjectBase::GetOBB()
	{
		return m_OBB;
	}


	bool MovableObjectBase::IsCulled(Frustum* frustum)
	{
		if (!frustum) return false;

		if (frustum->IntersectsAABB(m_AABB)) return false;

		return true;
	}
}
