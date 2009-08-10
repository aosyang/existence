//-----------------------------------------------------------------------------------
/// ALAudioSource.cpp OpenAL音源
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "ALAudioSource.h"
#include "ALAudioBuffer.h"

ALAudioSource::ALAudioSource()
: m_SourcePosition(0.0f, 0.0f, 0.0f),
  m_SourceVelocity(0.0f, 0.0f, 0.0f),
  m_RemoveOnDone(false)
{
	// 生成OpenAL音源
	alGenSources(1, &m_Source);
}

ALAudioSource::~ALAudioSource()
{
	// 销毁音源
	alDeleteSources(1, &m_Source);
}

// 播放音源
void ALAudioSource::Play()
{
	alSourcePlay(m_Source);
}

// 暂停播放
void ALAudioSource::Pause()
{
	alSourcePause(m_Source);
}

// 停止播放
void ALAudioSource::Stop()
{
	alSourceStop(m_Source);
}

// 回到初始点
void ALAudioSource::Rewind()
{
	alSourceRewind(m_Source);
}

// 指定音频缓冲
void ALAudioSource::SetAudioBuffer(IAudioBuffer* buffer)
{
	// 将buffer id赋予当前source
	ALAudioBuffer* al_buf = static_cast<ALAudioBuffer*>(buffer);

	alSourcei(m_Source, AL_BUFFER, al_buf->m_Buffer);
}

// 设置音高
void ALAudioSource::SetPitch(float pitch)
{
	alSourcef(m_Source, AL_PITCH, pitch);
}

// 获取音高
float ALAudioSource::GetPitch()
{
	float pitch;
	alGetSourcef(m_Source, AL_PITCH, &pitch);

	return pitch;
}

// 设置音量
void ALAudioSource::SetGain(float gain)
{
	alSourcef(m_Source, AL_GAIN, gain);
}

// 获取音量
float ALAudioSource::GetGain()
{
	float gain;
	alGetSourcef(m_Source, AL_GAIN, &gain);

	return gain;
}

// 设为循环播放
void ALAudioSource::SetLooping(bool loop)
{
	alSourcei(m_Source, AL_LOOPING, loop);
}

// 获取循环播放状态
bool ALAudioSource::GetLooping()
{
	ALint loop;
	alGetSourcei(m_Source, AL_LOOPING, &loop);

	return (bool)loop;
}

void ALAudioSource::SetPlaybackTime(float time)
{
	alSourcef(m_Source, AL_SEC_OFFSET, time);
}

// 获取当前播放时间
float ALAudioSource::GetPlaybackTime()
{
	float time;
	alGetSourcef(m_Source, AL_SEC_OFFSET, &time);

	return time;
}

void ALAudioSource::SetPosition(const Vector3f& position)
{
	alSourcefv(m_Source, AL_POSITION, position.GetArray());
}

void ALAudioSource::SetTransform(const Matrix4& transform)
{
	alSourcefv(m_Source, AL_POSITION, transform.GetPosition().GetArray());
}

void ALAudioSource::SetVelocity(const Vector3f& vel)
{
	alSourcefv(m_Source, AL_VELOCITY, vel.GetArray());
}

const Vector3f ALAudioSource::GetVelocity()
{
	Vector3f vel;
	alGetSourcefv(m_Source, AL_VELOCITY, &vel.x);

	return vel;
}

bool ALAudioSource::IsStopped()
{
	ALint state;
	alGetSourcei(m_Source, AL_SOURCE_STATE, &state);

	return (state==AL_STOPPED);
}
