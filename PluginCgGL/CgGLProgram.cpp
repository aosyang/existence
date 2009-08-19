#include "CgGLProgram.h"

CgGLProgram::~CgGLProgram()
{

}

void CgGLProgram::SetMatrix4Param(const String& paramName, const Matrix4& mat)
{
	CGparameter param = cgGetNamedParameter(m_CGProgram, paramName.Data());
	if (param)
		cgSetMatrixParameterfr(param, mat.mArray);
}
