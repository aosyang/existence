//-----------------------------------------------------------------------------------
/// Debug.cpp �����ø�������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Debug.h"

#include <crtdbg.h>
#include <fstream>

using namespace std;

namespace Debug
{
	void EnableBreakOnAlloc()
	{
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

		ifstream fs;
		fs.open("debugalloc.cfg");

		if (!fs.is_open())
			return;

		long allocNum;
		fs >> allocNum;

		// �����ڼ�ı�_crtBreakAlloc��ֵ
		_CrtSetBreakAlloc(allocNum);
	}
}

