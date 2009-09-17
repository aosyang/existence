//-----------------------------------------------------------------------------------
/// MovableObjectBase.h ���ƶ�������࣬ʵ���˶����ƶ������������
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

	// ����������������Ƿ��Ѿ������¹�
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

	// ��ȡ��Χ��
	const AABB& GetAABB();

	const OBB& GetOBB();

	// Render Collection

	virtual bool IsCulled(Frustum* frustum);

protected:
	bool							m_Updated;

	Quaternion						m_Rotation;					///< ��¼����ֲ��ռ���ת����Ԫ��
	Matrix4							m_Transform;				///< ��¼����ֲ��ռ�任
	Matrix4							m_WorldTransform;			///< ��¼��������ռ�任��ע��ͨ������Ҫ�ֶ����£���Update��������Ӹ��������


	OBB								m_OBB;
	AABB							m_AABB;

};

#endif
