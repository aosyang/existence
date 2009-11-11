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

	//DECLARE_RESOURCEMANAGER(EmdMesh);

	enum EmdlLump
	{
		EMDL_LUMP_TEXTURES = 100,
		EMDL_LUMP_MESH_ELEMENTS,
	};


	// TODO: EmdMesh应当属于一种资源，因为可以被重复使用

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
		friend class MeshObject;
		//friend class ResourceManager<EmdMesh>;
		friend class MeshManager;

	public:

		//-----------------------------------------------------------------------------------
		/// \brief
		/// 从文件载入一个模型
		/// 
		/// \param filename
		/// 资源文件名
		/// 
		/// \returns
		/// 成功读取返回true
		/// 
		/// 读取一个模型，保存顶点信息
		//-----------------------------------------------------------------------------------
		static IMesh* ManagerLoad(const String& filename);

		bool LoadFromFile(const String& filename);

		bool LoadMesh_NoVersion(ifstream& fin);
		bool LoadMesh_010(ifstream& fin);

		bool LoadMeshElement_NoVersion(MeshElement* elem, ifstream& fin, float& boundingRadius);
		bool LoadMeshElement_010(MeshElement* elem, ifstream& fin);

		void AddMaterial(Material* mat) { m_Materials.push_back(mat); }
		//size_t GetMaterialsNum() const { return m_Materials.size(); }
		//Material* GetMaterial(unsigned int index)
		//{
		//	return (index<m_Materials.size()) ? m_Materials[index] : NULL;
		//}
		//void SetMaterial(Material* mat, unsigned int index)
		//{
		//	if (index<m_Materials.size())
		//	{
		//		m_Materials[index] = mat;
		//	}
		//}

	private:
		EmdMesh();
		~EmdMesh();

		String					m_Filename;			// for test only
	};
}

#endif
