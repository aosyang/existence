//-----------------------------------------------------------------------------------
/// FileSystem.h �ļ�ϵͳ��������Դ·��������
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

	// ����б���뷴б�ܣ�ʹ֮���϶�Ӧ����ϵͳ��ϰ��
	void CorrectSlash(String& path);


	struct ResourceFileNameInfo
	{
		String filename;	///< �ļ�����(����path���������)
		String ext;			///< �ļ���չ��
		String path;		///< ·������(������������·��)
		int period;			///< �������ȼ���ԽСԽ��
	};

	// �ļ�ϵͳ
	// ���ܣ����𽫸���·������ת��Ϊ��Ч����·��
	//       ��ȡ����ǰ����·��
	class FileSystem : public Singleton<FileSystem>
	{
		friend class Singleton<FileSystem>;
	public:

		// ָ�������ļ����ڵ����·��
		void SetRelativeDataPath(const String& path);

		const String& GetAppPath() const { return m_AppPath; }
		const String& GetDataPath() const { return m_DataPath; }
		const String& GetDataFullPath() const { return m_DataFullPath; }

		// ��ȡ��Դ·������"$data"�滻���ļ�ϵͳָ����·��
		const String GetValidResourcePath(const String& path);

		// ��ָ��·���������ļ�
		// ע��·��������һ����Ч·��(������ϵͳ��ʶ���·��)
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
