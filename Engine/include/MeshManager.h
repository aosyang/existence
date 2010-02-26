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
#include "EmdMesh.h"
#include "Ms3dMesh.h"
#include "PrimitiveMesh.h"
#include "Singleton.h"
#include "ResourceManager.h"

#include <map>

using namespace std;

namespace Gen
{
	class MeshManager : public Singleton<MeshManager>, public ResourceManagerBase
	{
		friend class Singleton<MeshManager>;
	public:

		// ע��һ��ģ�͸�ʽ�Ķ�ȡ����
		//void RegisterExtensionLoader(const String& ext, MeshLoaderFunc func);

		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		// ��������������
		PrimitiveMesh* CreatePrimitiveMesh(const String& name="");

		// ��ȡָ�����Ƶ�Mesh
		BaseMesh* GetByName(const String& resName);
	protected:
		MeshManager();
	};
}

#endif
