//-----------------------------------------------------------------------------------
/// FileEnumerator.cpp 文件枚举器实现类
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "FileEnumerator.h"

namespace Gen
{
	FileEnumerator::FileEnumerator()
#if defined __PLATFORM_WIN32
		: m_FileHandle(NULL)
#elif defined __PLATFORM_LINUX
		: m_Directory(NULL)
#endif
	{
	}

	FileEnumerator::~FileEnumerator()
	{
	}

	bool FileEnumerator::EnumerateFirstFileInDir(const String& dirname, FileInfo& info)
	{
#if defined __PLATFORM_WIN32
		String filename = dirname + CORRECT_SLASH + "*";
		WIN32_FIND_DATA findFileData;
		m_FileHandle = FindFirstFile(filename.Data(), &findFileData);

		// 错误的句柄，文件可能没有找到
		if (m_FileHandle==INVALID_HANDLE_VALUE)
		{
			return false;
		}

		info.filename = findFileData.cFileName;
		info.isDirectory = ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0);

		return true;
#elif defined __PLATFORM_LINUX
		// TODO: __PLATFORM_LINUX下先修正路径的斜杠

		m_Directory = opendir(dirname.Data());

		// 无法打开文件夹
		if (!m_Directory) return false;

		struct dirent* ent = readdir(m_Directory);

		// 没有找到文件
		if (!ent) return false;

		info.filename = ent->d_name;
		info.isDirectory = (ent->d_reclen==16);

		return true;
#endif	// #if defined __PLATFORM_WIN32
	}

	bool FileEnumerator::EnumerateNextFileInDir(FileInfo& info)
	{
#if defined __PLATFORM_WIN32
		// 没有文件句柄
		if (!m_FileHandle) return false;

		WIN32_FIND_DATA findFileData;

		// 没有更多文件了
		if (!FindNextFile(m_FileHandle, &findFileData))
		{
			// 结束本次查找
			m_FileHandle = NULL;
			return false;
		}

		info.filename = findFileData.cFileName;
		info.isDirectory = ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0);

		return true;
#elif defined __PLATFORM_LINUX
		if (!m_Directory) return false;

		struct dirent* ent = readdir(m_Directory);

		// 没有找到文件
		if (!ent)
		{
			m_Directory = NULL;
			return false;
		}

		info.filename = ent->d_name;
		info.isDirectory = (ent->d_reclen==16);

		return true;

#endif	// #if defined __PLATFORM_WIN32
	}
}
