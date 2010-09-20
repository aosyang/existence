//-----------------------------------------------------------------------------------
/// CgGLProgram.h CgGL×ÅÉ«³ÌÐò
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _CGGLPROGRAM_H
#define _CGGLPROGRAM_H

#include "IGpuProgram.h"

#include <Cg/cg.h>

#include <map>

namespace Gen
{
	class CgGLDevice;

	class CgGLProgram : public IGpuProgram
	{
		friend class CgGLDevice;
	public:
		CgGLProgram();
		~CgGLProgram();

		// ----- Overwrite IGpuProgram
		GpuProgramType GetType() const { return m_GpuProgramType; }

		void SetAutoMatrixParam(const String& paramName, AutoMatrixType autoMatrix);

		void SetMatrix4Param(const String& paramName, const Matrix4& mat);
		void SetFloatParam(const String& paramName, float val);

		void UpdateParameters();
		// ----- CgGLProgram Methods
		void LoadCgProgramFromFile(const String& filename, const String& entry, CGcontext context, CGprofile p);

	private:
		GpuProgramType	m_GpuProgramType;
		CGprogram		m_CgProgram;
		std::map<CGparameter, AutoMatrixType>
						m_AutoMatrixMap;
	};
}

#endif
