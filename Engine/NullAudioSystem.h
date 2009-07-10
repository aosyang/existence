//-----------------------------------------------------------------------------------
/// NullAudioSystem.h �հ���Ƶϵͳ���ṩû����Ƶ���ʱ�Ľ������
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
/// �հ���Ƶϵͳ
/// 
/// ������Ƶ�ӿ��ṩ�հ׵�ʵ�ַ���������û����Ƶϵͳʱ���������
/// 
/// \remarks
/// �հ���Ƶϵͳ�ķ�������ֵ���ᵼ������ϵͳ����
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
