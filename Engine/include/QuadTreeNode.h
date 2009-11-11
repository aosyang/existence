//-----------------------------------------------------------------------------------
/// QuadTreeNode.h 四叉树节点
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _QUADTREENODE_H
#define _QUADTREENODE_H

#include "SceneObject.h"
#include "QuadTree.h"

namespace Gen
{
	class QuadTree;

	class QuadTreeNode
	{
		friend class QuadTree;
	public:
		QuadTreeNode();
		~QuadTreeNode();

		void CreateNode(QuadTree* quadtree, float size, unsigned int subdivCount, unsigned int level);
		bool QueryObjectsByAABB(const AABB& aabb, const SceneObjectList& list);

		void Render();
		void TraverseRender(const Vector3f pos);
		void TraverseRenderBox(const Vector3f vMin, const Vector3f vMax);

	protected:
		QuadTree*		m_QuadTree;				///< 该节点所属的四叉树
		unsigned int	m_SubdivisionLevel;		///< 细分级别，标记这是第几层节点
		QuadTreeNode*	m_ChildNode[4];			///< 子节点

		float			m_Size;
		float			m_OffsetX, m_OffsetZ;	///< X和Z方向上的偏移(基于四叉树的位置)

		SceneObjectSet	m_SceneObjects;
	};
}

#endif
