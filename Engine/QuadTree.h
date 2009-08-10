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

	// �����Ĳ���
	void Create(float size, unsigned int subdivCount);

	// ����Ĳ����ĳߴ�
	float GetSize() const { return m_Size; }

	bool QueryObjectsByAABB(const AABB& aabb, const SceneObjectList& list);
protected:
	float			m_Size;					///< �߳�
	unsigned int	m_SubdivisionCount;		///< ϸ�ִ�����0Ϊδ��ʼ��
	Vector3f		m_WorldPosition;		///< ����ռ��е�λ��

	QuadTreeNode*	m_RootNode;				///< �Ĳ������ڵ�
};

#endif
