//-----------------------------------------------------------------------------------
/// GLEnums.cpp OpenGL渲染系统枚举值
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "GLEnums.h"
#include "Texture.h"
#include "GL/glew.h"

namespace Gen
{
	GLenum GetGLPrimtiveType(PrimitiveType type)
	{
		GLenum t;

		switch (type)
		{
		case PRIM_LINES:
			t = GL_LINES;
			break;
		case PRIM_TRIANGLES:
		default:
			t = GL_TRIANGLES;
			break;
		}

		return t;
	}

	GLint GetFilterType(int type)
	{
		switch (type)
		{
		case FILTER_TYPE_NEAREST:
			return GL_NEAREST;
		case FILTER_TYPE_LINEAR:
			return GL_LINEAR;
		case FILTER_TYPE_NEAREST_MIPMAP_NEAREST:
			return GL_NEAREST_MIPMAP_NEAREST;
		case FILTER_TYPE_LINEAR_MIPMAP_NEAREST:
			return GL_LINEAR_MIPMAP_NEAREST;
		case FILTER_TYPE_NEAREST_MIPMAP_LINEAR:
			return GL_NEAREST_MIPMAP_LINEAR;
		case FILTER_TYPE_LINEAR_MIPMAP_LINEAR:
			return GL_LINEAR_MIPMAP_LINEAR;
		}

		// 默认返回Nearest模式
		return GL_NEAREST;
	}

	GLint GetBlendFactor(int factor)
	{
		switch (factor)
		{
		case BLEND_FACTOR_ZERO:
			return GL_ZERO;
		case BLEND_FACTOR_ONE:
			return GL_ONE;
		case BLEND_FACTOR_SRC_COLOR:
			return GL_SRC_COLOR;
		case BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
			return GL_ONE_MINUS_SRC_COLOR;
		case BLEND_FACTOR_SRC_ALPHA:
			return GL_SRC_ALPHA;
		case BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
			return GL_ONE_MINUS_SRC_ALPHA;
		case BLEND_FACTOR_DST_ALPHA:
			return GL_DST_ALPHA;
		case BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
			return GL_ONE_MINUS_DST_ALPHA;
		case BLEND_FACTOR_DST_COLOR:
			return GL_DST_COLOR;
		case BLEND_FACTOR_ONE_MINUS_DST_COLOR:
			return GL_ONE_MINUS_DST_COLOR;
		case BLEND_FACTOR_SRC_ALPHA_SATURATE:
			return GL_SRC_ALPHA_SATURATE;
		}

		// 传递其他非法参数，返回zero
		return BLEND_FACTOR_ZERO;
	}

	GLint GetTextureEnvironmentMode(int mode)
	{
		switch (mode)
		{
		case ENV_MODE_MODULATE:
			return GL_MODULATE;
		case ENV_MODE_REPLACE:
			return GL_REPLACE;
		case ENV_MODE_ADD:
			return GL_ADD;
		}

		return GL_MODULATE;
	}
}
