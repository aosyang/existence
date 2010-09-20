//-----------------------------------------------------------------------------------
/// IGpuProgram.h 显卡着色程序接口
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IGPUPROGRAM_H
#define _IGPUPROGRAM_H

#include "EString.h"
#include "Matrix4.h"

namespace Gen
{
	enum GpuProgramType
	{
		GPU_VERTEX_PROGRAM,
		GPU_FRAGMENT_PROGRAM,
		GPU_PROGRAM_UNKNOWN,
	};

	enum AutoMatrixType
	{
		AUTO_MATRIX_MODEL,
		AUTO_MATRIX_MODELVIEW,
		AUTO_MATRIX_PROJECTION,
		AUTO_MATRIX_TEXTURE,
		AUTO_MATRIX_MODELVIEW_PROJECTION,
		// TODO: to have more
	};

	class IGpuProgram
	{
	public:
		virtual ~IGpuProgram() {}

		virtual GpuProgramType GetType() const = 0;

		virtual void SetAutoMatrixParam(const String& paramName, AutoMatrixType autoMatrix) = 0;

		virtual void SetMatrix4Param(const String& paramName, const Matrix4& mat) = 0;
		virtual void SetFloatParam(const String& paramName, float val) = 0;

		virtual void UpdateParameters() = 0;
	};
}

#endif
