//-----------------------------------------------------------------------------------
/// ALAudioSystem.h OpenAL��Ƶϵͳ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ALAUDIOSYSTEM_H
#define _ALAUDIOSYSTEM_H

#include "Platform.h"
#include "IAudioSystem.h"
#include "ALAudioSource.h"
#include "ALAudioBuffer.h"
#include "FileFormatFuncs.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <map>
#include <set>
#include "EString.h"

using namespace std;

namespace Gen
{
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

		IDeviceAudioBuffer* LoadAudioBufferFromFile(const String& filename);

		void SetListenerTransform(const Matrix4& transform, const Vector3f& velocity);

		IAudioSource* CreateAudioSource();

		//IAudioSource* CreateSourceInstance(IAudioBuffer* buffer, const Vector3f& position, bool autoRemove);

	private:

		typedef IAudioFileFormat* (*CreateFileFormatFunc)(const String& filename);
		map<const String, CreateFileFormatFunc>			m_FileFormatCreator;
	};

	extern "C" DLLEXPORT IAudioSystem* CreateAudioSystem();
}

#endif

