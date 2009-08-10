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
	QuadTree*		m_QuadTree;				///< �ýڵ��������Ĳ���
	unsigned int	m_SubdivisionLevel;		///< ϸ�ּ��𣬱�����ǵڼ���ڵ�
	QuadTreeNode*	m_ChildNode[4];			///< �ӽڵ�

	float			m_OffsetX, m_OffsetZ;	///< X��Z�����ϵ�ƫ��(�����Ĳ�����λ��)

	ChildrenSceneObjectsSet	m_SceneObjects;
};

#endif
