//-----------------------------------------------------------------------------------
/// ALAudioBuffer.cpp OpenAL“Ù∆µª∫≥Â
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "ALAudioBuffer.h"

ALAudioBuffer::ALAudioBuffer()
{
	alGenBuffers(1, &m_Buffer);
}

ALAudioBuffer::~ALAudioBuffer()
{
	alDeleteBuffers(1, &m_Buffer);
}

void ALAudioBuffer::CreateFromData(int format, void* data, int size, int freq)
{
	m_Format = format;
	m_Frequency = freq;

	alBufferData(m_Buffer, format, data, size, freq);
}

