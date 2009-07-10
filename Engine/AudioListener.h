//-----------------------------------------------------------------------------------
/// AudioListener.h ��Ƶ������
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
/// ��Ƶ������
/// 
/// �ṩ�����ߵ�λ����Ϣ
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
