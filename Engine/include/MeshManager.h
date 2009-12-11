//-----------------------------------------------------------------------------------
/// MeshManager.h ģ�͹�����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MESHMANAGER_H
#define _MESHMANAGER_H

#include "EString.h"
#include "IMesh.h"
#include "EmdMesh.h"
#include "Ms3dMesh.h"
#include "PrimitiveMesh.h"
#include "Singleton.h"

#include <map>

using namespace std;

namespace Gen
{
	typedef map<const String, IMesh*>	MeshResources;

	typedef IMesh*(*MeshLoaderFunc)(const String&);
	typedef map<const String, MeshLoaderFunc>	MeshLoaders;
	
	bool LoadMeshes(const String& resName, const String& filename);

	class MeshManager : public Singleton<MeshManager>
	{
		friend class Singleton<MeshManager>;
	public:

		// ע��һ��ģ�͸�ʽ�Ķ�ȡ����
		void RegisterExtensionLoader(const String& ext, MeshLoaderFunc func);

		// ���ļ���ȡģ��
		bool LoadFromFile(const String& resName, const String& filename);

		// ��������������
		PrimitiveMesh* CreatePrimitiveMesh(const String& name="");

		IMesh* GetByName(const String& resName);
		MeshResources& GetResourceMap() { return m_MeshResources; }

		void UnloadAllResources();
	protected:
		MeshManager();

		MeshResources	m_MeshResources;
		MeshLoaders		m_MeshLoaders;
	};
}

#endif
