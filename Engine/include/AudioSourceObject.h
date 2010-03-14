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
#include "AudioBuffer.h"
#include "EString.h"

namespace Gen
{
	class AudioSourceObject : public SceneObject
	{
		DECLARE_FACTORY_OBJECT(AudioSourceObject, SceneObject);
	public:
		// ----- Overwrite IObject

		void Update(unsigned long deltaTime);

		// ----- AudioSourceObject Methods

		// 指定音频缓冲
		void SetAudioBuffer(AudioBuffer* buffer);
		AudioBuffer* GetAudioBuffer() { return m_Buffer; }

		void Play();
		void Stop();
		void Pause();

		void SetLooping(bool loop);
		bool GetLooping();

		inline void SetVelocityFactor(float factor) { m_VelocityFactor = factor; }
		inline float GetVelocityFactor() const { return m_VelocityFactor; }

	protected:
		IAudioSource*		m_Source;
		AudioBuffer*		m_Buffer;

		Vector3f			m_OldPosition;			///< 计算移动速度用的上一帧位置
		float				m_VelocityFactor;		///< 速度因子，影响多普勒效应，设置成0.0f为关闭
	};
}

#endif
