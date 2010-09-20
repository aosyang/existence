//-----------------------------------------------------------------------------------
/// Particle.h ��һ���ӣ����������ӵ�����
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
		Vector3f	m_Velocity;		///< �����ٶ�(����ռ�)

		Color4f		m_Color;

		long		m_Age;
		long		m_Duration;		///< �����������ڣ�-1Ϊ��������

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

		// �������ӣ��������������������false�������������true
		bool AdvanceTime(unsigned long deltaTime);

	};
}

#endif
