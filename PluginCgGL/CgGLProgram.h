#ifndef _CGGLPROGRAM_H
#define _CGGLPROGRAM_H

#include "IGpuProgram.h"

#include <Cg/cg.h>
#include <Cg/cgGL.h>

class CgGLProgram : public IGpuProgram
{
public:
	~CgGLProgram();

	void SetMatrix4Param(const String& paramName, const Matrix4& mat);

	CGprogram	m_CGProgram;
};

#endif
