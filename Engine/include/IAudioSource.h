//-----------------------------------------------------------------------------------
/// IAudioSource.h ��Դ�ӿڣ�һ����Դ��һ�����ж������ԵĿɲ��Ÿ���
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
		virtual ~IAudioSource() {}

		// ������Դ
		virtual void Play() = 0;
		// ��ͣ��Դ����
		virtual void Pause() = 0;
		// ֹͣ��Դ���ţ�����������Զ�ɾ������Դ��������
		virtual void Stop() = 0;
		// �ص�������ʼ��
		virtual void Rewind() = 0;

		// ָ����Դʹ�õ���Ƶ����
		virtual void SetAudioBuffer(IAudioBuffer* buffer) = 0;

		// ����
		virtual void SetPitch(float pitch) = 0;
		virtual float GetPitch() = 0;

		// ����
		virtual void SetGain(float gain) = 0;
		virtual float GetGain() = 0;

		// ѭ������
		virtual void SetLooping(bool loop) = 0;
		virtual bool GetLooping() = 0;

		// ����ʱ��(��λ����)
		virtual void SetPlaybackTime(float time) = 0;
		virtual float GetPlaybackTime() = 0;

		// �ռ�����
		virtual void SetPosition(const Vector3f& position) = 0;
		virtual void SetTransform(const Matrix4& transform) = 0;

		// ��Դ�ƶ��ٶ�
		virtual void SetVelocity(const Vector3f& vel) = 0;
		virtual const Vector3f GetVelocity() = 0;

		// �Զ�����(ֻ������)
		virtual bool GetAutoRemove() const = 0;

		// �Ƿ�ֹͣ����
		virtual bool IsStopped() = 0;
	protected:
		//IAudioSource() {}
	};
}

#endif
