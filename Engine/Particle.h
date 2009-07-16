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
#include <memory>

class ParticlePool;
struct Particle;

typedef bool(*ParticleUpdateFunc)(Particle*, unsigned long);

bool DefaultParticleUpdateBehavior(Particle* particle, unsigned long deltaTime);

struct Particle
{
	bool		m_Active;
	Vector3f	m_Position;
	Vector3f	m_Velocity;

	Color4f		m_Color;

	long		m_LifeTime;

	float		m_ZRotation;
	float		m_ZRotationInc;

	float		m_Scale;
	float		m_ScaleInc;

	float		m_ScreenScaleX;
	float		m_ScreenScaleY;

	ParticleUpdateFunc	m_UpdateFunc;

	//int			m_VertexOffset;

	//unsigned int m_IndexOffset;

	Particle()
		: m_Active(false),
		  m_Position(0.0f, 0.0f, 0.0f),
		  m_Velocity(0.0f, 0.0f, 0.0f),
		  m_LifeTime(-1),
		  m_ZRotation(0.0f),
		  m_ZRotationInc(0.0f),
		  m_Scale(1.0f),
		  m_ScaleInc(0.0f),
		  m_ScreenScaleX(1.0f),
		  m_ScreenScaleY(1.0f),
		  m_UpdateFunc(&DefaultParticleUpdateBehavior)
	{
	}

	Particle operator=(const Particle& rhs)
	{
		if (&rhs != this)
			memcpy(this, &rhs, sizeof(Particle));

		return *this;
	}

	// 更新粒子，如果粒子寿命结束返回false，其他情况返回true
	bool Update(unsigned long deltaTime)
	{
		return (*m_UpdateFunc)(this, deltaTime);
	}

};


#endif
