//-----------------------------------------------------------------------------------
/// ResourceManager.cpp 资源管理器实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "ResourceManager.h"
#include "Resource.h"
#include "Debug.h"
#include "FileSystem.h"

namespace Gen
{
	ResourceManagerBase::ResourceManagerBase()
		: m_AutoNameIndex(0)
	{
	}

	Resource* ResourceManagerBase::GetResourceByName(const String& resName)
	{
		String correctedResName = resName;
		CorrectSlash(correctedResName);

		// 没有找到资源则返回NULL
		if (m_ResourceMap.find(correctedResName)==m_ResourceMap.end())
			return NULL;

		return m_ResourceMap[correctedResName];
	}

	void ResourceManagerBase::UnloadAllResources()
	{
		for (ResourceMap::iterator iter=m_ResourceMap.begin();
			iter!=m_ResourceMap.end();
			iter++)
		{
			if (iter->second->IsLoaded())
				iter->second->Unload();
			delete iter->second;
		}
	}

	void ResourceManagerBase::DumpToLog() const
	{
		Log.MsgLn("----------- Begin dumping ResourceManager");

		ResourceMap::const_iterator iter;
		for (iter=m_ResourceMap.begin();
			iter!=m_ResourceMap.end();
			iter++)
		{
			String msg;
			msg.Format(" res name: %s, file name: %s, is loaded: %s",
					   iter->first.Data(),
					   iter->second->GetFilename().Data(),
					   iter->second->IsLoaded() ? "true" : "false");
			Log.MsgLn(msg);
		}

		Log.MsgLn("----------- End dumping ResourceManager");
	}

	String ResourceManagerBase::GenerateAutoName()
	{
		String autoName;
		autoName.Format("#UNNAMED%d", m_AutoNameIndex);
		m_AutoNameIndex++;

		while (m_ResourceMap.find(autoName)!=m_ResourceMap.end())
		{
			autoName.Format("#UNNAMED%d", m_AutoNameIndex);
			m_AutoNameIndex++;
		}

		return autoName;
	}

	String ResourceManagerBase::CheckResName(const String& resName)
	{
		// 如果没有指定名称，自动生成一个
		if (resName == "")
		{
			return GenerateAutoName();
		}

		// 修正斜杠
		String name = resName;
		CorrectSlash(name);

		// 创建资源已存在，致命错误
		AssertFatal(m_ResourceMap.find(name)==m_ResourceMap.end(),
					"ResourceManagerBase::CheckResName(): Specified resource name already exists.");

		return name;
	}


}
