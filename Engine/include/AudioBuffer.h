//-----------------------------------------------------------------------------------
/// AudioBuffer.h ��Ƶ�������
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

	// ��Ƶ������󣬴��ļ�������Ƶ���ݣ������Ϊ�豸����
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

		// �ж��Ƿ�Ϊ������
		bool IsStreamingBuffer() const;
	private:
		IDeviceAudioBuffer*		m_DeviceAudioBuffer;
	};
}

#endif
