//-----------------------------------------------------------------------------------
/// LightManager.cpp ���չ�����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "LightingManager.h"

namespace Gen
{
	LightingManager::LightingManager()
	{
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ������������һ��������������
	/// 
	/// \param lightableObj
	/// Ҫ��ӵ�����
	/// 
	/// \remarks
	/// û����ӵ������������岻���ܵ�����Ӱ��
	//-----------------------------------------------------------------------------------
	void LightingManager::AddLightableObject(LightableObject* lightableObj)
	{
		if (m_LightableObjects.find(lightableObj)==m_LightableObjects.end())
			m_LightableObjects.insert(lightableObj);
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// �ӹ�������ɾ��һ��������������
	/// 
	/// \param lightableObj
	/// Ҫɾ��������
	/// 
	/// ���彫�������ܵ�����Ӱ��
	//-----------------------------------------------------------------------------------
	void LightingManager::RemoveLightableObject(LightableObject* lightableObj)
	{
		LightableObjectList::iterator iter;
		if ((iter = m_LightableObjects.find(lightableObj))!=m_LightableObjects.end())
		{
			m_LightableObjects.erase(iter);
		}
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// �����������ӵƹ�
	/// 
	/// \param light
	/// Ҫ��ӵĵƹ�
	/// 
	/// ���һ���ƹ�ʹ����������ڷ�Χ�ڵ�������
	/// 
	/// \remarks
	/// ����ӵ��������ĵƹ⣬����Ч����������������Χ������
	//-----------------------------------------------------------------------------------
	void LightingManager::AddLight(Light* light)
	{
		if (m_Lights.find(light)==m_Lights.end())
			m_Lights.insert(light);
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ���ȫ�ֵƹ�
	/// 
	/// \param light
	/// Ҫ��ӵ�ȫ�ֵƹ�
	/// 
	/// ���һ���������ڹ����������п����������ϵĵƹ�
	/// 
	/// \remarks
	/// ����ģ��̫����ķ����Դ������Ϊȫ�ֵƹ���ӡ�<br>
	/// ȫ�ֵƹ⽫������������ľֲ��ƹ���������
	//-----------------------------------------------------------------------------------
	void LightingManager::AddGlobalLight(Light* light)
	{
		if (m_GlobalLights.find(light)==m_GlobalLights.end())
			m_GlobalLights.insert(light);
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// �ӹ�������ɾ��һ���ƹ�
	/// 
	/// \param light
	/// Ҫɾ���ĵƹ�
	/// 
	/// \remarks
	/// ȫ�ֵƹ�Ҳʹ���������ɾ��
	//-----------------------------------------------------------------------------------
	void LightingManager::RemoveLight(Light* light)
	{
		LightList::iterator iter;
		if ((iter = m_Lights.find(light))!=m_Lights.end())
		{
			m_Lights.erase(iter);
		}
		/*else */if ((iter = m_GlobalLights.find(light))!=m_GlobalLights.end())
		{
			m_GlobalLights.erase(iter);
		}
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ���¹�����
	/// 
	/// ���¹����������еƹ��뱻��������Ĺ���
	/// 
	/// \remarks
	/// ���������Ҫ����Ⱦ����֮ǰ���
	//-----------------------------------------------------------------------------------
	void LightingManager::Update()
	{
		//LightableObjectList	staticLightableObjs;
		//LightableObjectList	dynamicLightableObjs;

		//VecLights			staticLights;
		//VecLights			dynamicLights;

		//// �жϳ�����ÿ���ƹ��Ƿ��ƶ���
		//for (LightableObjectList::iterator iter=m_LightableObjects.begin();
		//	iter!=m_LightableObjects.end();
		//	iter++)
		//{
		//	if ((*iter)->HasMoved())
		//	{
		//		dynamicLightableObjs.push_back(*iter);
		//	}
		//	else
		//	{
		//		staticLightableObjs.push_back(*iter);
		//	}
		//}

		//// �ж�ÿ�������Ƿ��ƶ���
		//for (VecLights::iterator iter=m_Lights.begin();
		//	iter!=m_Lights.end();
		//	iter++)
		//{
		//	if ((*iter)->HasMoved())
		//	{
		//		dynamicLights.push_back(*iter);
		//	}
		//	else
		//	{
		//		staticLights.push_back(*iter);
		//	}
		//}

		// hack: ���ж�������й��ս��бȽ�

		// TODO: ֻ��Ҫ���ɼ������ܹ����
		for(LightableObjectList::iterator iter_obj=m_LightableObjects.begin();
			iter_obj!=m_LightableObjects.end();
			iter_obj++)
		{
			(*iter_obj)->ClearLights();

			// ʹ�����ܵ�ȫ�ֹ��յ�Ӱ��
			for (LightList::iterator iter_light=m_GlobalLights.begin();
				iter_light!=m_GlobalLights.end();
				iter_light++)
			{
				(*iter_obj)->AddLight(*iter_light);
			}

			// �ֲ�����Ӱ��
			for (LightList::iterator iter_light=m_Lights.begin();
				iter_light!=m_Lights.end();
				iter_light++)
			{
				Vector3f light_pos = (*iter_light)->WorldTransform().GetPosition();
				Vector3f obj_pos = (*iter_obj)->GetRenderableObject()->WorldTransform().GetPosition();

				float squaredRadius = (*iter_light)->GetRange() + (*iter_obj)->GetRenderableObject()->GetBoundingRadius();
				squaredRadius *= squaredRadius;
				if ((light_pos - obj_pos).SquaredLength() < squaredRadius)
				{
					(*iter_obj)->AddLight(*iter_light);
				}
			}
		}
	}
}
