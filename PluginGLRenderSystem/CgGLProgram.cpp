#include "CgGLProgram.h"
#include "CgGLDevice.h"
#include "GLRenderSystem.h"

#include <Cg/cgGL.h>

namespace Gen
{
	CgGLProgram::CgGLProgram()
	: m_GpuProgramType(GPU_PROGRAM_UNKNOWN),
	  m_CgProgram(0)
	{

	}

	CgGLProgram::~CgGLProgram()
	{
		if (cgGLIsProgramLoaded(m_CgProgram) == CG_TRUE)
		{
			cgGLUnloadProgram(m_CgProgram);
		}
	}

	void CgGLProgram::SetAutoMatrixParam(const String& paramName, AutoMatrixType autoMatrix)
	{
		CGparameter param = cgGetNamedParameter(m_CgProgram, paramName.Data());
		if (!param) return;

		m_AutoMatrixMap[param] = autoMatrix;
	}

	void CgGLProgram::SetMatrix4Param(const String& paramName, const Matrix4& mat)
	{
		CGparameter param = cgGetNamedParameter(m_CgProgram, paramName.Data());
		if (param)
			cgSetMatrixParameterfr(param, mat.mArray);
	}

	void CgGLProgram::SetFloatParam(const String& paramName, float val)
	{
		CGparameter param = cgGetNamedParameter(m_CgProgram, paramName.Data());
		if (param)
			cgSetParameter1f(param, val);
	}

	void CgGLProgram::UpdateParameters()
	{
		std::map<CGparameter, AutoMatrixType>::iterator iter;
		for (iter=m_AutoMatrixMap.begin(); iter!=m_AutoMatrixMap.end(); iter++)
		{
			switch (iter->second)
			{
			case AUTO_MATRIX_MODEL:
				cgSetMatrixParameterfr(iter->first, CgGLDevice::m_sGLRenderer->GetModelMatrix().mArray);
				break;
			case AUTO_MATRIX_MODELVIEW:
				cgGLSetStateMatrixParameter(iter->first, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
				break;
			case AUTO_MATRIX_PROJECTION:
				cgGLSetStateMatrixParameter(iter->first, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
				break;
			case AUTO_MATRIX_TEXTURE:
				cgGLSetStateMatrixParameter(iter->first, CG_GL_TEXTURE_MATRIX, CG_GL_MATRIX_IDENTITY);
				break;
			case AUTO_MATRIX_MODELVIEW_PROJECTION:
				cgGLSetStateMatrixParameter(iter->first, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
				break;
			}
		}
	}

	void CgGLProgram::LoadCgProgramFromFile(const String& filename, const String& entry, CGcontext context, CGprofile p)
	{
		m_CgProgram = cgCreateProgramFromFile(context, CG_SOURCE, filename.Data(), p, entry.Data(), NULL);
		CgGLDevice::CheckCgError();

		// ‘ÿ»Îshader
		cgGLLoadProgram(m_CgProgram);
		CgGLDevice::CheckCgError();

		//CGparameter param = cgGetFirstParameter(m_CgProgram, CG_PROGRAM);
		//while (param)
		//{
		//	const char* name = cgGetParameterName(param);

		//	param = cgGetNextParameter(param);
		//}
	}

}
