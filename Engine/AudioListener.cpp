//-----------------------------------------------------------------------------------
/// AudioListener.cpp ÒôÆµÊÕÌýÕß
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "AudioListener.h"
#include "Engine.h"

AudioListener::AudioListener() {}
AudioListener::~AudioListener() {}
void AudioListener::Update(unsigned long deltaTime)
{
	BaseSceneObject::Update(deltaTime);

	Engine::Instance().AudioSystem()->SetListenerTransform(this->WorldTransform());
}

void AudioListener::DebugRender() {}

bool AudioListener::IntersectsRay(const Ray& ray, CollisionInfo& info, int type)
{
	return false;
}
