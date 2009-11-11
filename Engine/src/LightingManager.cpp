//-----------------------------------------------------------------------------------
/// LightManager.cpp 光照管理器
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "LightingManager.h"

namespace Gen
{
	LightingManager::LightingManager()
	{
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 向管理器中添加一个可照亮的物体
	/// 
	/// \param lightableObj
	/// 要添加的物体
	/// 
	/// \remarks
	/// 没有添加到管理器的物体不会受到光照影响
	//-----------------------------------------------------------------------------------
	void LightingManager::AddLightableObject(LightableObject* lightableObj)
	{
		if (m_LightableObjects.find(lightableObj)==m_LightableObjects.end())
			m_LightableObjects.insert(lightableObj);
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 从管理器中删除一个可照亮的物体
	/// 
	/// \param lightableObj
	/// 要删除的物体
	/// 
	/// 物体将不会再受到光照影响
	//-----------------------------------------------------------------------------------
	void LightingManager::RemoveLightableObject(LightableObject* lightableObj)
	{
		LightableObjectList::iterator iter;
		if ((iter = m_LightableObjects.find(lightableObj))!=m_LightableObjects.end())
		{
			m_LightableObjects.erase(iter);
		}
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 向管理器中添加灯光
	/// 
	/// \param light
	/// 要添加的灯光
	/// 
	/// 添加一个灯光使其可以作用在范围内的物体上
	/// 
	/// \remarks
	/// 不添加到管理器的灯光，光照效果将不会作用在周围物体上
	//-----------------------------------------------------------------------------------
	void LightingManager::AddLight(Light* light)
	{
		if (m_Lights.find(light)==m_Lights.end())
			m_Lights.insert(light);
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 添加全局灯光
	/// 
	/// \param light
	/// 要添加的全局灯光
	/// 
	/// 添加一个会作用在管理器内所有可照亮物体上的灯光
	/// 
	/// \remarks
	/// 诸如模拟太阳光的方向光源可以作为全局灯光添加。<br>
	/// 全局灯光将会优先于物体的局部灯光照亮物体
	//-----------------------------------------------------------------------------------
	void LightingManager::AddGlobalLight(Light* light)
	{
		if (m_GlobalLights.find(light)==m_GlobalLights.end())
			m_GlobalLights.insert(light);
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 从管理器中删除一个灯光
	/// 
	/// \param light
	/// 要删除的灯光
	/// 
	/// \remarks
	/// 全局灯光也使用这个方法删除
	//-----------------------------------------------------------------------------------
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

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 更新管理器
	/// 
	/// 更新管理器中所有灯光与被照亮物体的关联
	/// 
	/// \remarks
	/// 这个方法需要在渲染场景之前完成
	//-----------------------------------------------------------------------------------
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
				Vector3f obj_pos = (*iter_obj)->GetRenderableObject()->WorldTransform().GetPosition();

				float squaredRadius = (*iter_light)->GetRange() + (*iter_obj)->GetRenderableObject()->GetBoundingRadius();
				squaredRadius *= squaredRadius;
				if ((light_pos - obj_pos).SquaredLength() < squaredRadius)
				{
					(*iter_obj)->AddLight(*iter_light);
				}
			}
		}
	}
}
