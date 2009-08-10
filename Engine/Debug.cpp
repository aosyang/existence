//-----------------------------------------------------------------------------------
/// Debug.cpp �����ø�������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Debug.h"

#if defined __PLATFORM_WIN32
#include <crtdbg.h>
#include <fstream>

using namespace std;
#endif // #if defined __PLATFORM_WIN32

namespace Debug
{
	void EnableBreakOnAlloc()
	{
#if defined __PLATFORM_WIN32
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

		ifstream fs;
		fs.open("debugalloc.cfg");

		if (!fs.is_open())
			return;

		long allocNum;
		fs >> allocNum;

		// �����ڼ�ı�_crtBreakAlloc��ֵ
		_CrtSetBreakAlloc(allocNum);
#elif defined __PLATFORM_LINUX

#endif // #if defined __PLATFORM_WIN32
	}
}

