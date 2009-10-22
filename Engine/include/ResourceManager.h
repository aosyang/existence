//-----------------------------------------------------------------------------------
/// ResourceManager.h ��Դ������ģ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "Platform.h"
#include "Singleton.h"
#include "Debug.h"
#include "FileSystem.h"

#include <map>
#include "EString.h"

#if defined __PLATFORM_WIN32
#pragma warning(disable: 4996)
#endif	//#if defined __PLATFORM_WIN32

using namespace std;

template <class T>
class ResourceManager : public Singleton< ResourceManager<T> >		// use "> >" instead of ">>", gcc told me so...
{
	friend class Singleton< ResourceManager<T> >;
public:
	typedef map<const String, T*>	ResourceMap;

	// ��������ʵ��
	T* Create(const String& name = "")
	{
		String res_name;

		T* res = new T;

		// ���û��ָ�����ƣ��Զ�����һ��
		if (name == "")
		{
			res_name.Format("#UNNAMED%d", m_sIndex);
			m_sIndex++;

			while (m_ResourceMap.find(res_name)!=m_ResourceMap.end())
			{
				res_name.Format("#UNNAMED%d", m_sIndex);
				m_sIndex++;
			}
		}
		else
		{
			res_name = name;
			CorrectSlash(res_name);

			// ������Դ�Ѵ��ڣ���������
			AssertFatal(m_ResourceMap.find(res_name)==m_ResourceMap.end(), "ResourceManager::Create(): Specified resource name already exists.");

			//while (m_ResourceMap.find(res_name)!=m_ResourceMap.end())
			//{
			//	// ��������Դ�����ظ���������
			//	res_name.Format("%s_%d", name.Data(), n);
			//	n++;
			//}
		}
		m_ResourceMap[res_name] = res;
		res->m_Name = &(m_ResourceMap.find(res_name)->first);

		return res;
	}

	T* LoadResource(const String& resname, const String& filename)
	{
		if (m_ResourceMap.find(resname)!=m_ResourceMap.end())
			return NULL;

		T* res = (*m_sLoadFunc)(filename);
		if (res)
		{
			m_ResourceMap[resname] = res;
			res->m_Name = &(m_ResourceMap.find(resname)->first);
		}

		return res;
	}

	//void Destroy(T* res)
	//{
	//	m_ResourceMap.erase(res);
	//	//if (m_ResourceMap.find(res)!=m_ResourceMap.end())
	//	//	delete res;
	//}

	T* GetByName(const String& name)
	{
		String sName(name);
		CorrectSlash(sName);

		typename ResourceMap::iterator iter;
		if ((iter = m_ResourceMap.find(sName))!=m_ResourceMap.end())
			return iter->second;

		return NULL;
	}

	// ж�����ж���
	void UnloadAllResources()
	{
		typename ResourceManager<T>::ResourceMap::iterator iter = m_ResourceMap.begin();
		for (;iter!=m_ResourceMap.end(); iter++)
		{
			SAFE_DELETE(iter->second);
		}
	}

	void DumpToLog()
	{
		Log.MsgLn("----------- Begin dumping ResourceManager");

		typename ResourceMap::iterator iter;
		for (iter=m_ResourceMap.begin();
			 iter!=m_ResourceMap.end();
			 iter++)
		{
			Log.MsgLn(iter->first);
		}

		Log.MsgLn("----------- End dumping ResourceManager");
	}
	
	ResourceMap& GetResourceMap() { return m_ResourceMap; }

	static unsigned int m_sIndex;

	typedef T*(*LoadFunc)(const String& filename);
	static LoadFunc m_sLoadFunc;

private:
	ResourceManager() {}
	~ResourceManager() { UnloadAllResources(); }

protected:
	ResourceMap			m_ResourceMap;
};

// �궨�壬��������һ���ཫʹ����Դ������
#if defined __PLATFORM_WIN32
#define DECLARE_RESOURCEMANAGER(className) ResourceManager<className>
#elif defined __PLATFORM_LINUX
#define DECLARE_RESOURCEMANAGER(className) template class ResourceManager<className>
#endif

#endif
