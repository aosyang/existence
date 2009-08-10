//-----------------------------------------------------------------------------------
/// ALAudioSource.cpp OpenAL��Դ
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
	// ����OpenAL��Դ
	alGenSources(1, &m_Source);
}

ALAudioSource::~ALAudioSource()
{
	// ������Դ
	alDeleteSources(1, &m_Source);
}

// ������Դ
void ALAudioSource::Play()
{
	alSourcePlay(m_Source);
}

// ��ͣ����
void ALAudioSource::Pause()
{
	alSourcePause(m_Source);
}

// ֹͣ����
void ALAudioSource::Stop()
{
	alSourceStop(m_Source);
}

// �ص���ʼ��
void ALAudioSource::Rewind()
{
	alSourceRewind(m_Source);
}

// ָ����Ƶ����
void ALAudioSource::SetAudioBuffer(IAudioBuffer* buffer)
{
	// ��buffer id���赱ǰsource
	ALAudioBuffer* al_buf = static_cast<ALAudioBuffer*>(buffer);

	alSourcei(m_Source, AL_BUFFER, al_buf->m_Buffer);
}

// ��������
void ALAudioSource::SetPitch(float pitch)
{
	alSourcef(m_Source, AL_PITCH, pitch);
}

// ��ȡ����
float ALAudioSource::GetPitch()
{
	float pitch;
	alGetSourcef(m_Source, AL_PITCH, &pitch);

	return pitch;
}

// ��������
void ALAudioSource::SetGain(float gain)
{
	alSourcef(m_Source, AL_GAIN, gain);
}

// ��ȡ����
float ALAudioSource::GetGain()
{
	float gain;
	alGetSourcef(m_Source, AL_GAIN, &gain);

	return gain;
}

// ��Ϊѭ������
void ALAudioSource::SetLooping(bool loop)
{
	alSourcei(m_Source, AL_LOOPING, loop);
}

// ��ȡѭ������״̬
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

// ��ȡ��ǰ����ʱ��
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
