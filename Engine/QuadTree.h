#ifndef _QUADTREE_H
#define _QUADTREE_H

#include "Vector3f.h"
#include "AABB.h"
#include "BaseSceneObject.h"
#include "QuadTreeNode.h"

class QuadTreeNode;

class QuadTree
{
public:
	QuadTree();
	~QuadTree();

	// 生成四叉树
	void Create(float size, unsigned int subdivCount);

	// 获得四叉树的尺寸
	float GetSize() const { return m_Size; }

	bool QueryObjectsByAABB(const AABB& aabb, const SceneObjectList& list);
protected:
	float			m_Size;					///< 边长
	unsigned int	m_SubdivisionCount;		///< 细分次数，0为未初始化
	Vector3f		m_WorldPosition;		///< 世界空间中的位置

	QuadTreeNode*	m_RootNode;				///< 四叉树根节点
};

#endif
