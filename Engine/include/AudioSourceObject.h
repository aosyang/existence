//-----------------------------------------------------------------------------------
/// AudioSourceObject.h 音源对象，可在场景中移动的音源
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _AUDIOSOURCEOBJECT_H
#define _AUDIOSOURCEOBJECT_H

#include "SceneObject.h"
#include "IAudioSource.h"
#include "EString.h"

namespace Gen
{
	class AudioSourceObject : public SceneObject
	{
		DECLARE_FACTORY_OBJECT(AudioSourceObject);
	public:
		// ----- Overwrite IObject

		void Update(unsigned long deltaTime);

		// ----- AudioSourceObject Methods

		bool CreateAudioSource(IAudioBuffer* buffer);
		bool CreateAudioSource(const String& bufferName);

		// 指定音源(注：不要让多个对象共享同一个音源)
		//void SetAudioSource(IAudioSource* source);
		IAudioSource* GetAudioSource() { return m_Source; }

		void Play();
		void Stop();
		void Pause();

		void SetLooping(bool loop);
		bool GetLooping();

		inline void SetVelocityFactor(float factor) { m_VelocityFactor = factor; }
		inline float GetVelocityFactor() const { return m_VelocityFactor; }

	protected:
		IAudioSource*		m_Source;

		Vector3f			m_OldPosition;			///< 计算移动速度用的上一帧位置
		float				m_VelocityFactor;		///< 速度因子，影响多普勒效应，设置成0.0f为关闭
	};
}

#endif
