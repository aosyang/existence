//-----------------------------------------------------------------------------------
/// ALAudioBuffer.h OpenAL��Ƶ����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _ALAUDIOBUFFER_H
#define _ALAUDIOBUFFER_H

#include "IAudioBuffer.h"
#include "ALAudioSource.h"

#include <al/al.h>
#include <al/alc.h>

class ALAudioBuffer : public IAudioBuffer
{
	friend class ALAudioSource;
public:
	ALAudioBuffer();
	~ALAudioBuffer();

	void CreateFromData(int format, void* data, int size, int freq);

private:
	ALuint		m_Buffer;

	ALenum		m_Format;
	ALsizei		m_Frequency;
};

#endif
