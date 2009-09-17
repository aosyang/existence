//-----------------------------------------------------------------------------------
/// FileEnumerator.h �ļ�ö�����������г�һ��·���µ������ļ�
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
	HANDLE	m_FileHandle;			// �ļ����Ҿ��
#elif defined __PLATFORM_LINUX
	DIR*	m_Directory;			// �ļ��о��
#endif	// #if defined __PLATFORM_WIN32
};

#endif
