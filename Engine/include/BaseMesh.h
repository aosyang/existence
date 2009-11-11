//-----------------------------------------------------------------------------------
/// BaseMesh.h 网格模型基类
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _BASEMESH_H
#define _BASEMESH_H

#include "IMesh.h"

#include <vector>

using namespace std;

namespace Gen
{
	class BaseMesh : public IMesh
	{
		friend class MeshManager;
	public:
		BaseMesh();
		~BaseMesh();

		int GetElementCount() const;
		MeshElement* GetElement(unsigned int index);

		Material* GetMaterial(unsigned int index);

		float GetBoundingRadius() const { return m_BoundingRadius; }
		const OBB& GetOBB() const { return m_OBB; }

		String GetName() const { return *m_Name; }
		void SetNamePtr(const String* namePtr) { m_Name = namePtr; }

	protected:

		// 添加模型子元素
		void AddMeshElement(MeshElement* elem);

		// 清除所有子元素
		void ClearMeshElements();

	protected:
		const String*			m_Name;

		float					m_BoundingRadius;		///< 包围球半径
		OBB						m_OBB;					///< 包围盒尺寸

		vector<MeshElement*>	m_MeshElements;			///< 子元素集合
		vector<Material*>		m_Materials;
	};
}

#endif

