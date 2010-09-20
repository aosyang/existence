//-----------------------------------------------------------------------------------
/// ParticlePool.h 粒子池，容纳并更新粒子行为
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _PARTICLEPOOL_H
#define _PARTICLEPOOL_H

#include "IVertexBuffer.h"
#include "GameObject.h"
#include "Particle.h"
#include "Material.h"
#include "ParticleEmitter.h"
#include <list>
#include <vector>



namespace Gen
{
	class ParticleEmitter;

	bool ParticleComparer(Particle lhs, Particle rhs);

	class ParticlePool : public GameObject
	{
		friend class ParticleEmitter;
		DECLARE_FACTORY_OBJECT(ParticlePool, GameObject);
	public:
		// ----- Overwrite ISceneObject

		void Destroy();

		void Update(unsigned long deltaTime);

		// ----- Overwrite SceneObject
	protected:
		void RenderSingleObject();

		//void DebugRender();

	public:
		// ----- ParticlePool Methods
		inline BaseMaterial* GetMaterial() const { return m_Material; }
		void SetMaterial(BaseMaterial* mat);

		// 池中粒子是否已满
		bool IsFull() const { return (m_ActiveParticleCount>=m_PoolCapability); }
		void BuildVertexData();

		void AddParticle(const Particle& particle);
	private:
		IVertexBuffer*		m_VertexBuffer;
		IIndexBuffer*		m_IndexBuffer;
		BaseMaterial*		m_Material;
		ParticleEmitter*	m_Emitter;


		unsigned int		m_PoolCapability;
		unsigned int		m_ActiveParticleCount;

		std::list<unsigned int>	m_FreeOffset;

		std::vector<Particle>		m_Particles;

		bool	m_SortByZOrder;
		bool	m_VanishOnEmpty;		///< 粒子生命结束，销毁粒子池

		bool	m_UseBoxBounding;
		bool	m_BoundingWrapRepeat;
		Vector3f	m_BoundingMin;
		Vector3f	m_BoundingMax;
	};
}

#endif
