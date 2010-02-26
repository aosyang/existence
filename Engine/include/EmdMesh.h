//-----------------------------------------------------------------------------------
/// EmdMesh.h Emd�����࣬���涥����Ϣ�����Դ�emd�ļ�����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _EMDMESH_H
#define _EMDMESH_H

#include "Vector3f.h"
#include "Matrix4.h"
#include "Color4f.h"
#include "Platform.h"
#include "MeshObject.h"
#include "Debug.h"
#include "Material.h"
#include "OBB.h"
#include "BaseMesh.h"

#include <vector>
#include "EString.h"
#include <fstream>

using namespace std;

namespace Gen
{
	class MeshElement;
	class EmdMesh;

	enum EmdlLump
	{
		EMDL_LUMP_MATERIALS = 100,
		EMDL_LUMP_MESH_ELEMENTS,
		EMDL_LUMP_VERTICES,
	};


	//-----------------------------------------------------------------------------------
	/// \brief
	/// ģ���࣬�洢��Ⱦ���������Ϣ
	/// 
	/// ģ�Ϳ��Դ��ļ���ȡ����ֱ�Ӵ���һ������������
	/// 
	/// \remarks
	/// ����Ķ��㼰������Ϣ���Ա���Ⱦ�����ý�����Ⱦ����ͬ��Ⱦ������Թ���ͬһ��ģ����Դ
	//-----------------------------------------------------------------------------------
	class EmdMesh : public BaseMesh
	{
		//friend class MeshObject;
		friend class MeshManager;

	public:
		// ----- EmdMesh Methods
		bool LoadMesh_020(ifstream& fin);

		void AddMaterial(Material* mat) { m_Materials.push_back(mat); }
		//size_t GetMaterialsNum() const { return m_Materials.size(); }

	protected:
		// ----- Overwrite Resource
		bool LoadImpl();

		// ----- EmdMesh Methods
		EmdMesh(const String& filename);
		~EmdMesh();
	};
}

#endif
