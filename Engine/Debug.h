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
}

//#define AssertFatal(expression, msg) assert(expression && msg)
#define AssertFatal(expression, msg) if (!(expression)) { \
		Log.Error(msg"\nExpression : "#expression); \
		MessageBoxA(NULL, msg"\nExpression : "#expression , "Fatal error!",MB_OK|MB_ICONERROR); \
		assert(0); \
	}

#endif
