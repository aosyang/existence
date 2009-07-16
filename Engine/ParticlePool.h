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
#include "BaseSceneObject.h"
#include "Particle.h"
#include "Material.h"
#include "ParticleEmitter.h"
#include <list>

using namespace std;

bool ParticleComparer(Particle lhs, Particle rhs);

class ParticlePool : public BaseSceneObject
{
	friend class ParticleEmitter;
public:
	ParticlePool();
	~ParticlePool();

	// ----- Overwrite ISceneObject

	void Update(unsigned long deltaTime);
	void Render();
	//void DebugRender();

	// ----- ParticlePool Methods
	inline Material* GetMaterial() const { return m_Material; }
	inline void SetMaterial(Material* mat) { m_Material = mat; }

	// 池中粒子是否已满
	bool IsFull() const { return (m_ActiveParticleCount>=m_PoolCapability); }
	void BuildVertexData();

	void AddParticle(const Particle& particle);
private:
	IVertexBuffer*		m_VertexBuffer;
	Material*			m_Material;
	ParticleEmitter*	m_Emitter;


	int		m_PoolCapability;
	int		m_ActiveParticleCount;

	list<unsigned int>	m_FreeOffset;

	list<Particle>		m_Particles;

	bool	m_SortByZOrder;
	bool	m_VanishOnEmpty;		///< 粒子生命结束，销毁粒子池

	bool	m_UseBoxBounding;
	bool	m_BoundingWrapRepeat;
	Vector3f	m_BoundingMin;
	Vector3f	m_BoundingMax;
};

#endif
