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

#include <string>

using namespace std;

//-----------------------------------------------------------------------------------
/// \brief
/// ��Դ�ӿ�
/// 
/// �����Ѷ�ȡ����Դ
/// 
/// \remarks
/// ��Դ��Ҫʹ��AudioSystem����ж�أ�����ֱ��ɾ��
//-----------------------------------------------------------------------------------
class IAudioSource
{
public:
	virtual void Play() = 0;
	virtual void Stop() = 0;

	virtual void SetTransform(const Matrix4& transform) = 0;
protected:
	IAudioSource() {}
	virtual ~IAudioSource() {}
};

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
	virtual bool LoadAudioFromFile(const string& sourceName, const string& filename) = 0;

	// ������Ƶ������һ����Դ
	virtual IAudioSource* GetAudioSource(const string& sourceName) = 0;

	// ָ�����ߵ�λ��
	virtual void SetListenerTransform(const Matrix4& transform) = 0;

	// ����һ��ָ������Դ
	virtual void PlaySource(IAudioSource* source) = 0;
	//-----------------------------------------------------------------------------------
	/// \brief
	/// ����һ��ָ�����Ƶ���Դ
	/// 
	/// \param sourceName
	/// ��Դ����
	/// 
	/// \remarks
	/// ��������൱�ڷֱ����GetAudioSource(const string& sourceName)��PlaySource(IAudioSource* source)
	//-----------------------------------------------------------------------------------
	virtual void PlaySource(const string& sourceName) = 0;
	// ָֹͣ����Դ�Ĳ���
	virtual void StopSource(IAudioSource* source) = 0;

};

// ��Ƶϵͳ�������������ڲ�������ɺ���
typedef IAudioSystem*(*AudioSystemFactoryCreateFunc)();

#endif
