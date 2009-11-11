#include "BaseMesh.h"

namespace Gen
{
	BaseMesh::BaseMesh()
		: m_BoundingRadius(0.0f)
	{
		m_OBB.Reset();
	}

	BaseMesh::~BaseMesh()
	{
		ClearMeshElements();
	}

	int BaseMesh::GetElementCount() const
	{
		return (int)m_MeshElements.size();
	}

	MeshElement* BaseMesh::GetElement(unsigned int index)
	{
		return m_MeshElements[index];
	}

	Material* BaseMesh::GetMaterial(unsigned int index)
	{
		if (index>=0 && index<m_Materials.size())
			return m_Materials[index];
		return NULL;
	}

	void BaseMesh::AddMeshElement(MeshElement* elem)
	{
		// 新元素添加到子元素列表，并更新obb尺寸
		m_MeshElements.push_back(elem);
		m_OBB.Expand(elem->GetOBB());
	}


	void BaseMesh::ClearMeshElements()
	{
		vector<MeshElement*>::iterator iter;
		for (iter=m_MeshElements.begin(); iter!=m_MeshElements.end(); iter++)
		{
			delete (*iter);
		}

		m_OBB.Reset();

		m_MeshElements.clear();

		m_Materials.clear();
	}
}
