//-----------------------------------------------------------------------------------
/// Debug.cpp 调试用辅助函数
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

namespace Gen
{
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

			// 调试期间改变_crtBreakAlloc的值
			_CrtSetBreakAlloc(allocNum);
#elif defined __PLATFORM_LINUX

#endif // #if defined __PLATFORM_WIN32
		}

		void FatalError(const char* title, const char* msg, const char* expression, const char* file, long line)
		{
			String errorMsg = msg;
			errorMsg += "\nExpression : ";
			errorMsg += expression;
			errorMsg += "\nIn file ";
			errorMsg += file;
			errorMsg += "\nLine ";
			errorMsg += line;

			Log.Error(errorMsg);
#if defined __PLATFORM_WIN32
			MessageBoxA(NULL, errorMsg.Data(), title, MB_OK|MB_ICONERROR);
#elif defined __PLATFORM_LINUX
			printf("%s %s", title, errorMsg.Data());
#endif
			assert(0);
		}

	}
}

