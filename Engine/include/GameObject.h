//-----------------------------------------------------------------------------------
/// GameObject.h ��Ϸ������࣬��Ϸ��ʹ�õ��Ķ����ɴ���̳�
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
	///	��Ϸ����
	///	@par
	///		��Ϸ�����Ǿ�����Ϸ�߼��ĳ����и���
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
		///	Ϊ��Ⱦ����׼������
		/// @par
		///		��Ϸ���󽫸��ݹ��չ��������ù���״̬
		void SetupLightForRendering();

		// ----- GameObject Methods

		/// @brief
		/// ��ȡ�����ð�Χ��뾶
		float GetBoundingRadius() const { return m_BoundingSphereRadius; }

		/// @brief
		///	����Ϸ���������һ����Ϸ���
		//void AddComponent(RefPtr<GameComponent> component);
	protected:
		// lighting

		// TODO: �������������Կ�����SceneGraph��Ⱦʱ��ɣ�

		/// @brief
		///	��ӶԸö�����Ч�Ĺ�Դ
		/// @param light
		///		��ӵĹ�Դ
		/// @remarks
		///		��������ɹ��չ��������ã�����Ϊ�������ù���״̬
		void AddEffectiveLight(Light* light);

		/// @brief
		///	�����Ч��Դ�б�
		void ClearEffectiveLights();

	protected:
		float							m_BoundingSphereRadius;		///< ��Χ��뾶(������)

		typedef std::vector<Light*>			VecLights;
		VecLights						m_Lights;		///< Ӱ�����Ĺ�Դ�б�

		//typedef std::map< const String, RefPtr<GameComponent> >	MapComponent;
		//MapComponent					m_GameComponents;
	};
}

#endif
