//-----------------------------------------------------------------------------------
/// IRenderableObject.h 可渲染对象接口
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IRENDERABLEOBJECT_H
#define _IRENDERABLEOBJECT_H

#include "OBB.h"
#include "AABB.h"
#include "Matrix4.h"
#include "Material.h"

// 单一可渲染物体，以Material分组
class IRenderableObject
{
public:
	virtual ~IRenderableObject() {}

	// 渲染物体
	virtual void RenderSingleObject() = 0;

	// 获取材质
	//virtual Material* GetMaterial() = 0;

	// 是否透明
	virtual bool IsTransparent() = 0;

	virtual	void SetRenderOrder(unsigned int order) = 0;
};

#endif
