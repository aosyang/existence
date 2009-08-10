//-----------------------------------------------------------------------------------
/// ParticlePool.cpp 粒子池，容纳并更新粒子行为
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "ParticlePool.h"
#include <algorithm>
#include "Engine.h"
#include "SceneGraph.h"

Matrix4	matViewInv;
Matrix4 matView;

// 对粒子进行排序
bool ParticleComparer(Particle lhs, Particle rhs)
{
	bool result;

	if (lhs.m_Active == rhs.m_Active)
	{
		// 按照与摄像机的距离由远及近排序
		float ldist = (matView * lhs.m_Position).z;
		float rdist = (matView * rhs.m_Position).z;

		result = (ldist < rdist);
	}
	else
		// 活动的粒子排在前面
		result = (lhs.m_Active > rhs.m_Active);

	return result;
}

ParticlePool::ParticlePool()
: BaseSceneObject(),
  m_Material(NULL),
  m_PoolCapability(100),
  m_ActiveParticleCount(0),
  m_SortByZOrder(true),
  m_VanishOnEmpty(true),
  m_UseBoxBounding(false)
{
	for (int i=0; i<m_PoolCapability; i++)
		m_FreeOffset.push_back(i);

	// 渲染顺序靠后，通常粒子不写入Z Buffer，且具有Alpha混合
	m_RenderOrder = 110;

	m_VertexBuffer = renderer->BuildVertexBuffer();
	m_VertexBuffer->CreateBuffer(VFormat_Position/*|VFormat_Normal*/|VFormat_Color|VFormat_Texcoord0, NULL, NULL, NULL, NULL, 4 * m_PoolCapability, NULL, 2 * m_PoolCapability);
	m_VertexBuffer->SetIndexSize(0);
}

ParticlePool::~ParticlePool()
{
	delete m_VertexBuffer;
}

void ParticlePool::Update(unsigned long deltaTime)
{
	BaseSceneObject::Update(deltaTime);

	int activeCount = 0;

	// 更新所有粒子
	vector<Particle>::iterator iter;
	for (iter=m_Particles.begin(); iter!=m_Particles.end(); iter++)
	{
		if (iter->m_Active)
		{
			iter->Update(deltaTime);

			float bound = Math::Max(iter->m_Scale * iter->m_ScreenScaleX, iter->m_Scale * iter->m_ScreenScaleY);
			Vector3f size = Vector3f(bound, bound, bound) * 0.5f;
			AABB aabb(iter->m_Position - size, iter->m_Position + size);
			m_AABB.Expand(aabb);
			activeCount++;
		}
	}

	m_ActiveParticleCount = activeCount;

	// 发射器如果已经销毁，当粒子池为空的时候销毁
	if (!m_ActiveParticleCount && !m_Emitter && m_VanishOnEmpty)
		m_Scene->RemoveObject(this);
}

void ParticlePool::Render()
{
	matView = renderer->ViewMatrix();
	matViewInv = renderer->ViewMatrix().GetInverseMatrix();

	// 排序
	stable_sort(m_Particles.begin(), m_Particles.end(), ParticleComparer);

	// 构造顶点数据
	BuildVertexData();

	// TODO: 这样的粒子池不能够进行渲染，否则粒子方向会发生错误
	renderer->RenderVertexBuffer(m_VertexBuffer, m_Material, m_WorldTransform);
	
	BaseSceneObject::Render();
}

void ParticlePool::BuildVertexData()
{
	// 每帧构造新的顶点缓冲，用于渲染
	vector<Particle>::iterator iter = m_Particles.begin();
	for (int i=0; i<m_ActiveParticleCount; i++, iter++)
	{
		// 更新顶点颜色
		float new_color[16];
		for (int j=0; j<4; j++)
			memcpy(&(new_color[j*4]), iter->m_Color.GetArray(), sizeof(float) * 4);

		m_VertexBuffer->ModifyVertexData(VFormat_Color, 16 * i/*iter->m_VertexOffset*/, sizeof(float) * 16, new_color);

		Vector3f points[4] = { Vector3f(-0.5f, 0.5f, 0.0f),
			Vector3f(0.5f, 0.5f, 0.0f),
			Vector3f(-0.5f, -0.5f, 0.0f),
			Vector3f(0.5f, -0.5f, 0.0f) };

		float new_pos[12];

		Matrix3 rotMat = Matrix3::BuildRollRotationMatrix(iter->m_ZRotation);
		for (int j=0; j<4; j++)
		{
			Vector3f point = rotMat * (points[j] * iter->m_Scale);
			//point.z = iter->m_ZOffset;
			point.x *= iter->m_ScreenScaleX;
			point.y *= iter->m_ScreenScaleY;

			// 生成billboard顶点的方向
			Matrix4 transform = matViewInv;
			transform.SetPosition(iter->m_Position);

			point = transform * point;

			//point += iter->m_Position;
			memcpy(&new_pos[j*3], point.GetArray(), sizeof(float) * 3);
		}

		m_VertexBuffer->ModifyVertexData(VFormat_Position, 12 * i/*iter->m_VertexOffset*/, sizeof(float) * 12, new_pos);

		float uv[] = { 0.0f, 1.0f,
					   1.0f, 1.0f,
					   0.0f, 0.0f,
					   1.0f, 0.0f };
		m_VertexBuffer->ModifyVertexData(VFormat_Texcoord0, 8 * i, sizeof(float) * 8, uv);
	}

	m_VertexBuffer->SetIndexSize(m_ActiveParticleCount * 2);
	iter = m_Particles.begin();
	for (int i=0; i<m_ActiveParticleCount; i++, iter++)
	{
		//unsigned int offset = iter->m_IndexOffset;
		unsigned int index[6] = { i * 4, 2 + i * 4, 1 + i * 4,
								  2 + i * 4, 3 + i * 4, 1 + i * 4 };
		m_VertexBuffer->ModifyIndexData(6 * i, sizeof(unsigned int) * 6, index);
	}
}

void ParticlePool::AddParticle(const Particle& particle)
{
	//unsigned int offset = m_FreeOffset.front();
	//m_FreeOffset.pop_front();

	//particle->m_IndexOffset = offset;

	bool done = false;

	vector<Particle>::iterator iter;
	for (iter=m_Particles.begin(); iter!=m_Particles.end(); iter++)
	{
		if (!iter->m_Active)
		{
			*iter = particle;
			m_ActiveParticleCount++;
			break;
		}
	}
	
	if (!done && m_Particles.size()<m_PoolCapability)
	{
		m_Particles.push_back(particle);
	}
}
