//-----------------------------------------------------------------------------------
/// GLEnums.h OpenGL渲染系统枚举值
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _GLENUMS_H
#define _GLENUMS_H

#include "GL/glew.h"

#include "IVertexBuffer.h"

namespace Gen
{
	/// @brief
	///	将引擎的几何体类型转换为OpenGL几何体类型
	/// @param type
	///		引擎几何体类型
	/// @returns
	///		OpenGL几何体类型
	GLenum GetGLPrimtiveType(PrimitiveType type);

	GLint GetFilterType(int type);
	GLint GetBlendFactor(int factor);
	GLint GetTextureEnvironmentMode(int mode);
}

#endif