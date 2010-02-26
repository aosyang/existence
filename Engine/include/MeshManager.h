//-----------------------------------------------------------------------------------
/// MeshManager.h 模型管理器
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

		// 注册一种模型格式的读取函数
		//void RegisterExtensionLoader(const String& ext, MeshLoaderFunc func);

		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		// 创建基本几何体
		PrimitiveMesh* CreatePrimitiveMesh(const String& name="");

		// 获取指定名称的Mesh
		BaseMesh* GetByName(const String& resName);
	protected:
		MeshManager();
	};
}

#endif
