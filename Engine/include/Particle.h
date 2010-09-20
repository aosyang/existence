//-----------------------------------------------------------------------------------
/// Particle.h 单一粒子，描述了粒子的属性
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "Vector3f.h"
#include "Color4f.h"
#include <memory.h>

namespace Gen
{
	class ParticlePool;
	struct Particle;

	//typedef bool(*ParticleUpdateFunc)(Particle*, unsigned long);

	//bool DefaultParticleUpdateBehavior(Particle* particle, unsigned long deltaTime);

	struct Particle
	{
		bool		m_Active;
		Vector3f	m_Position;
		Vector3f	m_Velocity;		///< 粒子速度(世界空间)

		Color4f		m_Color;

		long		m_Age;
		long		m_Duration;		///< 粒子生命周期，-1为永不消亡

		float		spin;
		float		spinInc;

		float		scale;
		float		scaleInc;

		float		alphaFadeIn;
		float		alphaFadeOut;

		float		m_ScreenScaleX;
		float		m_ScreenScaleY;

		//ParticleUpdateFunc	m_UpdateFunc;

		//int			m_VertexOffset;

		//unsigned int m_IndexOffset;

		Particle();

		Particle operator=(const Particle& rhs)
		{
			if (&rhs != this)
				memcpy(this, &rhs, sizeof(Particle));

			return *this;
		}

		// 更新粒子，如果粒子寿命结束返回false，其他情况返回true
		bool AdvanceTime(unsigned long deltaTime);

	};
}

#endif
