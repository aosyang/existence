//-----------------------------------------------------------------------------------
/// ResourceManager.h 资源管理器模板
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "Singleton.h"

#include <map>
#include "EString.h"

#pragma warning(disable: 4996)

using namespace std;

template <class T>
class ResourceManager : public Singleton<ResourceManager<T>>
{
	friend class Singleton<ResourceManager<T>>;
public:
	// 创建对象实例
	T* Create(const String& name = "")
	{
		char buf[32] = { '\0' };

		T* res = new T;

		if (name == "")
		{
			sprintf(buf, "%d", m_sIndex);
			m_sIndex++;

			while (m_ResourceMap.find(buf)!=m_ResourceMap.end())
			{
				sprintf(buf, "%d", m_sIndex);
				m_sIndex++;
			}
			m_ResourceMap[buf] = res;
			res->m_Name = &(m_ResourceMap.find(buf)->first);
		}
		else
		{
			sprintf(buf, "%s", name.Data());
			unsigned int n = 1;
			while (m_ResourceMap.find(buf)!=m_ResourceMap.end())
			{
				sprintf(buf, "%s_%d", name.Data(), n);
				n++;
			}
			m_ResourceMap[buf] = res;
			res->m_Name = &(m_ResourceMap.find(buf)->first);
		}

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
		ResourceMap::iterator iter;
		if ((iter = m_ResourceMap.find(name))!=m_ResourceMap.end())
			return iter->second;

		return NULL;
	}

	// 卸载所有对象
	void UnloadAllResources()
	{
		ResourceManager<T>::ResourceMap::iterator iter = m_ResourceMap.begin();
		for (;iter!=m_ResourceMap.end(); iter++)
		{
			SAFE_DELETE(iter->second);
		}
	}

	typedef map<const String, T*>	ResourceMap;
	
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

#endif
