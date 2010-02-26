//-----------------------------------------------------------------------------------
/// Resource.cpp 资源类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "Resource.h"
#include "Debug.h"

namespace Gen
{
	Resource::Resource()
		: m_IsResourceLoaded(false)
	{
	}

	Resource::Resource(const String& filename)
		: m_Filename(filename),
		  m_IsResourceLoaded(false)
	{
	}

	Resource::~Resource()
	{
		// 不要在析构函数中调用虚函数
		// 由于派生对象已经被销毁，虚函数调用将仅仅调用自己的函数！

		//if (m_IsResourceLoaded)
		//	Unload();
		//m_IsResourceLoaded = false;
	}

	bool Resource::Load()
	{
		AssertFatal(!m_IsResourceLoaded, "Resource::Load(): Resource has been loaded already!");

		bool result;
		if (result=LoadImpl())
		{
			m_IsResourceLoaded = true;
		}

		return result;
	}

	void Resource::Unload()
	{
		AssertFatal(m_IsResourceLoaded, "Resource::Unload(): Resource has been unloaded already!");

		UnloadImpl();
		m_IsResourceLoaded = false;
	}

	bool Resource::Trigger()
	{
		if (!m_IsResourceLoaded)
		{
			Load();
			return true;
		}

		return false;
	}
}
