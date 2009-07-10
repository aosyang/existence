//-----------------------------------------------------------------------------------
/// BaseSceneObject.h ����������࣬���������������������ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _BASESCENEOBJECT_H
#define _BASESCENEOBJECT_H

#include "ISceneObject.h"
#include "Ray.h"
#include "OBB.h"
#include "AABB.h"

#include <set>
#include <vector>

using namespace std;

class BaseSceneObject;

//typedef vector<BaseSceneObject*>	VecBaseSceneObjects;
typedef set<BaseSceneObject*>		ChildrenSceneObjectsSet;

enum CollisionType
{
	COLLISION_TYPE_MESH = 1 << 0,
	COLLISION_TYPE_CAMERA = 1 << 1,
	COLLISION_TYPE_BSP = 1 << 2,

	// collide with everything...
	COLLISION_ALL = COLLISION_TYPE_MESH | COLLISION_TYPE_CAMERA | COLLISION_TYPE_BSP,
};

struct CollisionInfo
{
	Vector3f point, normal;
	float squaredDistance;
	BaseSceneObject* obj;
};

typedef vector<CollisionInfo>		ObjectsCollisionInfos;

//-----------------------------------------------------------------------------------
/// \brief
/// �����������
/// 
/// ����������ӵ������Ķ���Ļ�����Ϊ��λ�ơ����ӽ���
/// 
/// \remarks
/// ���п���ֱ����ӵ������еĶ���Ӧ���ɴ���̳�
//-----------------------------------------------------------------------------------
class BaseSceneObject : public ISceneObject
{
public:
	BaseSceneObject();
	~BaseSceneObject();

	// ----- Overwrite ISceneObject

	void Update(unsigned long deltaTime);
	void Render();
	void DebugRender();

	// ----- BaseSceneObject Methods

	virtual void PrepareRenderObjects(ChildrenSceneObjectsSet& objects);

	void CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, CollisionType type);
	virtual bool IntersectsRay(const Ray& ray, CollisionInfo& info, CollisionType type);

	// ������������Ϊ�ö�����Ӷ���
	void AttachChildObject(BaseSceneObject* child, bool keepWorldTransform = false);
	void DetachFromParent(bool keepWorldTransform);

	virtual void SetPosition(const Vector3f& pos);
	virtual void SetRotation(const Matrix3& rot);

	Matrix4& Transform();
	const Matrix4& WorldTransform() const;

	void SetDebugRender(bool render) { m_DebugRender = render; }
	bool GetDebugRender() const { return m_DebugRender; }

	float GetBoundingRadius() const { return m_BoundingSphereRadius; }

	void SetVisible(bool visible) { m_Visible = visible; }
	bool GetVisible() const { return m_Visible; }

	void SetVisibleRecursively(bool visible);

	void SetRenderOrder(unsigned int order) { m_RenderOrder = order; }
	inline unsigned int GetRenderOrder() const { return m_RenderOrder; }

protected:

	// ָ��������
	void SetParentObject(BaseSceneObject* parent);


protected:
	ChildrenSceneObjectsSet			m_ChildrenObjects;
	BaseSceneObject*				m_ParentObject;

	Matrix4							m_Transform;				///< ��¼���󱾵ؿռ�任
	Matrix4							m_WorldTransform;			///< ��¼��������ռ�任��ע��ͨ������Ҫ�ֶ����£���Update��������Ӹ��������

	bool							m_DebugRender;
	float							m_BoundingSphereRadius;		///< ��Χ��뾶

	bool							m_Visible;					///< �ɼ���

	OBB								m_OBB;
	AABB							m_AABB;

	unsigned int					m_RenderOrder;
};

#endif
