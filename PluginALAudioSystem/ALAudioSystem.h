//-----------------------------------------------------------------------------------
/// ALAudioSystem.h OpenAL��Ƶϵͳ���
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
//   1����Buffer��Ϊ��Դ�����Դ��������ͬ��Source
//   2��Source������ƣ���ͨ���ľ�ʱ�ܹ�����������С�����ȼ�����Ϣ����ɾ����Щ��Դ

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

