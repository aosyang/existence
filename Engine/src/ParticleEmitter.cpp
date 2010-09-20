//-----------------------------------------------------------------------------------
/// ParticleEmitter.cpp ���ӷ����������������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------


#include "ParticleEmitter.h"
#include "SceneGraph.h"

namespace Gen
{
	void DefaultParticleState(Particle* particle, ParticleEmitter* emitter)
	{
		particle->m_Age = 0;
		particle->m_Duration= 5000;
		particle->m_Velocity = Vector3f(0.0f, 1.0f, 0.0f);
	}

	ParticleEmitter::ParticleEmitter(SceneGraph* scene)
	: SceneObject(scene),
	  m_EmitterShape(EMITTER_SHAPE_POINT),
	  m_ParticlePool(NULL),
	  m_Material(NULL),
	  m_BoxMin(0.0f, 0.0f, 0.0f),
	  m_BoxMax(0.0f, 0.0f, 0.0f),
	  m_Interval(-1),
	  m_TimeSinceLastUpdate(0),

	  m_Lifetime(1000),
	  m_InitialSpeed(0.0f),
	  m_InitialScale(1.0f),
	  m_ScaleInc(0.0f),
	  m_Spin(0.0f),
	  m_SpinVar(0.0f),
	  m_SpinInc(0.0f),
	  m_SpinIncVar(0.0f),
	  m_SpreadBegin(0.0f),
	  m_SpreadEnd(360.0f),
	  m_SpreadZBegin(-1.0f),
	  m_SpreadZEnd(1.0f)

	  //m_ParticleInitState(&DefaultParticleState),
	  //m_ParticleBehaviorFunc(NULL)
	{
	}

	ParticleEmitter::~ParticleEmitter()
	{
	}

	void ParticleEmitter::Destroy()
	{
		// ���ӳ��������������п������ٷ�����ʱ�������Ӵ��ڻ״̬
		if (m_ParticlePool)
			m_ParticlePool->m_Emitter = NULL;
	}

	void ParticleEmitter::Update(unsigned long deltaTime)
	{
		// TODO: ��������Ϊ�˷�ֹm_SceneΪNULL�������ַ���������
		if (!m_ParticlePool)
		{
			// ��Ҫ�����ӳ��ڷ�����������ʱ����������
			m_ParticlePool = FACTORY_CREATE(m_Scene, ParticlePool);
			m_ParticlePool->m_Emitter = this;
			m_ParticlePool->SetMaterial(m_Material);
		}

		SceneObject::Update(deltaTime);

		// ���ڷ�������

		// ���Ϊ-1ʱֹͣ���ӷ���
		if (m_Interval == -1) return;

		// �������ӷ���ʱ��
		if (m_TimeSinceLastUpdate<m_Interval)
			m_TimeSinceLastUpdate += deltaTime;
		else
		{
			// �����ӳ�δ��
			while (m_TimeSinceLastUpdate>=m_Interval && !m_ParticlePool->IsFull())
			{
				Particle particle;
				particle.m_Active = true;
				switch (m_EmitterShape)
				{
				case EMITTER_SHAPE_BOX:
					particle.m_Position = Vector3f(Math::Random(m_BoxMin.x, m_BoxMax.x),
						Math::Random(m_BoxMin.y, m_BoxMax.y),
						Math::Random(m_BoxMin.z, m_BoxMax.z));
					particle.m_Position = m_WorldTransform * particle.m_Position;
					break;

				case EMITTER_SHAPE_POINT:
				default:
					particle.m_Position = m_WorldTransform.GetPosition();
					break;
				}
				//(*m_ParticleInitState)(&particle, this);
				//if (m_ParticleBehaviorFunc)
				//	particle.m_UpdateFunc = m_ParticleBehaviorFunc;

				// �������ӵĳ�ʼ״̬

				// ���ٶ��ɳ����ʼ����������Ϸ������
				particle.m_Duration = m_Lifetime;
				//particle.m_Velocity = m_WorldTransform.GetUpVector() * m_InitialSpeed;

				float vy = cosf(DEG_TO_RAD(Math::Random(m_SpreadZBegin, m_SpreadZEnd)));
				float angle = DEG_TO_RAD(Math::Random(m_SpreadBegin, m_SpreadEnd));
				float t = sqrtf(1 - vy * vy);
				float vx = cosf(angle) * t;
				float vz = sinf(angle) * t;
				particle.m_Velocity = Vector3f(vx, vy, vz) * m_InitialSpeed;
				particle.scale = m_InitialScale;
				particle.scaleInc = m_ScaleInc;
				particle.spin = Math::Random(m_Spin - fabs(m_SpinVar), m_Spin + fabs(m_SpinVar));
				particle.spinInc = Math::Random(m_SpinInc - fabs(m_SpinIncVar), m_SpinInc + fabs(m_SpinIncVar));

				m_ParticlePool->AddParticle(particle);

				m_TimeSinceLastUpdate -= m_Interval;
				particle.AdvanceTime(m_TimeSinceLastUpdate);
			}
		}
	}

	void ParticleEmitter::SetMaterial(const String& matName)
	{
		BaseMaterial* mat = MaterialManager::Instance().GetByName(matName);
		SetMaterial(mat);
	}

	void ParticleEmitter::SetMaterial(BaseMaterial* material)
	{
		m_Material = material;
		if (m_ParticlePool)
			m_ParticlePool->SetMaterial(material);
		if (material) material->Trigger();
	}

	Particle* ParticleEmitter::EmitParticle()
	{
		return NULL;
	}

}
