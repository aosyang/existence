#include "QuadTree.h"
#include "Platform.h"

QuadTree::QuadTree()
: m_Size(0.0f),
  m_SubdivisionCount(0),
  m_WorldPosition(0.0f, 0.0f, 0.0f),
  m_RootNode(NULL)
{
}

QuadTree::~QuadTree()
{
	SAFE_DELETE(m_RootNode);
}

void QuadTree::Create(float size, unsigned int subdivCount)
{
	m_Size = size;
	m_SubdivisionCount = subdivCount;

	m_RootNode = new QuadTreeNode();
	m_RootNode->m_OffsetX = m_RootNode->m_OffsetZ = 0.0f;
	m_RootNode->CreateNode(this, size, subdivCount, 0);
}

bool QuadTree::QueryObjectsByAABB(const AABB& aabb, const SceneObjectList& list)
{
	return m_RootNode->QueryObjectsByAABB(aabb, list);
}

void QuadTree::Render()
{
	m_RootNode->Render();
}

void QuadTree::TraverseRender(const Vector3f pos)
{
	m_RootNode->TraverseRender(pos);
}
	
void QuadTree::TraverseRenderBox(const Vector3f vMin, const Vector3f vMax)
{
	m_RootNode->TraverseRenderBox(vMin, vMax);
}
