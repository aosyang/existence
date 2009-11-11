//-----------------------------------------------------------------------------------
/// ParticleEmitter.h 粒子发射器，会喷出粒子
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _PARTICLEEMITTER_H
#define _PARTICLEEMITTER_H

#include "SceneObject.h"
#include "ParticlePool.h"

namespace Gen
{
	enum ParticleEmitterShape
	{
		EMITTER_SHAPE_POINT,
		EMITTER_SHAPE_BOX,
	};

	class ParticleEmitter;

	typedef void(*ParticleInitStateFunc)(Particle*, ParticleEmitter*);

	void DefaultParticleState(Particle* particle, ParticleEmitter* emitter);

	class ParticleEmitter : public SceneObject
	{
		DECLARE_FACTORY(ParticleEmitter);
	public:
		// ----- Overwrite IObject

		void Destroy();

		void Update(unsigned long deltaTime);

		// ----- SceneObject methods
		void CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum);

		// ----- ParticleEmitter Methods
		void SetEmitterShape(ParticleEmitterShape shape) { m_EmitterShape = shape; }
		ParticleEmitterShape GetEmitterShape() const { return m_EmitterShape; }

		void SetBoxRange(const Vector3f& vMin, const Vector3f& vMax) { m_BoxMin = vMin; m_BoxMax = vMax; }

		void SetInterval(long interval) { m_Interval = interval; }
		long GetInverval() const { return m_Interval; }


		void SetMaterial(Material* material);
		inline Material* GetMaterial() { return m_Material; }

		void SetParticleInitStateFunc(const ParticleInitStateFunc func) { m_ParticleInitState = func; }
		void SetParticleBehaviorFunc(const ParticleUpdateFunc func) { m_ParticleBehaviorFunc = func; }
	private:
		ParticleEmitterShape	m_EmitterShape;
		ParticlePool*			m_ParticlePool;

		Material*				m_Material;

		Vector3f				m_BoxMin;
		Vector3f				m_BoxMax;

		long					m_Interval;
		unsigned int			m_TimeSinceLastUpdate;

		ParticleInitStateFunc	m_ParticleInitState;
		ParticleUpdateFunc		m_ParticleBehaviorFunc;
	};
}

#endif

