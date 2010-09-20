//-----------------------------------------------------------------------------------
/// FileSystem.cpp 文件系统实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "FileSystem.h"
#include "FileEnumerator.h"
#include "Debug.h"

#include <vector>


#if defined __PLATFORM_WIN32
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#elif defined __PLATFORM_LINUX
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#endif	// #if defined __PLATFORM_LINUX

namespace Gen
{
#if defined __PLATFORM_LINUX
	const int MAX_PATH = PATH_MAX;
#endif	// #if defined __PLATFORM_LINUX


	void CorrectSlash(String& path)
	{
		size_t pos;
		while((pos = path.Find(INCORRECT_SLASH)) != String::npos)
		{
			path.Replace(pos, 1, CORRECT_SLASH);
		}

		// 双斜杠改为单斜杠
		static String double_slash = String(CORRECT_SLASH) + String(CORRECT_SLASH);
		while((pos = path.Find(double_slash)) != String::npos)
		{
			path.Replace(pos, 2, CORRECT_SLASH);
		}
	}

	String CanonicalizePath(const String& path)
	{
#if defined __PLATFORM_WIN32
		char result[MAX_PATH];
		PathCanonicalize(result, path.Data());
		return result;

#elif defined __PLATFORM_LINUX
		String s;
		std::vector<String> tab;

		size_t len = path.Size();
		size_t i;

		bool ignore = false;

		for (i=0; i<len; i++)
		{
			switch (path[i])
			{
			case CORRECT_SLASH_CHAR:
				if (!ignore)
				{
					if (s.Size())
					{
						tab.push_back(s);
						s = "";
					}
					tab.push_back(CORRECT_SLASH);
				}
				else
				{
					ignore = false;
				}
				break;
			case '.':
				if (!s.Size())
				{
					if (++i<len && path[i]=='.')
					{
						if (++i<len && path[i]=='.')
						{
							s += "...";
							break;
						}
						i--;

						// 退一级路径
						if (tab.size()>=2)
						{
							tab.pop_back();
							tab.pop_back();
							ignore = true;
						}
						//else
						//	tab.push_back("..");
						break;
					}
					i--;

					if (tab.size())
					{
						tab.pop_back();
						//tab.push_back(".");
					}
					else
						ignore = true;
					break;
				}
				// 继续当作常规字符处理
			default:
				s += path[i];
				break;
			}
		}

		if (s.Size())
		{
			tab.push_back(s);
			s = "";
		}

		if (ignore && tab.size())
			tab.pop_back();

		if (!tab.size())
			tab.push_back(CORRECT_SLASH);

		for (std::vector<String>::iterator iter=tab.begin();
			iter!=tab.end();
			iter++)
		{
			s += *iter;
		}

		return s;
#endif
	}

	String GetPathName(const String& pathFilename)
	{
		size_t pos = pathFilename.FindLastOf(CORRECT_SLASH);

		if (pos == String::npos) return "";

		return pathFilename.Substr(0, pos + 1);
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

		// 默认data路径
		SetRelativeDataPath("data");

		m_IgnoreList.insert(".");
		m_IgnoreList.insert("..");
		m_IgnoreList.insert(".svn");
	}

	void FileSystem::SetRelativeDataPath(const String& path)
	{
		// 注：这里传进来的path必须是一个相对路径，否则得到的路径不正确
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

	void FileSystem::FindFilesInDir(const String& basePath, std::vector<ResourceFileNameInfo>& foundFileList, bool recursive, const String& subPath)
	{
		String fullPath = basePath + CORRECT_SLASH + subPath;

		FileEnumerator fileEnum;
		FileInfo info;

		// for test only
		//static int depth = 0;
		//AssertFatal(depth<=10, "FileSystem::FindFilesInDir(): invalid recursive function calling depth.");

		// 指定文件未找到
		if (!fileEnum.EnumerateFirstFileInDir(fullPath.Data(), info)) return;

		do
		{
			if (m_IgnoreList.find(info.filename)!=m_IgnoreList.end()) continue;

			// 如果指定文件是一个文件夹，递归检索
			if (info.isDirectory)
			{
				// 没有要求递归，继续下一个文件
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

				foundFileList.push_back(resNameInfo);
			}
		} while (fileEnum.EnumerateNextFileInDir(info));
	}

	const String FileSystem::GetExtension(const String& filename) const
	{
		// TODO: 路径名里面有扩展名且文件名不带扩展名的情况
		size_t pos = filename.FindLastOf(".");
		if (pos!=String::npos)
		{
			String result;
			result = filename.Substr(pos);

			return result;
		}

		return "";
	}
}
