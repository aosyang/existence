//-----------------------------------------------------------------------------------
/// LightableObject.cpp 可照亮对象基类
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "LightableObject.h"
#include "Engine.h"

LightableObject::LightableObject()
: m_Moved(true)
{
}

void LightableObject::AddLight(Light* light)
{
	// TODO: 判断最大受影响光源数量

	if (m_Lights.size()>=renderer->GetMaxLightsNumber())
		return;

	m_Lights.push_back(light);
}

void LightableObject::ClearLights()
{
	m_Lights.clear();
}

// 设置渲染器的灯光状态
void LightableObject::SetupLights()
{
	int maxLightNum = renderer->GetMaxLightsNumber();

	int i;
	for (i=0; i<m_Lights.size() && i<maxLightNum; i++)
	{
		renderer->SetupLight(i, m_Lights[i]);
	}

	for (; i<maxLightNum; i++)
	{
		renderer->SetupLight(i, NULL);
	}
}

