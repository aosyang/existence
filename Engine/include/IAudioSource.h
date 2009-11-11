//-----------------------------------------------------------------------------------
/// IAudioSource.h 音源接口，一个音源是一个具有独立属性的可播放个体
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IAUDIOSOURCE_H
#define _IAUDIOSOURCE_H

#include "IAudioBuffer.h"

namespace Gen
{
	//-----------------------------------------------------------------------------------
	/// \brief
	/// 音源接口
	/// 
	/// 保存已读取的音源
	/// 
	/// \remarks
	/// 音源需要使用AudioSystem进行卸载，不能直接删除
	//-----------------------------------------------------------------------------------
	class IAudioSource
	{
	public:
		virtual ~IAudioSource() {}

		// 播放音源
		virtual void Play() = 0;
		// 暂停音源播放
		virtual void Pause() = 0;
		// 停止音源播放，如果设置了自动删除，音源将被销毁
		virtual void Stop() = 0;
		// 回到播放起始点
		virtual void Rewind() = 0;

		// 指定音源使用的音频缓冲
		virtual void SetAudioBuffer(IAudioBuffer* buffer) = 0;

		// 音高
		virtual void SetPitch(float pitch) = 0;
		virtual float GetPitch() = 0;

		// 音量
		virtual void SetGain(float gain) = 0;
		virtual float GetGain() = 0;

		// 循环播放
		virtual void SetLooping(bool loop) = 0;
		virtual bool GetLooping() = 0;

		// 播放时间(单位：秒)
		virtual void SetPlaybackTime(float time) = 0;
		virtual float GetPlaybackTime() = 0;

		// 空间坐标
		virtual void SetPosition(const Vector3f& position) = 0;
		virtual void SetTransform(const Matrix4& transform) = 0;

		// 音源移动速度
		virtual void SetVelocity(const Vector3f& vel) = 0;
		virtual const Vector3f GetVelocity() = 0;

		// 自动销毁(只读属性)
		virtual bool GetAutoRemove() const = 0;

		// 是否停止播放
		virtual bool IsStopped() = 0;
	protected:
		//IAudioSource() {}
	};
}

#endif
