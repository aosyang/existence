//-----------------------------------------------------------------------------------
/// ResourceManager.h ��Դ��������ͳһ���������Դ
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

	// ��Դ����������
	class ResourceManagerBase
	{
	public:
		ResourceManagerBase();
		virtual ~ResourceManagerBase() {}

		// ���ļ�������Դ������
		virtual bool CreateResourceHandles(ResourceFileNameInfo* resName) = 0;

		// ͨ�����ƻ�ȡ��Դ
		Resource* GetResourceByName(const String& resName);

		// ж�ظù�������ȫ����Դ
		void UnloadAllResources();

		// ���Թ��ܣ���������Դ����״̬��������־
		void DumpToLog() const;

		ResourceMap& GetResourceMap() { return m_ResourceMap; }
	protected:
		// �����Զ�����
		String GenerateAutoName();

		// ����û�ָ������Դ���ƣ���������ظ��򱨴����ṩ�������Զ�����
		String CheckResName(const String& resName);

	protected:
		ResourceMap			m_ResourceMap;			///< ��Դ����

		int					m_AutoNameIndex;		///< �Զ���������
	};
}

#endif
