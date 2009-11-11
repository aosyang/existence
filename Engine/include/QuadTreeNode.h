//-----------------------------------------------------------------------------------
/// QuadTreeNode.h �Ĳ����ڵ�
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
		QuadTree*		m_QuadTree;				///< �ýڵ��������Ĳ���
		unsigned int	m_SubdivisionLevel;		///< ϸ�ּ��𣬱�����ǵڼ���ڵ�
		QuadTreeNode*	m_ChildNode[4];			///< �ӽڵ�

		float			m_Size;
		float			m_OffsetX, m_OffsetZ;	///< X��Z�����ϵ�ƫ��(�����Ĳ�����λ��)

		SceneObjectSet	m_SceneObjects;
	};
}

#endif
