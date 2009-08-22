//-----------------------------------------------------------------------------------
/// BaseSceneObject.h 场景对象基类，包含场景对象基本方法的实现
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
/// 场景对象基类
/// 
/// 包含可以添加到场景的对象的基础行为：位移、绑定子结点等
/// 
/// \remarks
/// 所有可以直接添加到场景中的对象应该由此类继承
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

	// 绑定其他对象作为该对象的子对象
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

	// 指定父对象
	void SetParentObject(BaseSceneObject* parent, bool keepRotation);

	// 判断物体是否可以被剔除
	// 重载这个方法以实现不同的剔除方案
	virtual bool IsCulled(const RenderView& view);

protected:
	ChildrenSceneObjectsSet			m_ChildrenObjects;
	BaseSceneObject*				m_ParentObject;

	Quaternion						m_Rotation;					///< 记录对象局部空间旋转的四元数
	Matrix4							m_Transform;				///< 记录对象局部空间变换
	Matrix4							m_WorldTransform;			///< 记录对象世界空间变换，注：通常不需要手动更新，由Update方法负责从父对象更新

	bool							m_KeepRotation;

	bool							m_DebugRender;
	float							m_BoundingSphereRadius;		///< 包围球半径

	bool							m_Visible;					///< 可见性
	SceneGraph*						m_Scene;					///< 该物体所属的场景

	OBB								m_OBB;
	AABB							m_AABB;

	unsigned int					m_RenderOrder;

	int								m_CollisionGroupMask;
};

#endif
