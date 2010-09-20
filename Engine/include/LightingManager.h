//-----------------------------------------------------------------------------------
/// LightManager.h ���չ�����
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

	// ��̬�����붯̬����
	// ��̬�����뾲̬����
	// ��̬�����붯̬����

	// ��̬����<==>���ж���
	// ��̬����<==>���й���

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ���չ�����
	/// 
	/// ����̬����̬��Դ�Լ������������壬����������������ϵ
	//-----------------------------------------------------------------------------------
	class LightingManager : public Singleton<LightingManager>
	{
		friend class Singleton<LightingManager>;
	public:
		void AddLightableObject(GameObject* lightableObj);
		void RemoveLightableObject(GameObject* lightableObj);

		void AddLight(Light* light);

		// ���ȫ�ֹ��գ��ƹ⽫��������ÿһ��������
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
