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

	bool IntersectsRay(const Ray& ray, CollisionInfo& info);

	// ----- AudioListener Methods

	inline void SetVelocityFactor(float factor) { m_VelocityFactor = factor; }
	inline float GetVelocityFactor() const { return m_VelocityFactor; }

protected:
	Vector3f		m_OldPosition;
	float			m_VelocityFactor;		///< �ٶ����ӣ�Ӱ�������ЧӦ�����ó�0.0fΪ�ر�
};

#endif
