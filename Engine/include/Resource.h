//-----------------------------------------------------------------------------------
/// Resource.h 资源类，提供资源的公共方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "EString.h"

namespace Gen
{
	class ResourceManagerBase;

	class Resource
	{
	public:
		// 构造函数

		Resource();
		//   filename 资源使用的文件名
		Resource(const String& filename);
		virtual ~Resource();

		// 资源载入操作
		bool Load();

		// 卸载资源
		void Unload();

		// 触发资源，引用资源前调用这个方法确保资源已加载
		// 返回true表示触发成功
		// 返回false表示不需要触发
		virtual bool Trigger();

		// 判断资源是否已经载入
		bool IsLoaded() const { return m_IsResourceLoaded; }

		// TODO: 资源类型
		//ResourceType GetType() const;

		// 获取资源名
		String GetResourceName() const { return m_ResourceName; }

		// 获取资源文件名
		String GetFilename() const { return m_Filename; }
	protected:

		// 派生类重载此函数以实现实际的载入/卸载操作
		virtual bool LoadImpl() { return true; }
		virtual void UnloadImpl() {}

	protected:
		ResourceManagerBase*	m_Manager;			///< 创建这个资源的管理器

		String					m_ResourceName;		///< 资源名称
		String					m_Filename;			///< 资源文件名

		bool					m_IsResourceLoaded;
	};
}

#endif
