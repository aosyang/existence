//-----------------------------------------------------------------------------------
/// ALAudioSystem.cpp OpenAL音频系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "ALAudioSystem.h"

ALAudioSystem::ALAudioSystem()
{
	m_FileFormatCreator["wav"] = &CreateWAVFileFormat;
	m_FileFormatCreator["ogg"] = &CreateOGGFileFormat;
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
	// 释放所有音频

	AudioBufferList::iterator iter = m_Buffers.begin();

	while (iter != m_Buffers.end())
	{
		delete iter->second;
		m_Buffers.erase(iter);

		iter = m_Buffers.begin();
	}

	AudioSourceList::iterator source_iter;
	for (source_iter=m_Sources.begin(); source_iter!=m_Sources.end(); source_iter++)
	{
		delete (*source_iter);
	}

	m_Sources.clear();
}

bool ALAudioSystem::LoadAudioBufferFromFile(const String& bufferName, const String& filename)
{
	// 已经存在则不允许再次载入
	if (m_Buffers.find(filename.Data()) != m_Buffers.end())
		return false;

	size_t pos = filename.FindLastOf(".");
	String ext = filename.Substr(pos + 1);

	if (m_FileFormatCreator.find(ext)==m_FileFormatCreator.end())
		return false;

	IAudioFileFormat* audioFile = (*m_FileFormatCreator[ext])(filename);

	ALAudioBuffer* al_buffer = new ALAudioBuffer();

	al_buffer->CreateFromData(audioFile->GetFormat(), audioFile->GetData(), audioFile->GetSize(), audioFile->GetFrequency());
	delete audioFile;

	m_Buffers[bufferName] = al_buffer;

	return true;
}

IAudioBuffer* ALAudioSystem::GetAudioBuffer(const String& bufferName)
{
	if (m_Buffers.find(bufferName) == m_Buffers.end())
		return NULL;

	return m_Buffers[bufferName];
}

void ALAudioSystem::SetListenerTransform(const Matrix4& transform, const Vector3f& velocity)
{
	// 取得矩阵的前方向和上方向，设置听者的位置
	alListenerfv(AL_POSITION,    transform.GetPosition().GetArray());

	// TODO: Set listener's velocity here.
	alListenerfv(AL_VELOCITY, velocity.GetArray());

	// 从变换矩阵获得前方向与上方向，指定听者的方向
	float orientation[6];
	memcpy(&orientation[0], transform.GetForwardVector().GetArray(), sizeof(float) * 3);
	memcpy(&orientation[3], transform.GetUpVector().GetArray(), sizeof(float) * 3);
	alListenerfv(AL_ORIENTATION, orientation);
}

IAudioSource* ALAudioSystem::CreateSourceInstance(IAudioBuffer* buffer, const Vector3f& position, bool autoRemove)
{
	ALAudioSource* al_source = new ALAudioSource();
	al_source->SetAudioBuffer(buffer);
	al_source->SetPosition(position);
	al_source->SetPitch(1.0f);
	al_source->SetGain(1.0f);
	al_source->SetAutoRemove(autoRemove);

	m_Sources.push_back(al_source);

	return al_source;
}

void ALAudioSystem::Update()
{
	AudioSourceList::iterator iter;
	for (iter=m_Sources.begin(); iter!=m_Sources.end();)
	{
		if ((*iter)->m_RemoveOnDone && (*iter)->IsStopped())
		{
			AudioSourceList::iterator next = iter;
			next++;
			delete (*iter);
			m_Sources.erase(iter);
			iter = next;

			continue;
		}

		iter++;
	}
}

IAudioSystem* CreateAudioSystem()
{
	return new ALAudioSystem;
}

