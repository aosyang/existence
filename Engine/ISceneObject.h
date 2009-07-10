//-----------------------------------------------------------------------------------
/// ISceneObject.h 场景对象接口，所有渲染/更新的对象继承自此接口
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _ISCENEOBJECT_H
#define _ISCENEOBJECT_H

#include "Matrix4.h"

class ISceneObject
{
public:
	virtual ~ISceneObject() {}

	virtual void Update(unsigned long deltaTime) = 0;
	virtual void Render() = 0;
	virtual void DebugRender() = 0;

	//virtual void AttachChildObject(ISceneObject* child, bool keepWorldTransform) = 0;

	//virtual Matrix4& Transform() = 0;
	//virtual Matrix4& WorldTransform() = 0;

//protected:
//	virtual void SetParentObject(ISceneObject* parent) = 0;
};

#endif

