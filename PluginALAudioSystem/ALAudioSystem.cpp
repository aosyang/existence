//-----------------------------------------------------------------------------------
/// ALAudioSystem.cpp OpenAL音频系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "ALAudioSystem.h"

namespace Gen
{
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
		alutExit();
	}

	IDeviceAudioBuffer* ALAudioSystem::LoadAudioBufferFromFile(const String& filename)
	{
		size_t pos = filename.FindLastOf(".");
		String ext = filename.Substr(pos + 1);

		if (m_FileFormatCreator.find(ext)==m_FileFormatCreator.end())
			return NULL;

		IAudioFileFormat* audioFile = (*m_FileFormatCreator[ext])(filename);

		ALAudioBuffer* al_buffer = new ALAudioBuffer();

		// 从音频数据创建音频缓冲
		al_buffer->CreateFromData(audioFile->GetFormat(), audioFile->GetData(), audioFile->GetSize(), audioFile->GetFrequency());
		delete audioFile;

		return al_buffer;
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

	IAudioSource* ALAudioSystem::CreateAudioSource()
	{
		return new ALAudioSource;
	}

	//IAudioSource* ALAudioSystem::CreateSourceInstance(IAudioBuffer* buffer, const Vector3f& position, bool autoRemove)
	//{
	//	ALAudioSource* al_source = new ALAudioSource();
	//	al_source->SetAudioBuffer(buffer);
	//	al_source->SetPosition(position);
	//	al_source->SetPitch(1.0f);
	//	al_source->SetGain(1.0f);
	//	al_source->m_RemoveOnDone = autoRemove;

	//	m_Sources.insert(al_source);

	//	return al_source;
	//}

	IAudioSystem* CreateAudioSystem()
	{
		return new ALAudioSystem;
	}
}

