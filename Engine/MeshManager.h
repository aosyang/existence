//-----------------------------------------------------------------------------------
/// MeshManager.h 网格模型管理器
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _MESHMANAGER_H
#define _MESHMANAGER_H

#include "Singleton.h"
#include "Mesh.h"
#include "Platform.h"

#include <map>
#include <string>

using namespace std;

typedef map<const string, Mesh*>	MeshMap;


class MeshManager : public Singleton<MeshManager>
{
	friend class Singleton<MeshManager>;
public:
	~MeshManager();
	Mesh* LoadMesh(const string& mesh_name, const string& filename);

	void UnloadAllMeshes();

	Mesh* GetMesh(const string& mesh_name);
	MeshMap& GetMeshMap() { return m_MeshMap; }

private:
	MeshManager();

	MeshMap		m_MeshMap;
};

#endif
