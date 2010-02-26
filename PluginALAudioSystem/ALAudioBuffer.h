//-----------------------------------------------------------------------------------
/// ALAudioBuffer.h OpenAL“Ù∆µª∫≥Â
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _ALAUDIOBUFFER_H
#define _ALAUDIOBUFFER_H

#include "IDeviceAudioBuffer.h"
#include "ALAudioSource.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace Gen
{
	class ALAudioBuffer : public IDeviceAudioBuffer
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
}

#endif
