//-----------------------------------------------------------------------------------
/// SkeletonManager.cpp 骨骼管理器实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "SkeletonManager.h"
#include "Skeleton.h"
#include "FileSystem.h"
#include "Debug.h"

namespace Gen
{
	SkeletonManager::SkeletonManager()
	{
	}

	bool SkeletonManager::CreateResourceHandles(ResourceFileNameInfo* resName)
	{
		// 骨骼名称使用原始名去掉'.ebh'扩展名的方式作为资源名
		size_t pos = resName->filename.FindLastOf(".");
		String newName = resName->filename.Substr(0, pos);
		String relativePathName = resName->path + CORRECT_SLASH + resName->filename;

		Skeleton* skeleton = new Skeleton(relativePathName);
		AssertFatal(m_ResourceMap.find(newName)==m_ResourceMap.end(),
					"SkeletonManager::CreateResourceHandles(): Specified resource name already exists.");
		m_ResourceMap[newName] = skeleton;
		skeleton->m_ResourceName = resName->filename;

		return true;
	}

	Skeleton* SkeletonManager::GetByName(const String& resName)
	{
		return static_cast<Skeleton*>(GetResourceByName(resName));
	}

}
