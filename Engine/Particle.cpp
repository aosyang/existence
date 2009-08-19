//-----------------------------------------------------------------------------------
/// Particle.cpp 单一粒子，描述了粒子的属性与行为
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Particle.h"

bool DefaultParticleUpdateBehavior(Particle* particle, unsigned long deltaTime)
{
	if (particle->m_Duration == -1)
	{
		// 粒子寿命无限
	}
	else if (particle->m_Age < particle->m_Duration)
		particle->m_Age += deltaTime;
	else
		particle->m_Active = false;

	particle->m_Position += particle->m_Velocity * (float)deltaTime * 0.001f;
	particle->m_ZRotation += particle->m_ZRotationInc * (float)deltaTime * 0.001f;
	particle->m_Scale += particle->m_ScaleInc * (float)deltaTime * 0.001f;

	return particle->m_Active;
}
