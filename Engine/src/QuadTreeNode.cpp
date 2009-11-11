//-----------------------------------------------------------------------------------
/// QuadTreeNode.cpp 四叉树节点实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "QuadTreeNode.h"
#include "Platform.h"
#include "Engine.h"

namespace Gen
{
	class QuadTree;

	QuadTreeNode::QuadTreeNode()
	{
		for (int i=0; i<4; i++)
		{
			m_ChildNode[i] = NULL;
		}
	}

	QuadTreeNode::~QuadTreeNode()
	{
		for (int i=0; i<4; i++)
		{
			SAFE_DELETE(m_ChildNode[i]);
		}
	}

	void QuadTreeNode::CreateNode(QuadTree* quadtree, float size, unsigned int subdivCount, unsigned int level)
	{
		m_QuadTree = quadtree;
		m_Size = size;
		m_SubdivisionLevel = level;

		if (level<subdivCount)
		{
			float offsetx[4] = { 0.0f, size / 2, 0.0f, size / 2 };
			float offsetz[4] = { 0.0f, 0.0f, size / 2, size / 2 };

			for (int i=0; i<4; i++)
			{
				m_ChildNode[i] = new QuadTreeNode();
				m_ChildNode[i]->m_OffsetX = m_OffsetX + offsetx[i];
				m_ChildNode[i]->m_OffsetZ = m_OffsetZ + offsetz[i];

				m_ChildNode[i]->CreateNode(quadtree, size/2, subdivCount, level+1);
			}
		}
	}

	bool QuadTreeNode::QueryObjectsByAABB(const AABB& aabb, const SceneObjectList& list)
	{
		bool result = false;

		SceneObjectSet::iterator iter;
		for (iter=m_SceneObjects.begin(); iter!=m_SceneObjects.end(); iter++)
		{
			// TODO: 如果物体与这个aabb相交，添加到list中
		}

		for (int i=0; i<4; i++)
		{
			result |= m_ChildNode[i]->QueryObjectsByAABB(aabb, list);
		}

		return result;
	}

	void QuadTreeNode::Render()
	{
		float height = m_QuadTree->GetSize();
		Vector3f vMin = Vector3f(m_OffsetX, 0.0f, m_OffsetZ);
		Vector3f vMax = Vector3f(m_OffsetX + m_Size, height, m_OffsetZ + m_Size);
		renderer->RenderBox(vMin, vMax);

		if (m_ChildNode[0])
			for (int i=0; i<4; i++)
				m_ChildNode[i]->Render();
	}

	void QuadTreeNode::TraverseRender(const Vector3f pos)
	{
		float height = m_QuadTree->GetSize();
		Vector3f vMin = Vector3f(m_OffsetX, 0.0f, m_OffsetZ);
		Vector3f vMax = Vector3f(m_OffsetX + m_Size, height, m_OffsetZ + m_Size);

#define VECTOR_LESS(a, b) ((a.x < b.x) && (a.y < b.y) && (a.z < b.z))
#define VECTOR_GREATER(a, b) ((a.x > b.x) && (a.y > b.y) && (a.z > b.z))
		if (VECTOR_GREATER(pos, vMin) && VECTOR_LESS(pos, vMax))
		{
			renderer->RenderBox(vMin, vMax);

			if (!m_ChildNode[0]) return;

			for (int i=0; i<4; i++)
			{
				m_ChildNode[i]->TraverseRender(pos);
			}
		}

	}

	void QuadTreeNode::TraverseRenderBox(const Vector3f vMin, const Vector3f vMax)
	{
		float height = m_QuadTree->GetSize();
		Vector3f vNodeMin = Vector3f(m_OffsetX, 0.0f, m_OffsetZ);
		Vector3f vNodeMax = Vector3f(m_OffsetX + m_Size, height, m_OffsetZ + m_Size);

		if (VECTOR_GREATER(vMin, vNodeMin) && VECTOR_LESS(vMax, vNodeMax))
		{
			renderer->RenderBox(vNodeMin, vNodeMax);

			if (!m_ChildNode[0]) return;

			for (int i=0; i<4; i++)
			{
				m_ChildNode[i]->TraverseRenderBox(vMin, vMax);
			}
		}

	}
}
