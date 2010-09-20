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

	/// @brief
	///	粒子发射器
	/// @par
	///		为每一个产生的粒子指定初状态
	class ParticleEmitter : public SceneObject
	{
		DECLARE_FACTORY_OBJECT(ParticleEmitter, SceneObject);
	public:
		// ----- Overwrite IObject

		void Destroy();

		void Update(unsigned long deltaTime);

		// ----- ParticleEmitter Methods
		void SetEmitterShape(ParticleEmitterShape shape) { m_EmitterShape = shape; }
		ParticleEmitterShape GetEmitterShape() const { return m_EmitterShape; }

		void SetBoxRange(const Vector3f& vMin, const Vector3f& vMax) { m_BoxMin = vMin; m_BoxMax = vMax; }

		/// @brief
		///	指定粒子发射间隔
		/// @param interval
		///		间隔时间(单位：毫秒)
		/// @remarks
		///		设定为-1时发射器将停止工作
		void SetInterval(long interval) { m_Interval = interval; }
		long GetInverval() const { return m_Interval; }

		/// @brief
		///	指定粒子寿命
		void SetParticleLifetime(long life) { m_Lifetime = life; }

		/// @brief
		///	指定粒子的初速率
		void SetInitialSpeed(float speed) { m_InitialSpeed = speed; }

		void SetInitialScale(float scale) { m_InitialScale = scale; }
		void SetScaleInc(float scaleInc) { m_ScaleInc = scaleInc; }

		void SetSpin(float rad_angle) { m_Spin = rad_angle; }
		void SetSpinVar(float var) { m_SpinVar = var; }
		void SetSpinInc(float inc) { m_SpinInc = inc; }
		void SetSpinIncVar(float var) { m_SpinIncVar = var; }

		void SetSpreadBegin(float spread) { m_SpreadBegin = spread; }
		void SetSpreadEnd(float spread) { m_SpreadEnd = spread; }

		void SetSpreadZBegin(float spread) { m_SpreadZBegin = spread; }
		void SetSpreadZEnd(float spread) { m_SpreadZEnd = spread; }

		void SetMaterial(const String& matName);
		void SetMaterial(BaseMaterial* material);
		inline BaseMaterial* GetMaterial() { return m_Material; }

		//void SetParticleInitStateFunc(const ParticleInitStateFunc func) { m_ParticleInitState = func; }
		//void SetParticleBehaviorFunc(const ParticleUpdateFunc func) { m_ParticleBehaviorFunc = func; }

	protected:
		Particle* EmitParticle();

	private:
		ParticleEmitterShape	m_EmitterShape;
		ParticlePool*			m_ParticlePool;

		BaseMaterial*			m_Material;

		Vector3f				m_BoxMin;
		Vector3f				m_BoxMax;

		long					m_Interval;
		unsigned int			m_TimeSinceLastUpdate;

		// 粒子属性
		long					m_Lifetime;					///< 粒子寿命
		float					m_InitialSpeed;				///< 粒子初始速率
		float					m_InitialScale;				///< 粒子初始大小
		float					m_ScaleInc;					///< 大小增量
		float					m_Spin;						///< 顺时针旋转量(单位：弧度)
		float					m_SpinVar;					///< 选装量变化量
		float					m_SpinInc;					///< 旋转量增量
		float					m_SpinIncVar;				///< 旋转量增量变化量

		float					m_SpreadBegin;
		float					m_SpreadEnd;

		float					m_SpreadZBegin;
		float					m_SpreadZEnd;

		//ParticleInitStateFunc	m_ParticleInitState;
		//ParticleUpdateFunc		m_ParticleBehaviorFunc;
	};
}

#endif

