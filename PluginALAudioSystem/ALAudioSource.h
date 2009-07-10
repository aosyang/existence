//-----------------------------------------------------------------------------------
/// ALAudioSource.h OpenAL��Դ
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ALAUDIOSOURCE_H
#define _ALAUDIOSOURCE_H

#include "IAudioSystem.h"

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

// TODO: ĿǰALAudioSource�ǲ�����ֱ��delete�ģ�������ALAudioSystem����ж��

//-----------------------------------------------------------------------------------
/// \brief
/// OpenAL��Դ
/// 
/// ��¼��һ�����Բ��ŵ���Ƶ������Ϣ��
/// ����������3D�ռ��е�λ���Լ��ƶ��ٶȵ���Ϣ
//-----------------------------------------------------------------------------------
class ALAudioSource : public IAudioSource
{
	friend class ALAudioSystem;
public:

	// ----- Overwrite IAudioSource

	void Play();
	void Stop();

	void SetTransform(const Matrix4& transform);
protected:
	ALAudioSource();

private:
	Vector3f		m_SourcePosition;			///< ��Դ����
	Vector3f		m_SourceVelocity;			///< ��Դ�ƶ��ٶ�

	ALuint			m_Buffer;					///< OpenAL Buffer���
	ALuint			m_Source;					///< OpenAL��Դ���

	bool			m_Loop;						///< �Ƿ�ѭ����Ч

};


#endif