//-----------------------------------------------------------------------------------
/// SkeletonManager.h ¹Ç÷À¹ÜÀíÆ÷
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _SKELETONMANAGER_H
#define _SKELETONMANAGER_H

#include "ResourceManager.h"
#include "Singleton.h"

namespace Gen
{
	class Skeleton;

	class SkeletonManager : public Singleton<SkeletonManager>, public ResourceManagerBase
	{
		friend class Singleton<SkeletonManager>;
	public:
		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		Skeleton* GetByName(const String& resName);
	protected:
		SkeletonManager();
	};
}

#endif
