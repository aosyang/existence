//-----------------------------------------------------------------------------------
/// Mesh.h �����࣬���涥����Ϣ�����Դ��ļ�����
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


// TODO: MeshӦ������һ����Դ����Ϊ���Ա��ظ�ʹ��

//-----------------------------------------------------------------------------------
/// \brief
/// ģ���࣬�洢��Ⱦ���������Ϣ
/// 
/// ģ�Ϳ��Դ��ļ���ȡ����ֱ�Ӵ���һ������������
/// 
/// \remarks
/// ����Ķ��㼰������Ϣ���Ա���Ⱦ�����ý�����Ⱦ����ͬ��Ⱦ������Թ���ͬһ��ģ����Դ
//-----------------------------------------------------------------------------------
class Mesh
{
	//friend class Shape;			// TODO: Remove this
	friend class MeshObject;
	friend class ResourceManager<Mesh>;

public:

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ���ļ�����һ��ģ��
	/// 
	/// \param filename
	/// ��Դ�ļ���
	/// 
	/// \returns
	/// �ɹ���ȡ����true
	/// 
	/// ��ȡһ��ģ�ͣ����涥����Ϣ
	//-----------------------------------------------------------------------------------
	static Mesh* LoadMeshFromFile(const String& filename);

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ����һ��ָ���߳���������
	/// 
	/// \param side
	/// ������ı߳�
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

	// ���������Ԫ��
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
