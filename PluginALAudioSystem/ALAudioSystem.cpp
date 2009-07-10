//-----------------------------------------------------------------------------------
/// ALAudioSystem.cpp OpenAL��Ƶϵͳ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "ALAudioSystem.h"

ALAudioSystem::ALAudioSystem()
{

}

ALAudioSystem::~ALAudioSystem()
{

}

bool ALAudioSystem::Initialize()
{
	alutInit(NULL, 0);
	if (alGetError() != AL_NO_ERROR)
		return false;

	return true;
}

void ALAudioSystem::Shutdown()
{
	// �ͷ�������Ƶ

	AudioSourceList::iterator iter = m_Sources.begin();

	while (iter != m_Sources.end())
	{
		//al_source
		
		// TODO: ͬһ��Buffer������Source�ᵼ�������ظ�ɾ��
		alDeleteBuffers(1, &iter->second->m_Buffer);
		alDeleteSources(1, &iter->second->m_Source);

		delete iter->second;
		m_Sources.erase(iter);

		iter = m_Sources.begin();
	}
}

bool ALAudioSystem::LoadAudioFromFile(const string& sourceName, const string& filename)
{
	// �Ѿ������������ٴ�����
	if (m_Sources.find(filename.data()) != m_Sources.end())
		return false;

	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;
	unsigned int buffer;
	unsigned int source;

	// ��wav������Ƶbuffer
	alGenBuffers(1, &buffer);

	if(alGetError() != AL_NO_ERROR)
		return false;

	alutLoadWAVFile((ALbyte*)filename.data(), &format, &data, &size, &freq, &loop);
	alBufferData(buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	// ��buffer����Դ��
	alGenSources(1, &source);

	if(alGetError() != AL_NO_ERROR)
		return false;

	// ������Դ����
	alSourcei (source, AL_BUFFER,   buffer   );
	alSourcef (source, AL_PITCH,    1.0      );
	alSourcef (source, AL_GAIN,     1.0      );
	//alSourcefv(source, AL_POSITION, SourcePos);
	//alSourcefv(source, AL_VELOCITY, SourceVel);
	alSourcei (source, AL_LOOPING,  loop     );

	// Do another error check and return.

	if(alGetError() != AL_NO_ERROR)
		return false;

	ALAudioSource* al_source = new ALAudioSource();
	al_source->m_Buffer = buffer;
	al_source->m_Source = source;
	al_source->m_Loop = (loop == AL_TRUE) ? true : false;

	m_Sources[sourceName.data()] = al_source;

	return true;
}

IAudioSource* ALAudioSystem::GetAudioSource(const string& sourceName)
{
	if (m_Sources.find(sourceName) == m_Sources.end())
		return NULL;

	return m_Sources[sourceName];
}

void ALAudioSystem::SetListenerTransform(const Matrix4& transform)
{
	// ȡ�þ����ǰ������Ϸ����������ߵ�λ��
	alListenerfv(AL_POSITION,    transform.GetPosition().GetArray());

	// TODO: Set listener's velocity here.
	//alListenerfv(AL_VELOCITY,    ListenerVel);

	// �ӱ任������ǰ�������Ϸ���ָ�����ߵķ���
	float orientation[6];
	memcpy(&orientation[0], transform.GetForwardVector().GetArray(), sizeof(float) * 3);
	memcpy(&orientation[3], transform.GetUpVector().GetArray(), sizeof(float) * 3);
	alListenerfv(AL_ORIENTATION, orientation);
}

void ALAudioSystem::PlaySource(IAudioSource* source)
{
	if (!source)
		return;

	source->Play();
};

void ALAudioSystem::PlaySource(const string& sourceName)
{
	IAudioSource* source = GetAudioSource(sourceName);
	if (source)
		PlaySource(source);
}

void ALAudioSystem::StopSource(IAudioSource* source)
{
	source->Stop();
}

IAudioSystem* CreateAudioSystem()
{
	return new ALAudioSystem;
}

