//-----------------------------------------------------------------------------------
/// BaseMesh.h ����ģ�ͻ���
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

		// ���ģ����Ԫ��
		void AddMeshElement(MeshElement* elem);

		// ���������Ԫ��
		void ClearMeshElements();

	protected:
		const String*			m_Name;

		float					m_BoundingRadius;		///< ��Χ��뾶
		OBB						m_OBB;					///< ��Χ�гߴ�

		vector<MeshElement*>	m_MeshElements;			///< ��Ԫ�ؼ���
		vector<Material*>		m_Materials;
	};
}

#endif

