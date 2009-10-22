//-----------------------------------------------------------------------------------
/// AudioListener.cpp ��Ƶ������
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
	SceneObject::Update(deltaTime);

	// �����������ٶ���Ϣ
	Vector3f vel = (m_WorldTransform.GetPosition() - m_OldPosition) * 1000.0f / (float)deltaTime * m_VelocityFactor;
	m_OldPosition = m_WorldTransform.GetPosition();
	Engine::Instance().AudioSystem()->SetListenerTransform(this->WorldTransform(), vel);
}