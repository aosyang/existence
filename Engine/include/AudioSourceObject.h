//-----------------------------------------------------------------------------------
/// AudioSourceObject.h ��Դ���󣬿��ڳ������ƶ�����Դ
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

		// ָ����Դ(ע����Ҫ�ö��������ͬһ����Դ)
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

		Vector3f			m_OldPosition;			///< �����ƶ��ٶ��õ���һ֡λ��
		float				m_VelocityFactor;		///< �ٶ����ӣ�Ӱ�������ЧӦ�����ó�0.0fΪ�ر�
	};
}

#endif
