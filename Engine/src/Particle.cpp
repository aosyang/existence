//-----------------------------------------------------------------------------------
/// Particle.cpp 单一粒子，描述了粒子的属性与行为
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Particle.h"

namespace Gen
{
	//bool DefaultParticleUpdateBehavior(Particle* particle, unsigned long deltaTime)
	//{
	//	if (particle->m_Duration == -1)
	//	{
	//		// 粒子寿命无限
	//	}
	//	else if (particle->m_Age < particle->m_Duration)
	//		particle->m_Age += deltaTime;
	//	else
	//		particle->m_Active = false;

	//	particle->m_Position += particle->m_Velocity * (float)deltaTime * 0.001f;
	//	particle->spin += particle->spinInc * (float)deltaTime * 0.001f;
	//	particle->m_Scale += particle->m_ScaleInc * (float)deltaTime * 0.001f;

	//	return particle->m_Active;
	//}

	Particle::Particle()
		: m_Active(false),
		  m_Position(0.0f, 0.0f, 0.0f),
		  m_Velocity(0.0f, 0.0f, 0.0f),
		  m_Age(-1),
		  m_Duration(-1),
		  spin(0.0f),
		  spinInc(0.0f),
		  scale(1.0f),
		  scaleInc(0.0f),
		  alphaFadeIn(0.1f),
		  alphaFadeOut(0.8f),
		  m_ScreenScaleX(1.0f),
		  m_ScreenScaleY(1.0f)/*,
		  m_UpdateFunc(&DefaultParticleUpdateBehavior)*/
	{
	}

	bool Particle::AdvanceTime(unsigned long deltaTime)
	{
		if (!m_Active) return false;

		if (m_Duration == -1)
		{
			// 粒子寿命无限
		}
		else if (m_Age < m_Duration)
			m_Age += deltaTime;
		else
			m_Active = false;

		m_Position += m_Velocity * (float)deltaTime * 0.001f;
		spin += spinInc * (float)deltaTime * 0.001f;
		scale += scaleInc * (float)deltaTime * 0.001f;

		// alpha

		float fadeInTime;
		float fadeOutTime;
		if (m_Age < (fadeInTime = alphaFadeIn * m_Duration))
		{
			m_Color.a = (float)m_Age / fadeInTime;
		}
		else if (m_Age > (fadeOutTime = alphaFadeOut * m_Duration))
		{
			m_Color.a = 1.0f - (float)(m_Age-fadeOutTime) / (float)(m_Duration-fadeOutTime);
		}
		else
			m_Color.a = 1.0f;


		return m_Active;

		//return (*m_UpdateFunc)(this, deltaTime);
	}

}
