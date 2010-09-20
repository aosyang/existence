//-----------------------------------------------------------------------------------
/// ParticleEmitter.h ���ӷ����������������
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
	///	���ӷ�����
	/// @par
	///		Ϊÿһ������������ָ����״̬
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
		///	ָ�����ӷ�����
		/// @param interval
		///		���ʱ��(��λ������)
		/// @remarks
		///		�趨Ϊ-1ʱ��������ֹͣ����
		void SetInterval(long interval) { m_Interval = interval; }
		long GetInverval() const { return m_Interval; }

		/// @brief
		///	ָ����������
		void SetParticleLifetime(long life) { m_Lifetime = life; }

		/// @brief
		///	ָ�����ӵĳ�����
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

		// ��������
		long					m_Lifetime;					///< ��������
		float					m_InitialSpeed;				///< ���ӳ�ʼ����
		float					m_InitialScale;				///< ���ӳ�ʼ��С
		float					m_ScaleInc;					///< ��С����
		float					m_Spin;						///< ˳ʱ����ת��(��λ������)
		float					m_SpinVar;					///< ѡװ���仯��
		float					m_SpinInc;					///< ��ת������
		float					m_SpinIncVar;				///< ��ת�������仯��

		float					m_SpreadBegin;
		float					m_SpreadEnd;

		float					m_SpreadZBegin;
		float					m_SpreadZEnd;

		//ParticleInitStateFunc	m_ParticleInitState;
		//ParticleUpdateFunc		m_ParticleBehaviorFunc;
	};
}

#endif

