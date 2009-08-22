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
#include "Quaternion.h"

#include <set>
#include <vector>

using namespace std;

class BaseSceneObject;
class SceneGraph;
class Frustum;

//typedef vector<BaseSceneObject*>	VecBaseSceneObjects;
typedef set<BaseSceneObject*>		ChildrenSceneObjectsSet;
typedef vector<BaseSceneObject*>	SceneObjectList;

enum CollisionType
{
	COLLISION_TYPE_MESH = 1 << 0,
	COLLISION_TYPE_CAMERA = 1 << 1,
	COLLISION_TYPE_BSP = 1 << 2,

	// collide with everything...
	COLLISION_ALL = COLLISION_TYPE_MESH | COLLISION_TYPE_CAMERA | COLLISION_TYPE_BSP,
};

#define COLLISION_GROUP_ALL 0xFFFFFFFF

struct CollisionInfo
{
	Vector3f point, normal;
	float squaredDistance;
	BaseSceneObject* obj;
};


struct RenderView
{
	Vector3f position;
	Matrix4 viewMatrix;
	Matrix4 projMatrix;
	Frustum* frustum;
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
	friend class SceneGraph;
public:
	BaseSceneObject();
	~BaseSceneObject();

	// ----- Overwrite ISceneObject

	void Update(unsigned long deltaTime);
	void Render();
	void DebugRender();

	// ----- BaseSceneObject Methods

	void PrepareRenderObjects(SceneObjectList& objects, const RenderView& view);

	// collisions

	int GetCollisionGroupMask() const { return m_CollisionGroupMask; }
	void SetCollisionGroupMask(int mask) { m_CollisionGroupMask = mask; }

	virtual int GetCollisionType() const { return 0; }
	void CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, int type, int collisionGroup);
	virtual bool IntersectsRay(const Ray& ray, CollisionInfo& info);

	// ������������Ϊ�ö�����Ӷ���
	void AttachChildObject(BaseSceneObject* child, bool keepRotation = false, bool keepWorldTransform = false);
	void DetachFromParent(bool keepWorldTransform);

	virtual void SetPosition(const Vector3f& pos);
	virtual void SetRotation(const Quaternion& rot);

	void RotateLocal(const Quaternion& quat);
	void TranslateLocal(const Vector3f& trans);

	inline const Matrix4& Transform() const { return m_Transform; }
	inline const Matrix4& WorldTransform() const { return m_WorldTransform; }

	void SetDebugRender(bool render) { m_DebugRender = render; }
	bool GetDebugRender() const { return m_DebugRender; }

	float GetBoundingRadius() const { return m_BoundingSphereRadius; }

	AABB GetAABB() const { return m_AABB; }

	void SetVisible(bool visible) { m_Visible = visible; }
	bool GetVisible() const { return m_Visible; }

	void SetVisibleRecursively(bool visible);

	inline SceneGraph* GetScene() { return m_Scene; }

	void SetRenderOrder(unsigned int order) { m_RenderOrder = order; }
	inline unsigned int GetRenderOrder() const { return m_RenderOrder; }

protected:

	// ָ��������
	void SetParentObject(BaseSceneObject* parent, bool keepRotation);

	// �ж������Ƿ���Ա��޳�
	// �������������ʵ�ֲ�ͬ���޳�����
	virtual bool IsCulled(const RenderView& view);

protected:
	ChildrenSceneObjectsSet			m_ChildrenObjects;
	BaseSceneObject*				m_ParentObject;

	Quaternion						m_Rotation;					///< ��¼����ֲ��ռ���ת����Ԫ��
	Matrix4							m_Transform;				///< ��¼����ֲ��ռ�任
	Matrix4							m_WorldTransform;			///< ��¼��������ռ�任��ע��ͨ������Ҫ�ֶ����£���Update��������Ӹ��������

	bool							m_KeepRotation;

	bool							m_DebugRender;
	float							m_BoundingSphereRadius;		///< ��Χ��뾶

	bool							m_Visible;					///< �ɼ���
	SceneGraph*						m_Scene;					///< �����������ĳ���

	OBB								m_OBB;
	AABB							m_AABB;

	unsigned int					m_RenderOrder;

	int								m_CollisionGroupMask;
};

#endif
