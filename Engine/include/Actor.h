//-----------------------------------------------------------------------------------
/// Actor.h ��Ϸ��̬����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ACTOR_H
#define _ACTOR_H

#include "GameObject.h"

namespace Gen
{
	// Actor����̬��Ϸ�������
	// �ڳ����п��ƶ�������������һ��
	class Actor : public GameObject
	{
		DECLARE_FACTORY_OBJECT(Actor, GameObject)
	};
}

#endif
