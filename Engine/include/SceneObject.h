//-----------------------------------------------------------------------------------
/// SceneObject.h 场景对象，添加到场景的对象继承自此类，包含了绑定子对象的方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _SCENEOBJECT_H
#define _SCENEOBJECT_H

#include "MovableObjectBase.h"
#include "Frustum.h"
#include "Collision.h"
#include "EString.h"

#include <set>
#include <list>
#include <map>

using namespace std;

class SceneGraph;
class SceneObject;
class RenderableObjectBase;
class SceneSerializer;





typedef SceneObject* (*FactoryCreateFuncPtr)();

// 在每个SceneObject的派生类中声明这个对象，事先创建该类对象的工厂
#define DECLARE_FACTORY(className) \
public:	\
	static SceneObject* FactoryCreateSceneObject() { \
		return new className(); \
	}

#define REGISTER_FACTORY(className) RegisterFactoryCreator(#className, &className::FactoryCreateSceneObject);

typedef map<const String, FactoryCreateFuncPtr>	SceneObjectFactoryCreators;

// 场景对象工厂，使用工厂方法创建对象
class SceneObjectFactory
{
public:
	static void RegisterFactoryCreator(const String& className, FactoryCreateFuncPtr ptrFunc);
	static void RegisterDefaultFactories();

	SceneObject* CreateSceneObject(const String& className);
protected:
	static SceneObjectFactoryCreators	m_FactoryCreators;
};





typedef set<RenderableObjectBase*> RenderableObjectSet;
typedef set<SceneObject*>	SceneObjectSet;
typedef list<SceneObject*>	SceneObjectList;

class SceneObject : public MovableObjectBase
{
	friend class SceneSerializer;
public:
	SceneObject();
	~SceneObject();

	// ----- Overwrite IObject
	void Update(unsigned long deltaTime);

	const String GetTypeName() const { return "SceneObject"; }

	// ----- Overwrite MovableObjectBase

	bool IsCulled(Frustum* frustum);

	// ----- SceneObject methods

	// 碰撞检测

	// AABB碰撞查询
	// virtual void QueryAABB

	// 可视性判断
	void SetVisible(bool visible) { m_Visible = visible; }
	bool GetVisible() const { return m_Visible; }

	void SetSceneGraph(SceneGraph* scene) { m_Scene = scene; }
	void SetSceneGraphRecursively(SceneGraph* scene);
	SceneGraph* GetSceneGraph() { return m_Scene; }

	virtual void CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum);


	// Collision
	void RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup);
	virtual bool IntersectsRay(const Ray& ray, CollisionResult& info);

	virtual bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius);

	int GetCollisionGroupMask() const { return m_CollisionGroupMask; }
	void SetCollisionGroupMask(int mask) { m_CollisionGroupMask = mask; }

	// Attachable

	// 绑定其他对象作为该对象的子对象
	void AttachChildObject(SceneObject* child, bool keepRotation = false, bool keepWorldTransform = false);
	void DetachFromParent(bool keepWorldTransform);

	// Debug

	// 递归dump场景物体信息
	void DumpToLogRecursively(int level);

protected:
	// 指定父对象
	void SetParentObject(SceneObject* parent, bool keepRotation);

protected:
	//// 添加一个可渲染物体
	//void AddRenderableObject(RenderableObjectBase* renderable);

	//// 删除一个可渲染物体
	//void RemoveRenderableObject(RenderableObjectBase* renderable);

protected:
	bool							m_Visible;					///< 可见性
	SceneGraph*						m_Scene;


	SceneObjectSet					m_ChildrenObjects;
	SceneObject*					m_ParentObject;

	bool							m_KeepRotation;
	int								m_CollisionGroupMask;
	//RenderableObjectSet				m_RenderableObjects;
};

#endif

