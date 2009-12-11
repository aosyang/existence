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

namespace Gen
{
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

		void FatalError(const char* title, const char* msg, const char* expression, const char* file, long line);
	}

#define AssertFatal(expression, msg) if ((expression)==0) { \
		Debug::FatalError("Fatal error!", msg, #expression, __FILE__, __LINE__); \
	}
}

#endif

