//-----------------------------------------------------------------------------------
/// Debug.h 调试用辅助函数
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
		/// 打开内存分配断点功能
		/// 
		/// 直接通过文件指定内存分配断点，无需重新编译代码
		///
		/// \remark
		/// 这个方法会读取运行路径下的debugalloc.cfg文件，并设置内存分配断点
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

