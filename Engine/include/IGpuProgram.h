#ifndef _IGPUPROGRAM_H
#define _IGPUPROGRAM_H

#include "EString.h"
#include "Matrix4.h"

enum GpuProgramType
{
	GPU_VERTEX_PROGRAM,
	GPU_FRAGMENT_PROGRAM,
};

class IGpuProgram
{
public:
	virtual ~IGpuProgram() {}

	virtual void SetMatrix4Param(const String& param, const Matrix4& mat) = 0;
};

#endif
