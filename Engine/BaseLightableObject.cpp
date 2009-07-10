//-----------------------------------------------------------------------------------
/// BaseLightableObject.cpp 可照亮对象基类
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "BaseLightableObject.h"
#include "Engine.h"

BaseLightableObject::BaseLightableObject()
: BaseSceneObject(),
  m_Moved(true)
{
}

void BaseLightableObject::AddLight(Light* light)
{
	// TODO: 判断最大受影响光源数量

	if (m_Lights.size()>=renderer->GetMaxLightsNumber())
		return;

	m_Lights.push_back(light);
}

void BaseLightableObject::ClearLights()
{
	m_Lights.clear();
}
