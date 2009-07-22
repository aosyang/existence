//-----------------------------------------------------------------------------------
/// AudioListener.cpp 音频收听者
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "AudioListener.h"
#include "Engine.h"

AudioListener::AudioListener()
: m_OldPosition(0.0f, 0.0f, 0.0f),
  m_VelocityFactor(0.0f)
{}
AudioListener::~AudioListener() {}
void AudioListener::Update(unsigned long deltaTime)
{
	BaseSceneObject::Update(deltaTime);

	// 更新收听者速度信息
	Vector3f vel = (m_WorldTransform.GetPosition() - m_OldPosition) * 1000 / deltaTime * m_VelocityFactor;
	m_OldPosition = m_WorldTransform.GetPosition();
	Engine::Instance().AudioSystem()->SetListenerTransform(this->WorldTransform(), vel);
}

void AudioListener::DebugRender() {}

bool AudioListener::IntersectsRay(const Ray& ray, CollisionInfo& info)
{
	return false;
}
