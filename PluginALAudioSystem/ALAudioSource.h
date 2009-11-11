//-----------------------------------------------------------------------------------
/// ALAudioSource.h OpenAL音源
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
	// TODO: 目前ALAudioSource是不允许被直接delete的，必须由ALAudioSystem负责卸载

	//-----------------------------------------------------------------------------------
	/// \brief
	/// OpenAL音源
	/// 
	/// 记录了一个可以播放的音频个体信息，
	/// 可以设置在3D空间中的位置以及移动速度等信息
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
		Vector3f		m_SourcePosition;			///< 音源坐标
		Vector3f		m_SourceVelocity;			///< 音源移动速度

		ALuint			m_Buffer;					///< OpenAL Buffer序号
		ALuint			m_Source;					///< OpenAL音源序号

		bool			m_RemoveOnDone;
	};
}


#endif
