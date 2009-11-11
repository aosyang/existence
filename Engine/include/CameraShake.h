//-----------------------------------------------------------------------------------
/// Camera.h ������࣬ʵ���˱����ƶ������ݱ任�����Ӿ���Ĺ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _CAMERASHAKE_H
#define _CAMERASHAKE_H

#include "Vector3f.h"
#include "Matrix4.h"
#include "Quaternion.h"

namespace Gen
{
	class CameraShakeEffect
	{
	public:
		CameraShakeEffect()
		{
			ClearValue();
		}

		void ClearValue()
		{
			m_Amplitude = 0.0f;
			m_Frequency = 0.0f;
			m_Duration = 0;
			m_ElpasedTime = 0;
			m_NextShakeTime = 0;
			m_OffsetMatrix.MakeIdentity();
		}

		void Create(float amplitude, float frequency, unsigned long duration)
		{
			m_Amplitude = amplitude;
			m_Frequency = frequency;
			m_Duration = duration;
			m_NextShakeTime = 0;
			m_ElpasedTime = 0;
		}

		void Update(unsigned long deltaTime)
		{
			m_ElpasedTime += deltaTime;

			if (m_ElpasedTime > m_Duration ||
				m_Duration <= 0.0f ||
				m_Amplitude <= 0.0f ||
				m_Frequency <= 0.0f)
			{
				ClearValue();
				return;
			}

			if (m_ElpasedTime > m_NextShakeTime)
			{
				m_NextShakeTime = m_ElpasedTime + (unsigned long)(1000 / m_Frequency);

				m_ShakeOffset = Vector3f(Math::Random(-m_Amplitude, m_Amplitude),
										 Math::Random(-m_Amplitude, m_Amplitude),
										 Math::Random(-m_Amplitude, m_Amplitude));

				m_Angle = Math::Random(-m_Amplitude * 0.25f, m_Amplitude * 0.25f);
			}
			
			// ����ʱ���������˥��
			float fraction = 1.0f - (float)m_ElpasedTime / m_Duration;
			float freq;

			if (fraction > 0)
			{
				freq = (float)m_Frequency / fraction;
			}
			else
			{
				freq = 0.0f;
			}

			// ���ٽӽ�0
			fraction *= fraction;

			fraction *= sin((float)m_ElpasedTime * 0.001f * freq);

			m_AppliedOffset = m_ShakeOffset * fraction;
			m_AppliedAngle = m_Angle * fraction;

			m_Amplitude -= m_Amplitude * deltaTime * 0.001f / (m_Frequency * m_Duration);

			Quaternion rot;
			rot.CreateFromLocalAxisAngle(m_AppliedOffset + Vector3f(0.0f, 0.0f, 1.0f), m_AppliedAngle);
			m_OffsetMatrix.SetRotation(rot);
			m_OffsetMatrix.SetPosition(m_AppliedOffset);
		}

		const Matrix4& GetOffsetMatrix() const { return m_OffsetMatrix; }

	protected:
		float			m_Amplitude;			///< ���
		float			m_Frequency;			///< Ƶ��
		unsigned long	m_Duration;				///< ����ʱ��
		unsigned long	m_ElpasedTime;			///< ����ʱ��
		unsigned long	m_NextShakeTime;		///< ��һ����ʱ��

		Vector3f		m_ShakeOffset;			///< һ�������ڵ����ƫ��
		Vector3f		m_AppliedOffset;		///< ʵ��ƫ��

		float			m_Angle;
		float			m_AppliedAngle;

		Matrix4			m_OffsetMatrix;
	};
}

#endif
