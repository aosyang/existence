//-----------------------------------------------------------------------------------
/// MeshManager.cpp 模型管理器实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "MeshManager.h"
#include "FileSystem.h"
#include "EmdMesh.h"
#include "System.h"
#include "Resource.h"

namespace Gen
{
	MeshManager::MeshManager()
	{

	}

	bool MeshManager::CreateResourceHandles(ResourceFileNameInfo* resName)
	{
		String ext = FileSystem::Instance().GetExtension(resName->filename);
		ext.ToLowerCase();

		String relativePathName = resName->path + CORRECT_SLASH + resName->filename;

		BaseMesh* mesh = NULL;

		// TODO: 使用map
		if (ext==".emd")
		{
			mesh = new EmdMesh(relativePathName);
		}
		else if (ext==".ms3d")
		{
			mesh = new Ms3dMesh(relativePathName);
		}

		if (mesh)
		{
			// 检查资源名称是否已经被占用
			AssertFatal(m_ResourceMap.find(resName->filename)==m_ResourceMap.end(),
						"MeshManager::CreateResourceHandles(): Specified resource name already exists.");

			m_ResourceMap[resName->filename] = mesh;
			mesh->m_ResourceName = resName->filename;
			return true;
		}

		return false;
	}

	PrimitiveMesh* MeshManager::CreatePrimitiveMesh(const String& name)
	{
		String resName;
		PrimitiveMesh* res = new PrimitiveMesh("");

		resName = CheckResName(name);
		m_ResourceMap[resName] = res;
		res->m_ResourceName = resName;

		return res;
	}

	BaseMesh* MeshManager::GetByName(const String& resName)
	{
		return static_cast<BaseMesh*>(GetResourceByName(resName));
	}

}
