//-----------------------------------------------------------------------------------
/// LightableObject.h 可照亮对象基类
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _LIGHTABLEOBJECT_H
#define _LIGHTABLEOBJECT_H

#include "Light.h"
#include "RenderableObjectBase.h"

#include <vector>

using namespace std;

typedef vector<Light*>		VecLights;

//-----------------------------------------------------------------------------------
/// \brief
/// 可照亮对象基类
/// 
/// 记录了可以被照亮的物体受到了哪些光源的影响
/// 
/// \remarks
/// 任何能够受到灯光影响的对象应该继承自此类
//-----------------------------------------------------------------------------------
class LightableObject
{
public:
	LightableObject();

	void AddLight(Light* light);
	void ClearLights();

	void SetupLights();

	void SetMoved(bool moved) { m_Moved = moved; }
	bool HasMoved() const { return m_Moved; }

	void SetRenderableObject(RenderableObjectBase* rendeable) { m_RenderableObject = rendeable; }
	RenderableObjectBase* GetRenderableObject() { return m_RenderableObject; }

protected:
	bool		m_Moved;		///< 对象是否移动过
	VecLights	m_Lights;		///< 影响对象的光源列表

	RenderableObjectBase*	m_RenderableObject;	///< 被照亮的物体
};

#endif
