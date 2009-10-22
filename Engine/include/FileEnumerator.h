//-----------------------------------------------------------------------------------
/// FileEnumerator.h 文件枚举器，可以列出一个路径下的所有文件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _FILEENUMERATOR_H
#define _FILEENUMERATOR_H

#include "Platform.h"

#if defined __PLATFORM_WIN32
#include <windows.h>
#elif defined __PLATFORM_LINUX
#include <dirent.h>
#include <stdlib.h>
#endif	// #if defined __PLATFORM_WIN32

#include "EString.h"
#include "FileSystem.h"

struct FileInfo
{
	String	filename;
	bool	isDirectory;
};

class FileEnumerator
{
public:
	FileEnumerator();
	~FileEnumerator();

	bool EnumerateFirstFileInDir(const String& dirname, FileInfo& info);
	bool EnumerateNextFileInDir(FileInfo& info);

protected:
#if defined __PLATFORM_WIN32
	HANDLE	m_FileHandle;			// 文件查找句柄
#elif defined __PLATFORM_LINUX
	DIR*	m_Directory;			// 文件夹句柄
#endif	// #if defined __PLATFORM_WIN32
};

#endif
