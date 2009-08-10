#ifndef _QUADTREENODE_H
#define _QUADTREENODE_H

#include "BaseSceneObject.h"

class QuadTree;

class QuadTreeNode
{
	friend class QuadTree;
public:
	QuadTreeNode();
	~QuadTreeNode();

	void CreateNode(QuadTree* quadtree, unsigned int subdivCount, unsigned int level);
	bool QueryObjectsByAABB(const AABB& aabb, const SceneObjectList& list);

protected:
	QuadTree*		m_QuadTree;				///< 该节点所属的四叉树
	unsigned int	m_SubdivisionLevel;		///< 细分级别，标记这是第几层节点
	QuadTreeNode*	m_ChildNode[4];			///< 子节点

	float			m_OffsetX, m_OffsetZ;	///< X和Z方向上的偏移(基于四叉树的位置)

	ChildrenSceneObjectsSet	m_SceneObjects;
};

#endif
