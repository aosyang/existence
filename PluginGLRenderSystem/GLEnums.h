//-----------------------------------------------------------------------------------
/// GLEnums.h OpenGL��Ⱦϵͳö��ֵ
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
	///	������ļ���������ת��ΪOpenGL����������
	/// @param type
	///		���漸��������
	/// @returns
	///		OpenGL����������
	GLenum GetGLPrimtiveType(PrimitiveType type);

	GLint GetFilterType(int type);
	GLint GetBlendFactor(int factor);
	GLint GetTextureEnvironmentMode(int mode);
}

#endif