//-----------------------------------------------------------------------------------
/// FileFormatFuncs.h 音频格式读取函数
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _FILEFORMATFUNCS_H
#define _FILEFORMATFUNCS_H


#include "EString.h"
#include <al/al.h>
#include <al/alut.h>
#include <vorbis/vorbisfile.h>

#include <vector>

using namespace std;

class IAudioFileFormat
{
public:
	virtual ~IAudioFileFormat() {}

	virtual ALenum GetFormat() = 0;
	virtual void* GetData() = 0;
	virtual ALsizei GetSize() = 0;
	virtual ALsizei GetFrequency() = 0;
};

class BaseAudioFormat : public IAudioFileFormat
{
public:
	ALenum GetFormat() { return m_Format; }
	void* GetData() { return m_Data; } 
	ALsizei GetSize() { return m_Size; }
	ALsizei GetFrequency() { return m_Frequency; }
protected:
	ALenum	m_Format;
	void*	m_Data;
	ALsizei	m_Size;
	ALsizei	m_Frequency;
};

// wav格式文件读取
class WAVFileFormat : public BaseAudioFormat
{
public:
	WAVFileFormat(const String& filename)
	{
		ALboolean loop;

		// 使用alut读取wav文件
		alutLoadWAVFile((ALbyte*)filename.Data(), &m_Format, &m_Data, &m_Size, &m_Frequency, &loop);
	}

	~WAVFileFormat()
	{
		alutUnloadWAV(m_Format, m_Data, m_Size, m_Frequency);
	}
};

IAudioFileFormat* CreateWAVFileFormat(const String& filename)
{
	return new WAVFileFormat(filename);
}

#define BUFFER_SIZE 32768

class OGGFileFormat : public BaseAudioFormat
{
public:
	OGGFileFormat(const String& filename)
	{
		// OGG文件读取
		// 参考这篇文章：http://www.gamedev.net/reference/articles/article2031.asp

		// 0为little-endian
		// 1为big-endian
		int endian = 0;
		int bitStream;
		long bytes;
		char array[BUFFER_SIZE];
		FILE* fp;

		// 打开二进制文件
		fp = fopen(filename.Data(), "rb");

		vorbis_info *pInfo;

		ov_open(fp, &m_OGGFile, NULL, 0);

		// 获取OGG文件信息
		pInfo = ov_info(&m_OGGFile, -1);

		// 检查音频通道数目，假设始终是16位采样
		if (pInfo->channels == 1)
			m_Format = AL_FORMAT_MONO16;
		else
			m_Format = AL_FORMAT_STEREO16;

		m_Frequency = pInfo->rate;

		do
		{
			bytes = ov_read(&m_OGGFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

			m_BufferData.insert(m_BufferData.end(), array, array + bytes);
		} while (bytes>0);

		m_Data = &m_BufferData[0];
		m_Size = static_cast<ALsizei>(m_BufferData.size());
	}

	~OGGFileFormat()
	{
		ov_clear(&m_OGGFile);
	}
private:
	OggVorbis_File	m_OGGFile;

	// 音频缓冲数据
	vector<char>	m_BufferData;
};

IAudioFileFormat* CreateOGGFileFormat(const String& filename)
{
	return new OGGFileFormat(filename);
}

#endif
