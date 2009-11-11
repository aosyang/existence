//-----------------------------------------------------------------------------------
/// ALAudioSource.h OpenAL��Դ
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ALAUDIOSOURCE_H
#define _ALAUDIOSOURCE_H

#include "IAudioSystem.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace Gen
{
	// TODO: ĿǰALAudioSource�ǲ�����ֱ��delete�ģ�������ALAudioSystem����ж��

	//-----------------------------------------------------------------------------------
	/// \brief
	/// OpenAL��Դ
	/// 
	/// ��¼��һ�����Բ��ŵ���Ƶ������Ϣ��
	/// ����������3D�ռ��е�λ���Լ��ƶ��ٶȵ���Ϣ
	//-----------------------------------------------------------------------------------
	class ALAudioSource : public IAudioSource
	{
		friend class ALAudioSystem;
	public:
		ALAudioSource();
		~ALAudioSource();

		// ----- Overwrite IAudioSource

		void Play();
		void Pause();
		void Stop();
		void Rewind();

		void SetAudioBuffer(IAudioBuffer* buffer);

		void SetPitch(float pitch);
		float GetPitch();

		void SetGain(float gain);
		float GetGain();

		void SetLooping(bool loop);
		bool GetLooping();

		void SetPlaybackTime(float time);
		float GetPlaybackTime();

		void SetPosition(const Vector3f& position);
		void SetTransform(const Matrix4& transform);

		void SetVelocity(const Vector3f& vel);
		const Vector3f GetVelocity();

		bool GetAutoRemove() const { return m_RemoveOnDone; }

		bool IsStopped();
	protected:

	private:
		Vector3f		m_SourcePosition;			///< ��Դ����
		Vector3f		m_SourceVelocity;			///< ��Դ�ƶ��ٶ�

		ALuint			m_Buffer;					///< OpenAL Buffer���
		ALuint			m_Source;					///< OpenAL��Դ���

		bool			m_RemoveOnDone;
	};
}


#endif
