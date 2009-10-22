//-----------------------------------------------------------------------------------
/// FileSystem.cpp �ļ�ϵͳʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "FileSystem.h"
#include "FileEnumerator.h"
#include "Debug.h"

#if defined __PLATFORM_LINUX
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>

const int MAX_PATH = PATH_MAX;
#endif	// #if defined __PLATFORM_LINUX


void CorrectSlash(String& path)
{
	size_t pos;
	while((pos = path.Find(INCORRECT_SLASH)) != String::npos)
	{
		path.Replace(pos, 1, CORRECT_SLASH);
	}

	// ˫б�ܸ�Ϊ��б��
	static String double_slash = String(CORRECT_SLASH) + String(CORRECT_SLASH);
	while((pos = path.Find(double_slash)) != String::npos)
	{
		path.Replace(pos, 2, CORRECT_SLASH);
	}
}


FileSystem::FileSystem()
{
#if defined __PLATFORM_WIN32
	char path[MAX_PATH];
	GetCurrentDirectory(sizeof(path), path);
	m_AppPath = path;
#elif defined __PLATFORM_LINUX
	m_AppPath = get_current_dir_name();
#endif	// #if defined __PLATFORM_WIN32
	CorrectSlash(m_AppPath);

	// Ĭ��data·��
	SetRelativeDataPath("data");

	m_IgnoreList.insert(".");
	m_IgnoreList.insert("..");
	m_IgnoreList.insert(".svn");
}

void FileSystem::SetRelativeDataPath(const String& path)
{
	// ע�����ﴫ������path������һ�����·��������õ���·������ȷ
	m_DataPath = path;
	CorrectSlash(m_DataPath);

	char fullPath[MAX_PATH];
#if defined __PLATFORM_WIN32
	char* name;
	GetFullPathName(path.Data(), sizeof(fullPath), fullPath, &name);
#elif defined __PLATFORM_LINUX
	String relPath = m_AppPath + CORRECT_SLASH + path;
	realpath(relPath.Data(), fullPath);
#endif	// #if defined __PLATFORM_WIN32

	m_DataFullPath = fullPath;
	CorrectSlash(m_DataFullPath);
}

const String FileSystem::GetValidResourcePath(const String& path)
{
	String result = path;
	size_t pos = result.Find("$data");
	if (pos!=String::npos)
	{
		result.Replace(pos, 5, m_DataPath);
	}

	CorrectSlash(result);
	return result;
}

void FileSystem::FindFilesInDir(const String& basePath, vector<ResourceFileNameInfo>& foundFileList, bool recursive, const String& subPath)
{
	String fullPath = basePath + CORRECT_SLASH + subPath;
	
	FileEnumerator fileEnum;
	FileInfo info;
	
	// for test only
	//static int depth = 0;
	//AssertFatal(depth<=10, "FileSystem::FindFilesInDir(): invalid recursive function calling depth.");
	
	// ָ���ļ�δ�ҵ�
	if (!fileEnum.EnumerateFirstFileInDir(fullPath.Data(), info)) return;
	
	do
	{
		if (m_IgnoreList.find(info.filename)!=m_IgnoreList.end()) continue;

		// ���ָ���ļ���һ���ļ��У��ݹ����
		if (info.isDirectory)
		{
			// û��Ҫ��ݹ飬������һ���ļ�
			if (!recursive) continue;

			//depth++;
			FindFilesInDir(basePath,
						   foundFileList,
						   recursive, 
						   subPath + info.filename + CORRECT_SLASH);
			//depth--;
		}
		else
		{
			ResourceFileNameInfo resNameInfo;
			resNameInfo.path = basePath;
			resNameInfo.filename = subPath + info.filename;
			resNameInfo.ext = GetExtension(info.filename);
			resNameInfo.ext.ToLowerCase();

			// �⽫��ֹ������������������
			if (resNameInfo.ext==".emt")
				resNameInfo.period = 2;
			else if (resNameInfo.ext==".emd")
				resNameInfo.period = 3;
			else
				resNameInfo.period = 0;

			foundFileList.push_back(resNameInfo);
		}
	} while (fileEnum.EnumerateNextFileInDir(info));
}

const String FileSystem::GetExtension(const String& filename)
{
	// TODO: ·������������չ�����ļ���������չ�������
	size_t pos = filename.FindLastOf(".");
	if (pos!=String::npos)
	{
		String result;
		result = filename.Substr(pos);

		return result;
	}

	return "";
}

