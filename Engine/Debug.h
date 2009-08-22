//-----------------------------------------------------------------------------------
/// Debug.h �����ø�������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _DEBUG_H
#define _DEBUG_H

#include <assert.h>

#include "Platform.h"
#include "Log.h"

namespace Debug
{

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ���ڴ����ϵ㹦��
	/// 
	/// ֱ��ͨ���ļ�ָ���ڴ����ϵ㣬�������±������
	///
	/// \remark
	/// ����������ȡ����·���µ�debugalloc.cfg�ļ����������ڴ����ϵ�
	//-----------------------------------------------------------------------------------
	void EnableBreakOnAlloc();
}

#if defined __PLATFORM_WIN32
//#define AssertFatal(expression, msg) assert(expression && msg)
#define AssertFatal(expression, msg) if ((expression)==0) { \
		Log.Error(msg"\nExpression : "#expression); \
		MessageBoxA(NULL, msg"\nExpression : "#expression , "Fatal error!",MB_OK|MB_ICONERROR); \
		assert(0); \
	}
#elif defined __PLATFORM_LINUX
#define AssertFatal(expression, msg) if ((expression)==0) { \
		Log.Error(msg"\nExpression : "#expression); \
		printf("!! Faltal error !! "#expression"\n"); \
		assert(0); \
	}
#endif

#endif

