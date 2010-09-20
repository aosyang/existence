//-----------------------------------------------------------------------------------
/// ResourceManager.h 资源管理器，统一管理各类资源
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "Platform.h"
#include "Singleton.h"

#include <map>
#include "EString.h"

#if defined __PLATFORM_WIN32
#pragma warning(disable: 4996)
#endif	//#if defined __PLATFORM_WIN32



namespace Gen
{
	struct ResourceFileNameInfo;
	class Resource;
	
	typedef std::map<const String, Resource*>	ResourceMap;

	// 资源管理器基类
	class ResourceManagerBase
	{
	public:
		ResourceManagerBase();
		virtual ~ResourceManagerBase() {}

		// 从文件创建资源对象句柄
		virtual bool CreateResourceHandles(ResourceFileNameInfo* resName) = 0;

		// 通过名称获取资源
		Resource* GetResourceByName(const String& resName);

		// 卸载该管理器的全部资源
		void UnloadAllResources();

		// 测试功能：将所有资源及其状态导出到日志
		void DumpToLog() const;

		ResourceMap& GetResourceMap() { return m_ResourceMap; }
	protected:
		// 生成自动名称
		String GenerateAutoName();

		// 检查用户指定的资源名称，如果发生重复则报错，不提供名字则自动生成
		String CheckResName(const String& resName);

	protected:
		ResourceMap			m_ResourceMap;			///< 资源容器

		int					m_AutoNameIndex;		///< 自动命名索引
	};
}

#endif
