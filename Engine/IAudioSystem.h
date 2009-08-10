//-----------------------------------------------------------------------------------
/// IAudioSystem.h ��Ƶϵͳ�ӿڣ��ṩ��Ƶϵͳ�Ĺ�����������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _IAUDIOSYSTEM_H
#define _IAUDIOSYSTEM_H

#include "Matrix4.h"
#include "EString.h"
#include "IAudioSource.h"
#include "IAudioBuffer.h"

//-----------------------------------------------------------------------------------
/// \brief
/// ��Ƶϵͳ�ӿ�
/// 
/// �ṩ��Ƶϵͳ�Ļ��������ӿ�
//-----------------------------------------------------------------------------------
class IAudioSystem
{
public:
	virtual ~IAudioSystem() {}

	// ��ʼ����ر�
	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;

	// ���ļ�����һ����Ƶ
	virtual bool LoadAudioBufferFromFile(const String& bufferName, const String& filename) = 0;

	// ������Ƶ������һ����Դ
	virtual IAudioBuffer* GetAudioBuffer(const String& bufferName) = 0;

	// ָ�����ߵ�λ��
	virtual void SetListenerTransform(const Matrix4& transform, const Vector3f& velocity) = 0;

	virtual IAudioSource* CreateSourceInstance(IAudioBuffer* buffer, const Vector3f& position, bool autoRemove=true) = 0;

	virtual void RemoveSource(IAudioSource* source) = 0;

	virtual void Update() = 0;
};

// ��Ƶϵͳ�������������ڲ�������ɺ���
typedef IAudioSystem*(*AudioSystemFactoryCreateFunc)();

#endif
