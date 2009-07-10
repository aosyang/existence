//-----------------------------------------------------------------------------------
/// NullAudioSystem.h 空白音频系统，提供没有音频插件时的解决方案
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _NULLAUDIOSYSTEM_H
#define _NULLAUDIOSYSTEM_H

#include "IAudioSystem.h"
#include "Platform.h"

//-----------------------------------------------------------------------------------
/// \brief
/// 空白音频系统
/// 
/// 根据音频接口提供空白的实现方法，用于没有音频系统时的替代方案
/// 
/// \remarks
/// 空白音频系统的方法返回值不会导致整个系统报错
//-----------------------------------------------------------------------------------
class NullAudioSystem : public IAudioSystem
{
public:
	bool Initialize()
	{
		//MessageBoxA(NULL, "Warning: Using NullAudioSystem. No sound will be heard.", "NullAudioSystem", MB_OK|MB_ICONINFORMATION);
		return true;
	}

	void Shutdown() {}

	bool LoadAudioFromFile(const string& sourceName, const string& filename) { return true; }

	IAudioSource* GetAudioSource(const string& sourceName) { return NULL; }

	void SetListenerTransform(const Matrix4& transform) {}

	void PlaySource(IAudioSource* source) {}

	void PlaySource(const string& sourceName) {}
	void StopSource(IAudioSource* source) {}
};

#endif
