#include "QuadTreeNode.h"
#include "Platform.h"

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

void QuadTreeNode::CreateNode(QuadTree* quadtree, unsigned int subdivCount, unsigned int level)
{
	m_QuadTree = quadtree;

	if (level<subdivCount)
	{
		float offsetx[4] = { 0.0f, m_OffsetX / 2, 0.0f, m_OffsetX / 2 };
		float offsetz[4] = { 0.0f, 0.0f, m_OffsetZ / 2, m_OffsetZ / 2 };

		for (int i=0; i<4; i++)
		{
			m_ChildNode[i] = new QuadTreeNode();
			m_ChildNode[i]->CreateNode(quadtree, subdivCount, level+1);

			m_ChildNode[i]->m_OffsetX = offsetx[i];
			m_ChildNode[i]->m_OffsetZ = offsetz[i];
		}
	}
}

bool QuadTreeNode::QueryObjectsByAABB(const AABB& aabb, const SceneObjectList& list)
{
	bool result = false;

	ChildrenSceneObjectsSet::iterator iter;
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
