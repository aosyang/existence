//-----------------------------------------------------------------------------------
/// GameObjectBase.h ��Ϸ������࣬��Ϸ��ʹ�õ��Ķ����ɴ���̳�
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _GAMEOBJECTBASE_H
#define _GAMEOBJECTBASE_H

#include "SceneObject.h"

// ÿ����Ϸ�еĶ����ɴ�������
// ��Ϸ�������
// ����������ײ�����µȵ�
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
