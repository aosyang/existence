//-----------------------------------------------------------------------------------
/// MovableObjectBase.h 可移动对象基类，实现了对象移动的最基本方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MOVABLEOBJECTBASE_H
#define _MOVABLEOBJECTBASE_H

#include "IObject.h"
#include "OBB.h"
#include "AABB.h"
#include "Vector3f.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Frustum.h"

#include <set>
#include <vector>

using namespace std;

class MovableObjectBase;
class RenderableObjectBase;

typedef set<RenderableObjectBase*>	RenderableObjectSet;
typedef vector<RenderableObjectBase*> RenderableObjectList;

class MovableObjectBase : public IObject
{
public:
	MovableObjectBase();

	// ----- Overwrite IObject

	void Destroy();

	void Update(unsigned long deltaTime);

	// 对象在这个周期内是否已经被更新过
	bool IsUpdated();

	const String GetTypeName() const { return "MovableObjectBase"; }

	// ----- MovableObjectBase methods

	// Transform
	virtual void SetPosition(const Vector3f& pos);
	virtual void SetRotation(const Quaternion& rot);

	void RotateLocal(const Quaternion& quat);
	void TranslateLocal(const Vector3f& trans);
	void TranslateParent(const Vector3f& trans);

	inline const Matrix4& Transform() const { return m_Transform; }
	inline const Matrix4& WorldTransform() const { return m_WorldTransform; }

	// 获取包围盒
	const AABB& GetAABB();

	const OBB& GetOBB();

	// Render Collection

	virtual bool IsCulled(Frustum* frustum);

protected:
	bool							m_Updated;

	Quaternion						m_Rotation;					///< 记录对象局部空间旋转的四元数
	Matrix4							m_Transform;				///< 记录对象局部空间变换
	Matrix4							m_WorldTransform;			///< 记录对象世界空间变换，注：通常不需要手动更新，由Update方法负责从父对象更新


	OBB								m_OBB;
	AABB							m_AABB;

};

#endif
