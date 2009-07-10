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

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

#include <map>
#include <string>

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

	bool LoadAudioFromFile(const string& sourceName, const string& filename);

	IAudioSource* GetAudioSource(const string& sourceName);

	void SetListenerTransform(const Matrix4& transform);

	void PlaySource(IAudioSource* source);
	void PlaySource(const string& sourceName);
	void StopSource(IAudioSource* source);

private:
	typedef map<string, ALAudioSource*>	AudioSourceList;
	AudioSourceList						m_Sources;
};

extern "C" __declspec(dllexport) IAudioSystem* CreateAudioSystem();

#endif

