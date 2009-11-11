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

		void FatalError(const String& title, const String& msg, const String& file, const String& line);

		static String	_DEBUG_MSG;
	}

#define AssertFatal(expression, msg) if ((expression)==0) { \
		Debug::_DEBUG_MSG = ""; \
		Debug::_DEBUG_MSG += msg; \
		Debug::_DEBUG_MSG += "\nExpression : "; \
		Debug::_DEBUG_MSG += #expression; \
		Debug::FatalError("Fatal error!", Debug::_DEBUG_MSG, __FILE__, __LINE__); \
	}
}

#endif

