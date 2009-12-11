//-----------------------------------------------------------------------------------
/// AudioListener.h ��Ƶ������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _AUDIOLISTENER_H
#define _AUDIOLISTENER_H

#include "SceneObject.h"

namespace Gen
{
	//-----------------------------------------------------------------------------------
	/// \brief
	/// ��Ƶ������
	/// 
	/// �ṩ�����ߵ�λ����Ϣ
	//-----------------------------------------------------------------------------------
	class AudioListener : public SceneObject
	{
		DECLARE_FACTORY_OBJECT(AudioListener);
	public:

		// ----- Overwrite IObject

		void Update(unsigned long deltaTime);

		// ----- AudioListener Methods

		inline void SetVelocityFactor(float factor) { m_VelocityFactor = factor; }
		inline float GetVelocityFactor() const { return m_VelocityFactor; }

	protected:
		Vector3f		m_OldPosition;
		float			m_VelocityFactor;		///< �ٶ����ӣ�Ӱ�������ЧӦ�����ó�0.0fΪ�ر�
	};
}

#endif
