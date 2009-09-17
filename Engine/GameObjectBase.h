//-----------------------------------------------------------------------------------
/// GameObjectBase.h 游戏对象基类，游戏中使用到的对象由此类继承
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _GAMEOBJECTBASE_H
#define _GAMEOBJECTBASE_H

#include "SceneObject.h"

// 每个游戏中的对象由此类派生
// 游戏物体基类
// 负责对象的碰撞、更新等等
class GameObjectBase : public SceneObject
{
public:
	GameObjectBase();

	// ----- Overwrite IObject
	//void Update(unsigned long deltaTime);

	const String GetTypeName() const { return "GameObjectBase"; }

	// ----- Overwrite SceneObject
	//void CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum);

protected:
};

#endif
