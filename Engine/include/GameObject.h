//-----------------------------------------------------------------------------------
/// GameObject.h 游戏对象基类，游戏中使用到的对象由此类继承
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "SceneObject.h"
#include "Light.h"
#include "RefPtr.h"

#include <vector>

namespace Gen
{
	class LightingManager;
	//class GameComponent;

	/// @brief
	///	游戏对象
	///	@par
	///		游戏对象是具有游戏逻辑的场景中个体
	class GameObject : public SceneObject
	{
		DECLARE_FACTORY_OBJECT(GameObject, SceneObject)
		friend class LightingManager;
	public:
		// ----- Overwrite IObject
		//void Update(unsigned long deltaTime);

		//const String GetTypeName() const { return "GameObject"; }

		void Destroy();

		// ----- Overwrite SceneObject

		/// @brief
		///	为渲染对象准备光照
		/// @par
		///		游戏对象将根据光照管理器设置光照状态
		void SetupLightForRendering();

		// ----- GameObject Methods

		/// @brief
		/// 获取光照用包围球半径
		float GetBoundingRadius() const { return m_BoundingSphereRadius; }

		/// @brief
		///	向游戏对象中添加一个游戏组件
		//void AddComponent(RefPtr<GameComponent> component);
	protected:
		// lighting

		// TODO: 这两个方法可以考虑在SceneGraph渲染时完成？

		/// @brief
		///	添加对该对象有效的光源
		/// @param light
		///		添加的光源
		/// @remarks
		///		这个方法由光照管理器调用，用于为对象设置光照状态
		void AddEffectiveLight(Light* light);

		/// @brief
		///	清除有效光源列表
		void ClearEffectiveLights();

	protected:
		float							m_BoundingSphereRadius;		///< 包围球半径(光照用)

		typedef std::vector<Light*>			VecLights;
		VecLights						m_Lights;		///< 影响对象的光源列表

		//typedef std::map< const String, RefPtr<GameComponent> >	MapComponent;
		//MapComponent					m_GameComponents;
	};
}

#endif
