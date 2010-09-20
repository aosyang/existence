//-----------------------------------------------------------------------------------
/// MovableObjectBase.cpp ���ƶ���������ʵ�ִ���
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
	{
		// OBBĬ�ϳߴ磬����߸�Ϊ1
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
		m_AABB.Reset();
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

}
