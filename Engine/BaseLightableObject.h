//-----------------------------------------------------------------------------------
/// BaseLightableObject.h 可照亮对象基类
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _BASELIGHTABLEOBJECT_H
#define _BASELIGHTABLEOBJECT_H

#include "BaseSceneObject.h"
#include "Light.h"

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
class BaseLightableObject : public BaseSceneObject
{
public:
	BaseLightableObject();

	void AddLight(Light* light);
	void ClearLights();

	void SetMoved(bool moved) { m_Moved = moved; }
	bool HasMoved() const { return m_Moved; }

protected:
	bool		m_Moved;		///< 对象是否移动过
	VecLights	m_Lights;		///< 影响对象的光源列表
};

#endif
