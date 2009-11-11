//-----------------------------------------------------------------------------------
/// FileSystem.h 文件系统，处理资源路径等问题
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "EString.h"
#include "Platform.h"
#include "Singleton.h"

#include <vector>
#include <set>

using namespace std;

namespace Gen
{
#if defined __PLATFORM_WIN32
#define CORRECT_SLASH "\\"
#define INCORRECT_SLASH "/"
#elif defined __PLATFORM_LINUX
#define CORRECT_SLASH "/"
#define INCORRECT_SLASH "\\"
#endif	// #if defined __PLATFORM_WIN32

	// 修正斜杠与反斜杠，使之符合对应操作系统的习惯
	void CorrectSlash(String& path);


	struct ResourceFileNameInfo
	{
		String filename;	///< 文件名称(基于path的相对名称)
		String ext;			///< 文件扩展名
		String path;		///< 路径名称(不必须是完整路径)
		int period;			///< 载入优先级，越小越早
	};

	// 文件系统
	// 功能：负责将给定路径名称转换为有效完整路径
	//       获取程序当前运行路径
	class FileSystem : public Singleton<FileSystem>
	{
		friend class Singleton<FileSystem>;
	public:

		// 指定数据文件所在的相对路径
		void SetRelativeDataPath(const String& path);

		const String& GetAppPath() const { return m_AppPath; }
		const String& GetDataPath() const { return m_DataPath; }
		const String& GetDataFullPath() const { return m_DataFullPath; }

		// 获取资源路径，将"$data"替换成文件系统指定的路径
		const String GetValidResourcePath(const String& path);

		// 在指定路径下搜索文件
		// 注：路径必须是一个有效路径(即操作系统可识别的路径)
		void FindFilesInDir(const String& basePath, vector<ResourceFileNameInfo>& foundFileList, bool recursive = false, const String& subPath = "");

		const String GetExtension(const String& filename);

	protected:
		FileSystem();

	protected:
		String	m_AppPath;
		String	m_DataPath;
		String	m_DataFullPath;

		set<String>		m_IgnoreList;
	};
}

#endif
