//-----------------------------------------------------------------------------------
/// AudioSourceObject.h 音源对象，可在场景中移动的音源
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "AudioSourceObject.h"
#include "Engine.h"

namespace Gen
{
	AudioSourceObject::AudioSourceObject(SceneGraph* scene)
	: SceneObject(scene),
	  m_Buffer(NULL),
	  m_OldPosition(0.0f, 0.0f, 0.0f),
	  m_VelocityFactor(0.0f)
	{
		m_Source = Engine::Instance().AudioSystem()->CreateAudioSource();
	}

	AudioSourceObject::~AudioSourceObject()
	{
		SAFE_DELETE(m_Source);
	}

	void AudioSourceObject::Update(unsigned long deltaTime)
	{
		SceneObject::Update(deltaTime);

		// 更新音源者速度信息
		Vector3f vel = (m_WorldTransform.GetPosition() - m_OldPosition) * 1000.0f / (float)deltaTime * m_VelocityFactor;
		m_OldPosition = m_WorldTransform.GetPosition();

		m_Source->SetPosition(this->WorldTransform().GetPosition());
		m_Source->SetVelocity(vel);
	}

	void AudioSourceObject::SetAudioBuffer(AudioBuffer* buffer)
	{
		m_Buffer = buffer;
		m_Source->SetAudioBuffer(buffer->GetDeviceAudioBuffer());

		if (buffer)
		{
			buffer->Trigger();
		}
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
}
