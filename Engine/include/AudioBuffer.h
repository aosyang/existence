//-----------------------------------------------------------------------------------
/// AudioBuffer.h 音频缓冲对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _AUDIOBUFFER_H
#define _AUDIOBUFFER_H

#include "Resource.h"

namespace Gen
{
	class AudioManager;
	class IDeviceAudioBuffer;

	// 音频缓冲对象，从文件载入音频数据，保存成为设备缓冲
	class AudioBuffer : public Resource
	{
		friend class AudioManager;
	public:
		AudioBuffer(const String& filename);

		// ----- Overwrite Resource
		bool LoadImpl();
		void UnloadImpl();

		// ----- AudioBuffer Methods
		IDeviceAudioBuffer* GetDeviceAudioBuffer() { return m_DeviceAudioBuffer; }

		// 判断是否为流缓冲
		bool IsStreamingBuffer() const;
	private:
		IDeviceAudioBuffer*		m_DeviceAudioBuffer;
	};
}

#endif
