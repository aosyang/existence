//-----------------------------------------------------------------------------------
/// Mesh.h 网格类，保存顶点信息，可以从文件加载
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef __MESH__
#define __MESH__

#include "Vector3f.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Color4f.h"
#include "Platform.h"
#include "MeshElement.h"
#include "MeshObject.h"
#include "Debug.h"
#include "BspTree.h"
#include "Material.h"
#include "OBB.h"

#include <vector>
#include "EString.h"
#include <fstream>

using namespace std;

class BspTree;
class MeshElement;
class Mesh;

ResourceManager<Mesh>;

enum EMDL_LUMP
{
	EMDL_LUMP_TEXTURES = 100,
	EMDL_LUMP_MESH_ELEMENTS,
};


// TODO: Mesh应当属于一种资源，因为可以被重复使用

//-----------------------------------------------------------------------------------
/// \brief
/// 模型类，存储渲染所需基本信息
/// 
/// 模型可以从文件读取或者直接创建一个基本几何体
/// 
/// \remarks
/// 储存的顶点及索引信息可以被渲染器调用进行渲染，不同渲染对象可以共享同一个模型资源
//-----------------------------------------------------------------------------------
class Mesh
{
	//friend class Shape;			// TODO: Remove this
	friend class MeshObject;
	friend class ResourceManager<Mesh>;

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
	static Mesh* LoadMeshFromFile(const String& filename);

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 构造一个指定边长的立方体
	/// 
	/// \param side
	/// 立方体的边长
	//-----------------------------------------------------------------------------------
	void CreateBox(float side);

	void CreatePositiveYPlane(float side);

	void AddMeshElement(MeshElement* elem);

	void AddMaterial(Material* mat) { m_Materials.push_back(mat); }
	size_t GetMaterialsNum() const { return m_Materials.size(); }
	Material* GetMaterial(unsigned int index)
	{
		return (index<m_Materials.size()) ? m_Materials[index] : NULL;
	}
	void SetMaterial(Material* mat, unsigned int index)
	{
		if (index<m_Materials.size())
		{
			m_Materials[index] = mat;
		}
	}

	// 清除所有子元素
	void ClearMeshElements();

	size_t GetElementsNum() const { return m_MeshElements.size(); }
	MeshElement* GetElement(unsigned int index)
	{
		return (index<m_MeshElements.size()) ? m_MeshElements[index] : NULL;
	}

	float GetBoundingRadius() const { return m_BoundingRadius; }
	OBB& GetOBB() { return m_OBB; }

	friend bool LoadMesh_NoVersion(Mesh* mesh, ifstream& fin);

private:
	Mesh();
	~Mesh();

	const String*			m_Name;
	float					m_BoundingRadius;
	vector<MeshElement*>	m_MeshElements;
	vector<Material*>		m_Materials;

	OBB						m_OBB;
};

bool LoadMesh_NoVersion(Mesh* mesh, ifstream& fin);
bool LoadMesh_010(Mesh* mesh, ifstream& fin);

#endif
