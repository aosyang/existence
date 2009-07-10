//-----------------------------------------------------------------------------------
/// LightManager.cpp ���չ�����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "LightingManager.h"

LightingManager::LightingManager()
{
}

void LightingManager::AddLightableObject(BaseLightableObject* lightableObj)
{
	if (m_LightableObjects.find(lightableObj)==m_LightableObjects.end())
		m_LightableObjects.insert(lightableObj);
}

void LightingManager::RemoveLightableObject(BaseLightableObject* lightableObj)
{
	LightableObjectList::iterator iter;
	if ((iter = m_LightableObjects.find(lightableObj))!=m_LightableObjects.end())
	{
		m_LightableObjects.erase(iter);
	}
}

void LightingManager::AddLight(Light* light)
{
	if (m_Lights.find(light)==m_Lights.end())
		m_Lights.insert(light);
}

void LightingManager::AddGlobalLight(Light* light)
{
	if (m_GlobalLights.find(light)==m_GlobalLights.end())
		m_GlobalLights.insert(light);
}

void LightingManager::RemoveLight(Light* light)
{
	LightList::iterator iter;
	if ((iter = m_Lights.find(light))!=m_Lights.end())
	{
		m_Lights.erase(iter);
	}
	/*else */if ((iter = m_GlobalLights.find(light))!=m_GlobalLights.end())
	{
		m_GlobalLights.erase(iter);
	}
}

// �������еƹ��뱻���������Ĺ�ϵ
void LightingManager::Update()
{
	//LightableObjectList	staticLightableObjs;
	//LightableObjectList	dynamicLightableObjs;

	//VecLights			staticLights;
	//VecLights			dynamicLights;

	//// �жϳ�����ÿ���ƹ��Ƿ��ƶ���
	//for (LightableObjectList::iterator iter=m_LightableObjects.begin();
	//	iter!=m_LightableObjects.end();
	//	iter++)
	//{
	//	if ((*iter)->HasMoved())
	//	{
	//		dynamicLightableObjs.push_back(*iter);
	//	}
	//	else
	//	{
	//		staticLightableObjs.push_back(*iter);
	//	}
	//}

	//// �ж�ÿ�������Ƿ��ƶ���
	//for (VecLights::iterator iter=m_Lights.begin();
	//	iter!=m_Lights.end();
	//	iter++)
	//{
	//	if ((*iter)->HasMoved())
	//	{
	//		dynamicLights.push_back(*iter);
	//	}
	//	else
	//	{
	//		staticLights.push_back(*iter);
	//	}
	//}

	// hack: ���ж�������й��ս��бȽ�

	// TODO: ֻ��Ҫ���ɼ������ܹ����
	for(LightableObjectList::iterator iter_obj=m_LightableObjects.begin();
		iter_obj!=m_LightableObjects.end();
		iter_obj++)
	{
		(*iter_obj)->ClearLights();

		// ʹ�����ܵ�ȫ�ֹ��յ�Ӱ��
		for (LightList::iterator iter_light=m_GlobalLights.begin();
			iter_light!=m_GlobalLights.end();
			iter_light++)
		{
				(*iter_obj)->AddLight(*iter_light);
		}

		// �ֲ�����Ӱ��
		for (LightList::iterator iter_light=m_Lights.begin();
			iter_light!=m_Lights.end();
			iter_light++)
		{
			Vector3f light_pos = (*iter_light)->WorldTransform().GetPosition();
			Vector3f obj_pos = (*iter_obj)->WorldTransform().GetPosition();

			float squaredRadius = (*iter_light)->GetRange() + (*iter_obj)->GetBoundingRadius();
			squaredRadius *= squaredRadius;
			if ((light_pos - obj_pos).SquaredLength() < squaredRadius)
			{
				(*iter_obj)->AddLight(*iter_light);
			}
		}
	}
}
