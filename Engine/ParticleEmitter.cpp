//-----------------------------------------------------------------------------------
/// ParticleEmitter.cpp 粒子发射器，会喷出粒子
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------


#include "ParticleEmitter.h"
#include "SceneGraph.h"

void DefaultParticleState(Particle* particle, ParticleEmitter* emitter)
{
	particle->m_LifeTime = 5000;
	particle->m_Velocity = Vector3f(0.0f, 1.0f, 0.0f);
}

ParticleEmitter::ParticleEmitter()
: BaseSceneObject(),
  m_PoolCreated(false),
  m_EmitterShape(EMITTER_SHAPE_POINT),
  m_Material(NULL),
  m_BoxMin(0.0f, 0.0f, 0.0f),
  m_BoxMax(0.0f, 0.0f, 0.0f),
  m_Interval(-1),
  m_TimeSinceLastUpdate(0),
  m_ParticleInitState(&DefaultParticleState),
  m_ParticleBehaviorFunc(NULL)
{
}

ParticleEmitter::~ParticleEmitter()
{
	// 粒子池留着自生自灭，有可能销毁发射器时仍有粒子处于活动状态
	m_ParticlePool->m_Emitter = NULL;
}

void ParticleEmitter::Update(unsigned long deltaTime)
{
	if (!m_PoolCreated)
	{
		// 不要让粒子池在发射器还存在时就自行销毁
		m_ParticlePool = new ParticlePool();
		m_ParticlePool->m_Emitter = this;
		m_Scene->AddObject(m_ParticlePool);
		m_PoolCreated = true;
		m_ParticlePool->SetMaterial(m_Material);
	}

	BaseSceneObject::Update(deltaTime);

	if (m_Interval == -1) return;

	if (m_TimeSinceLastUpdate<m_Interval)
		m_TimeSinceLastUpdate += deltaTime;
	else
	{
		if (!m_ParticlePool->IsFull())
		{
			Particle particle;
			particle.m_Active = true;
			switch (m_EmitterShape)
			{
			case EMITTER_SHAPE_BOX:
				particle.m_Position = Vector3f(MathRandom(m_BoxMin.x, m_BoxMax.x),
											   MathRandom(m_BoxMin.y, m_BoxMax.y),
											   MathRandom(m_BoxMin.z, m_BoxMax.z));
				particle.m_Position = m_WorldTransform * particle.m_Position;
				break;

			case EMITTER_SHAPE_POINT:
			default:
				particle.m_Position = m_WorldTransform.GetPosition();
				break;
			}
			(*m_ParticleInitState)(&particle, this);
			if (m_ParticleBehaviorFunc)
				particle.m_UpdateFunc = m_ParticleBehaviorFunc;
			m_ParticlePool->AddParticle(particle);

			m_TimeSinceLastUpdate = 0;
		}
	}
}

void ParticleEmitter::SetMaterial(Material* material)
{
	m_Material = material;
	if (m_PoolCreated)
		m_ParticlePool->SetMaterial(material);
}
