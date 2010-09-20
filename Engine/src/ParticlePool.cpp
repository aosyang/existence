//-----------------------------------------------------------------------------------
/// ParticlePool.cpp 粒子池，容纳并更新粒子行为
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "ParticlePool.h"
#include "Engine.h"
#include "SceneGraph.h"
#include "Renderer.h"
#include <algorithm>

namespace Gen
{
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

	ParticlePool::ParticlePool(SceneGraph* scene)
	: BaseClass(scene),
	  m_Material(NULL),
	  m_PoolCapability(100),
	  m_ActiveParticleCount(0),
	  m_SortByZOrder(true),
	  m_VanishOnEmpty(true),
	  m_UseBoxBounding(false)
	{
		for (unsigned int i=0; i<m_PoolCapability; i++)
			m_FreeOffset.push_back(i);

		// 渲染顺序靠后，通常粒子不写入Z Buffer，且具有Alpha混合
		m_RenderOrder = 110;

		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
		m_IndexBuffer = Renderer::Instance().BuildIndexBuffer();
		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Color|VFormat_Texcoord0, NULL, NULL, NULL, NULL, 4 * m_PoolCapability);
		m_IndexBuffer->CreateBuffer(NULL, 2 * m_PoolCapability);
		m_IndexBuffer->SetIndexSize(0);
	}

	ParticlePool::~ParticlePool()
	{
	}

	void ParticlePool::Destroy()
	{
		BaseClass::Destroy();

		SAFE_DELETE(m_VertexBuffer);
		SAFE_DELETE(m_IndexBuffer);
	}

	void ParticlePool::Update(unsigned long deltaTime)
	{
		BaseClass::Update(deltaTime);

		int activeCount = 0;

		// 更新所有粒子
		std::vector<Particle>::iterator iter;
		for (iter=m_Particles.begin(); iter!=m_Particles.end(); iter++)
		{
			if (iter->AdvanceTime(deltaTime))
			{
				float bound = Math::Max(iter->scale * iter->m_ScreenScaleX, iter->scale * iter->m_ScreenScaleY);
				Vector3f size = Vector3f(bound, bound, bound) * 0.5f;
				AABB aabb(iter->m_Position - size, iter->m_Position + size);
				m_AABB.Expand(aabb);
				activeCount++;
			}
		}

		m_ActiveParticleCount = activeCount;

		// 发射器如果已经销毁，当粒子池为空的时候销毁
		//if (!m_ActiveParticleCount && !m_Emitter && m_VanishOnEmpty)
		//	m_Scene->RemoveObject(this);
	}

	void ParticlePool::RenderSingleObject()
	{
		BaseClass::RenderSingleObject();

		matView = Renderer::Instance().GetViewMatrix();
		matViewInv = matView.GetInverseMatrix();

		// 排序
		sort(m_Particles.begin(), m_Particles.end(), ParticleComparer);

		// 构造顶点数据
		BuildVertexData();

		// TODO: 这样的粒子池不能够进行渲染，否则粒子方向会发生错误
		//Renderer::Instance().SetupMaterial(m_Material);
		//Renderer::Instance().RenderPrimitives(m_VertexBuffer, m_IndexBuffer, m_WorldTransform);

		RenderCommand cmd;
		cmd.indexBuffer = m_IndexBuffer;
		cmd.vertexBuffer = m_VertexBuffer;
		cmd.primType = PRIM_TRIANGLES;
		cmd.transform = m_WorldTransform;
		cmd.material = m_Material;
		cmd.renderOrder = m_RenderOrder;
		cmd.sceneObj = this;

		Renderer::Instance().SubmitRenderCommand(cmd);
	}

	//void ParticlePool::DebugRender()
	//{
	//	RenderCommand cmd;
	//	cmd.indexBuffer = m_IndexBuffer;
	//	cmd.vertexBuffer = m_VertexBuffer;
	//	cmd.primType = PRIM_LINES;
	//	cmd.transform = m_WorldTransform;
	//	cmd.renderOrder = m_RenderOrder;
	//	cmd.sceneObj = this;

	//	Renderer::Instance().SubmitRenderCommand(cmd);
	//}

	void ParticlePool::SetMaterial(BaseMaterial* mat)
	{
		m_Material = mat;
		if (mat) mat->Trigger();
	}

	void ParticlePool::BuildVertexData()
	{
		Matrix4 transform = matViewInv;
		Matrix3 localRot = transform.GetRotationMatrix();

		// 计算顶点法线信息
		Vector3f normal = localRot * Vector3f(0.0f, 0.0f, 1.0f);

		// 每帧构造新的顶点缓冲，用于渲染
		std::vector<Particle>::iterator iter = m_Particles.begin();
		for (unsigned int i=0; i<m_ActiveParticleCount; i++, iter++)
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
			float new_normal[12];

			// 生成billboard顶点的方向
			transform.SetPosition(iter->m_Position);

			Matrix3 rotMat = Matrix3::BuildRollRotationMatrix(iter->spin);
			for (int j=0; j<4; j++)
			{
				// 根据缩放等属性计算顶点位置信息
				Vector3f point = rotMat * (points[j] * iter->scale);
				//point.z = iter->m_ZOffset;
				point.x *= iter->m_ScreenScaleX;
				point.y *= iter->m_ScreenScaleY;

				point = transform * point;

				//point += iter->m_Position;
				memcpy(&new_pos[j*3], point.GetArray(), sizeof(float) * 3);

				memcpy(&new_normal[j*3], normal.GetArray(), sizeof(float) * 3);
			}

			m_VertexBuffer->ModifyVertexData(VFormat_Position, 12 * i/*iter->m_VertexOffset*/, sizeof(float) * 12, new_pos);
			m_VertexBuffer->ModifyVertexData(VFormat_Normal, 12 * i, sizeof(float) * 12, new_normal);

			float uv[] = { 0.0f, 1.0f,
				1.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 0.0f };
			m_VertexBuffer->ModifyVertexData(VFormat_Texcoord0, 8 * i, sizeof(float) * 8, uv);

		}

		// 更新索引缓冲数据，处理有效粒子数目变化的情况
		m_IndexBuffer->SetIndexSize(m_ActiveParticleCount * 2);
		iter = m_Particles.begin();
		for (unsigned int i=0; i<m_ActiveParticleCount; i++, iter++)
		{
			//unsigned int offset = iter->m_IndexOffset;
			unsigned int index[6] = { i * 4, 2 + i * 4, 1 + i * 4,
				2 + i * 4, 3 + i * 4, 1 + i * 4 };
			m_IndexBuffer->ModifyIndexData(6 * i, sizeof(unsigned int) * 6, index);
		}
	}

	void ParticlePool::AddParticle(const Particle& particle)
	{
		//unsigned int offset = m_FreeOffset.front();
		//m_FreeOffset.pop_front();

		//particle->m_IndexOffset = offset;

		bool done = false;

		// 遍历粒子列表，如果有已经结束生命周期的粒子，重新利用它
		std::vector<Particle>::iterator iter;
		for (iter=m_Particles.begin(); iter!=m_Particles.end(); iter++)
		{
			if (!iter->m_Active)
			{
				*iter = particle;
				m_ActiveParticleCount++;
				done = true;
				break;
			}
		}

		// 没有可用闲置粒子，容量允许时添加一个新的粒子
		if (!done && m_Particles.size()<m_PoolCapability)
		{
			m_Particles.push_back(particle);
			m_ActiveParticleCount++;
		}
	}
}
