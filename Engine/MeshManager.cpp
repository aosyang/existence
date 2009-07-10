//-----------------------------------------------------------------------------------
/// MeshManager.cpp 网格模型管理器
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "MeshManager.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

Mesh* MeshManager::LoadMesh(const string& mesh_name, const string& filename)
{
	if (m_MeshMap.find(mesh_name)!=m_MeshMap.end())
		return NULL;

	Mesh* mesh = new Mesh();
	if (!mesh->LoadFromFile(filename))
	{
		delete mesh;

		return NULL;
	}

	return (m_MeshMap[mesh_name] = mesh);
}

void MeshManager::UnloadAllMeshes()
{
	MeshMap::iterator iter;
	for (iter=m_MeshMap.begin(); iter!=m_MeshMap.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

Mesh* MeshManager::GetMesh(const string& mesh_name)
{
	if (m_MeshMap.find(mesh_name)==m_MeshMap.end())
		return NULL;

	return m_MeshMap[mesh_name];
}
