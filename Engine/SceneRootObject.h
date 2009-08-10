//-----------------------------------------------------------------------------------
/// SceneRootObject.h 根对象，场景所有对象挂载在这个对象上
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _SCENEROOTOBJECT_H
#define _SCENEROOTOBJECT_H

#include "ISceneObject.h"
#include "BaseSceneObject.h"
#include "Ray.h"

#include <set>
#include <vector>

using namespace std;

class BaseSceneObject;

class SceneRootObject : public ISceneObject
{
public:
	~SceneRootObject() {}

	// ----- Overwrite ISceneObject

	void Update(unsigned long deltaTime);
	void Render();
	void DebugRender() {}

	// ----- SceneRootObject Methods

	// 收集渲染对象，准备渲染
	void CollectRenderObjects();

	void AttachChildObject(BaseSceneObject* object);
	bool DetachChildObject(BaseSceneObject* object);

	void CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, int type, int collisionGroup);
private:

	ChildrenSceneObjectsSet	m_ChildrenObjects;
	SceneObjectList  m_RenderObjects;
};

#endif
