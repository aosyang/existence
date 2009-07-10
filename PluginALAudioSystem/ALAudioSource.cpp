//-----------------------------------------------------------------------------------
/// ALAudioSource.cpp OpenAL“Ù‘¥
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "ALAudioSource.h"

ALAudioSource::ALAudioSource()
: m_SourcePosition(0.0f, 0.0f, 0.0f),
  m_SourceVelocity(0.0f, 0.0f, 0.0f),
  m_Loop(false)
{

}


void ALAudioSource::Play()
{
	alSourcePlay(m_Source);
}

void ALAudioSource::Stop()
{
	alSourceStop(m_Source);
}

void ALAudioSource::SetTransform(const Matrix4& transform)
{
	alSourcefv(m_Source, AL_POSITION, transform.GetPosition().GetArray());
}
