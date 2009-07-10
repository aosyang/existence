//-----------------------------------------------------------------------------------
/// ALAudioSource.h OpenAL音源
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

// TODO: 目前ALAudioSource是不允许被直接delete的，必须由ALAudioSystem负责卸载

//-----------------------------------------------------------------------------------
/// \brief
/// OpenAL音源
/// 
/// 记录了一个可以播放的音频个体信息，
/// 可以设置在3D空间中的位置以及移动速度等信息
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
	Vector3f		m_SourcePosition;			///< 音源坐标
	Vector3f		m_SourceVelocity;			///< 音源移动速度

	ALuint			m_Buffer;					///< OpenAL Buffer序号
	ALuint			m_Source;					///< OpenAL音源序号

	bool			m_Loop;						///< 是否循环音效

};


#endif