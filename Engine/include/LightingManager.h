//-----------------------------------------------------------------------------------
/// LightManager.h 光照管理器
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _LIGHTINGMANAGER_H
#define _LIGHTINGMANAGER_H

#include "Singleton.h"

//#include <vector>
//#include <list>
#include <set>



namespace Gen
{
	class Light;
	class GameObject;

	typedef std::set<GameObject*>					LightableObjectList;
	typedef std::set<Light*>							LightList;

	// 静态光照与动态对象
	// 动态光照与静态对象
	// 动态光照与动态对象

	// 动态光照<==>所有对象
	// 动态物体<==>所有光照

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 光照管理器
	/// 
	/// 管理动态、静态光源以及被照亮的物体，并建立其间的照亮关系
	//-----------------------------------------------------------------------------------
	class LightingManager : public Singleton<LightingManager>
	{
		friend class Singleton<LightingManager>;
	public:
		void AddLightableObject(GameObject* lightableObj);
		void RemoveLightableObject(GameObject* lightableObj);

		void AddLight(Light* light);

		// 添加全局光照，灯光将会作用在每一个对象上
		void AddGlobalLight(Light* light);
		void RemoveLight(Light* light);

		void Update();
	private:
		LightingManager();

		LightableObjectList		m_LightableObjects;
		LightList				m_Lights;
		LightList				m_GlobalLights;
	};
}

#endif
