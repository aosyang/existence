//-----------------------------------------------------------------------------------
/// Actor.h 游戏动态对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ACTOR_H
#define _ACTOR_H

#include "GameObjectBase.h"

namespace Gen
{
	// Actor，动态游戏对象基类
	// 在场景中可移动的物体属于这一类
	class Actor : public GameObjectBase
	{
		DECLARE_FACTORY_OBJECT(Actor, GameObjectBase)
	};
}

#endif
