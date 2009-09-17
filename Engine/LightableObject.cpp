//-----------------------------------------------------------------------------------
/// LightableObject.cpp �������������
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
	// TODO: �ж������Ӱ���Դ����

	if (m_Lights.size()>=renderer->GetMaxLightsNumber())
		return;

	m_Lights.push_back(light);
}

void LightableObject::ClearLights()
{
	m_Lights.clear();
}

// ������Ⱦ���ĵƹ�״̬
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

