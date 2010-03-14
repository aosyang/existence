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

	/// @brief
	/// 资源类
	/// @par
	///		记录一个用于引擎的资源与磁盘上文件的关联
	///		可以根据需求从文件加载或者从内存释放资源
	class Resource
	{
	public:

		/// 构造函数
		Resource();
		
		/// @brief
		///	构造函数
		/// @param filename 资源使用的文件名
		Resource(const String& filename);
		virtual ~Resource();

		/// @brief
		/// 载入资源
		/// @par
		///		从文件将资源加载到内存
		/// @remarks
		///		这个方法由用户调用
		bool Load();

		/// @brief
		/// 卸载资源
		/// @par
		///		将内存的资源释放
		/// @remarks
		///		这个方法由用户调用
		void Unload();

		/// @brief
		/// 触发资源，对资源进行不重复的加载操作
		/// @par
		///		引用资源前调用这个方法确保资源已加载
		/// @returns
		///		返回true表示触发成功，返回false表示资源不需要触发
		virtual bool Trigger();

		/// 判断资源是否已经载入
		bool IsLoaded() const { return m_IsResourceLoaded; }

		// TODO: 资源类型
		//ResourceType GetType() const;

		/// 获取资源名
		String GetResourceName() const { return m_ResourceName; }

		/// 获取资源文件名
		String GetFilename() const { return m_Filename; }
	protected:

		/// @brief
		///	资源载入实现方法
		/// @returns
		///		如果载入成功，返回true；否则返回false
		/// @remarks
		///		进行实际资源载入操作，派生类覆写此方法以实现自己的载入操作
		virtual bool LoadImpl() { return true; }

		/// @brief
		/// 资源卸载实现方法
		/// @remarks
		///		进行实际资源卸载操作，派生类覆写此方法以实现自己的卸载操作
		virtual void UnloadImpl() {}

	protected:
		ResourceManagerBase*	m_Manager;			///< 创建这个资源的管理器

		String					m_ResourceName;		///< 资源名称
		String					m_Filename;			///< 资源文件名

		bool					m_IsResourceLoaded;	///< 资源是否已经加载
	};
}

#endif
