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

	//DECLARE_RESOURCEMANAGER(EmdMesh);

	enum EmdlLump
	{
		EMDL_LUMP_TEXTURES = 100,
		EMDL_LUMP_MESH_ELEMENTS,
	};


	// TODO: EmdMeshӦ������һ����Դ����Ϊ���Ա��ظ�ʹ��

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
		friend class MeshObject;
		//friend class ResourceManager<EmdMesh>;
		friend class MeshManager;

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
