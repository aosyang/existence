//-----------------------------------------------------------------------------------
/// ALAudioSystem.h OpenAL音频系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ALAUDIOSYSTEM_H
#define _ALAUDIOSYSTEM_H

#include "IAudioSystem.h"
#include "ALAudioSource.h"
#include "ALAudioBuffer.h"
#include "FileFormatFuncs.h"

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

#include <map>
#include <list>
#include "EString.h"

using namespace std;

// TODO: 
//   1、以Buffer作为资源，可以创建多个不同的Source
//   2、Source管理机制，在通道耗尽时能够根据音量大小及优先级等信息决定删除哪些音源

class ALAudioSystem : public IAudioSystem
{
public:
	ALAudioSystem();
	~ALAudioSystem();

	bool Initialize();
	void Shutdown();

	bool LoadAudioBufferFromFile(const String& bufferName, const String& filename);

	IAudioBuffer* GetAudioBuffer(const String& bufferName);

	void SetListenerTransform(const Matrix4& transform, const Vector3f& velocity);

	IAudioSource* CreateSourceInstance(IAudioBuffer* buffer, const Vector3f& position, bool autoRemove);

	void Update();
private:
	typedef map<const String, ALAudioBuffer*>	AudioBufferList;
	AudioBufferList						m_Buffers;

	typedef list<ALAudioSource*>		AudioSourceList;
	AudioSourceList						m_Sources;

	typedef IAudioFileFormat* (*CreateFileFormatFunc)(const String& filename);
	map<const String, CreateFileFormatFunc>			m_FileFormatCreator;
};

extern "C" __declspec(dllexport) IAudioSystem* CreateAudioSystem();

#endif

