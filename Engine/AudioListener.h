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

	bool IntersectsRay(const Ray& ray, CollisionInfo& info);

	// ----- AudioListener Methods

	inline void SetVelocityFactor(float factor) { m_VelocityFactor = factor; }
	inline float GetVelocityFactor() const { return m_VelocityFactor; }

protected:
	Vector3f		m_OldPosition;
	float			m_VelocityFactor;		///< 速度因子，影响多普勒效应，设置成0.0f为关闭
};

#endif
