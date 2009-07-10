//-----------------------------------------------------------------------------------
/// LightManager.cpp 光照管理器
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

// 更新所有灯光与被照明物体间的关系
void LightingManager::Update()
{
	//LightableObjectList	staticLightableObjs;
	//LightableObjectList	dynamicLightableObjs;

	//VecLights			staticLights;
	//VecLights			dynamicLights;

	//// 判断场景中每个灯光是否移动过
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

	//// 判断每个对象是否移动过
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

	// hack: 所有对象和所有光照进行比较

	// TODO: 只需要检测可见对象受光情况
	for(LightableObjectList::iterator iter_obj=m_LightableObjects.begin();
		iter_obj!=m_LightableObjects.end();
		iter_obj++)
	{
		(*iter_obj)->ClearLights();

		// 使对象受到全局光照的影响
		for (LightList::iterator iter_light=m_GlobalLights.begin();
			iter_light!=m_GlobalLights.end();
			iter_light++)
		{
				(*iter_obj)->AddLight(*iter_light);
		}

		// 局部光照影响
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
