//-----------------------------------------------------------------------------------
/// Collision.h 碰撞，碰撞检测结果信息
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _COLLISION_H
#define _COLLISION_H

#include "SceneObject.h"

#include <vector>



namespace Gen
{
	class SceneObject;

	// 碰撞检测结果
	struct CollisionResult
	{
		Vector3f point, normal;
		float distance;
		SceneObject* obj;
	};

	typedef std::vector<CollisionResult>		ObjectsCollisionInfos;

	enum CollisionGroup
	{
		COLLISION_GROUP_ALL = 0xFFFFFFFF,
	};
};

#endif
