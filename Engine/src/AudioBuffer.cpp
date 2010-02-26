//-----------------------------------------------------------------------------------
/// AudioBuffer.cpp ��Ƶ�������ʵ�ִ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "AudioBuffer.h"
#include "Engine.h"

namespace Gen
{
	AudioBuffer::AudioBuffer(const String& filename)
		: Resource(filename),
		  m_DeviceAudioBuffer(NULL)
	{
	}

	bool AudioBuffer::LoadImpl()
	{
		// ���ļ�������Ƶ
		m_DeviceAudioBuffer = Engine::Instance().AudioSystem()->LoadAudioBufferFromFile(m_Filename);
		if (!m_DeviceAudioBuffer) return false;

		return true;
	}

	void AudioBuffer::UnloadImpl()
	{
		SAFE_DELETE(m_DeviceAudioBuffer);
	}
}

