//-----------------------------------------------------------------------------------
/// BaseLightableObject.cpp �������������
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
	// TODO: �ж������Ӱ���Դ����

	if (m_Lights.size()>=renderer->GetMaxLightsNumber())
		return;

	m_Lights.push_back(light);
}

void BaseLightableObject::ClearLights()
{
	m_Lights.clear();
}

// ������Ⱦ���ĵƹ�״̬
void BaseLightableObject::SetupLights()
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

