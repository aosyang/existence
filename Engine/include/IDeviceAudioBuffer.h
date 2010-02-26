//-----------------------------------------------------------------------------------
/// IDeviceAudioBuffer.h 音频缓冲接口，存放已经读取的音频数据，多个音源可以共享同一个音频缓冲
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IDEVICEAUDIOBUFFER_H
#define _IDEVICEAUDIOBUFFER_H

namespace Gen
{
	class IDeviceAudioBuffer
	{
	public:
		virtual ~IDeviceAudioBuffer() {}
	};
}

#endif
