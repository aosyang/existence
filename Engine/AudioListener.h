//-----------------------------------------------------------------------------------
/// AudioListener.h 音频收听者
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _AUDIOLISTENER_H
#define _AUDIOLISTENER_H

#include "BaseSceneObject.h"

//-----------------------------------------------------------------------------------
/// \brief
/// 音频收听者
/// 
/// 提供收听者的位置信息
//-----------------------------------------------------------------------------------
class AudioListener : public BaseSceneObject
{
public:
	AudioListener();
	~AudioListener();

	// ----- Overwrite ISceneObject

	void Update(unsigned long deltaTime);
	void DebugRender();

	// ----- Overwrite BaseSceneObject

	bool IntersectsRay(const Ray& ray, CollisionInfo& info, CollisionType type);

};

#endif
