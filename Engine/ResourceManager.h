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
#include <string>

#pragma warning(disable: 4996)

using namespace std;

template <class T>
class ResourceManager : public Singleton<ResourceManager<T>>
{
	friend class Singleton<ResourceManager<T>>;
public:
	// 创建对象实例
	T* Create(const string& name = "")
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
		}
		else
		{
			sprintf(buf, "%s", name.data());
			unsigned int n = 1;
			while (m_ResourceMap.find(buf)!=m_ResourceMap.end())
			{
				sprintf(buf, "%s_%d", name.data(), n);
				n++;
			}
			m_ResourceMap[buf] = res;
		}

		return res;
	}

	//void Destroy(T* res)
	//{
	//	m_ResourceMap.erase(res);
	//	//if (m_ResourceMap.find(res)!=m_ResourceMap.end())
	//	//	delete res;
	//}

	T* GetByName(const string& name)
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

	static unsigned int m_sIndex;

private:
	ResourceManager() {}
	~ResourceManager() { UnloadAllResources(); }

protected:
	typedef map<const string, T*>	ResourceMap;
	ResourceMap			m_ResourceMap;
};

#endif
