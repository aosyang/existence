//-----------------------------------------------------------------------------------
/// SkeletonManager.cpp ����������ʵ��
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
		// ��������ʹ��ԭʼ��ȥ��'.ebh'��չ���ķ�ʽ��Ϊ��Դ��
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
