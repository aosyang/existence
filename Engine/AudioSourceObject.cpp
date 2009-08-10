//-----------------------------------------------------------------------------------
/// AudioSourceObject.h 音源对象，可在场景中移动的音源
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "AudioSourceObject.h"
#include "Engine.h"

AudioSourceObject::AudioSourceObject()
: m_Source(NULL),
  m_OldPosition(0.0f, 0.0f, 0.0f),
  m_VelocityFactor(0.0f)
{
}

AudioSourceObject::~AudioSourceObject()
{
	Engine::Instance().AudioSystem()->RemoveSource(m_Source);
}

void AudioSourceObject::Update(unsigned long deltaTime)
{
	BaseSceneObject::Update(deltaTime);

	// 更新音源者速度信息
	Vector3f vel = (m_WorldTransform.GetPosition() - m_OldPosition) * 1000.0f / (float)deltaTime * m_VelocityFactor;
	m_OldPosition = m_WorldTransform.GetPosition();
	
	m_Source->SetPosition(this->WorldTransform().GetPosition());
	m_Source->SetVelocity(vel);
}

void AudioSourceObject::DebugRender() {}

bool AudioSourceObject::IntersectsRay(const Ray& ray, CollisionInfo& info)
{
	return false;
}
//
//void AudioSourceObject::SetAudioSource(IAudioSource* source)
//{
//	// 自动删除的对象不能使用，否则会出现问题
//	AssertFatal(!source->GetAutoRemove(), "AudioSourceObject::SetAudioSource(): Unable to use a source which will auto remove.");
//	m_Source = source;
//}

bool AudioSourceObject::CreateAudioSource(IAudioBuffer* buffer)
{
	SAFE_DELETE(m_Source);

	m_Source = Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f), false);
	//m_Source->Play();
	return (m_Source != NULL);
}

bool AudioSourceObject::CreateAudioSource(const String& bufferName)
{
	IAudioBuffer* buffer;
	buffer = Engine::Instance().AudioSystem()->GetAudioBuffer(bufferName);

	if (!buffer) return false;

	return CreateAudioSource(buffer);
}

void AudioSourceObject::Play()
{
	m_Source->Play();
}

void AudioSourceObject::Stop()
{
	m_Source->Stop();
}

void AudioSourceObject::Pause()
{
	m_Source->Pause();
}

void AudioSourceObject::SetLooping(bool loop)
{
	m_Source->SetLooping(loop);
}

bool AudioSourceObject::GetLooping()
{
	return m_Source->GetLooping();
}
