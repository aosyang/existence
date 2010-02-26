//-----------------------------------------------------------------------------------
/// EmdMesh.h Emd网格类，保存顶点信息，可以从emd文件加载
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
	/// 模型类，存储渲染所需基本信息
	/// 
	/// 模型可以从文件读取或者直接创建一个基本几何体
	/// 
	/// \remarks
	/// 储存的顶点及索引信息可以被渲染器调用进行渲染，不同渲染对象可以共享同一个模型资源
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
