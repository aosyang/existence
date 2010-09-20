//-----------------------------------------------------------------------------------
/// CgGLDevice.cpp Cg for OpenGL�����豸
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#include "Debug.h"

#include "CgGLDevice.h"
#include "CgGLProgram.h"
#include "CgEffect.h"

#include <Cg/cgGL.h>

namespace Gen
{
	GLRenderer* CgGLDevice::m_sGLRenderer = NULL;

	CgGLDevice::CgGLDevice()
		: m_VPEnabled(false),
		  m_FPEnabled(false)
	{
	}

	void CgGLDevice::Initialize()
	{
		// ����CG����
		m_CgContext = cgCreateContext();
		CheckCgError();

		// ע��CgFX����Ⱦͨ·״̬
		cgGLRegisterStates(m_CgContext);

		// ���ò���ģʽΪdeferred
		cgSetParameterSettingMode(m_CgContext, CG_DEFERRED_PARAMETER_SETTING);

		// ���Vertex Profile
		m_CgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
		cgGLSetOptimalOptions(m_CgVertexProfile);
		CheckCgError();

		// ���Fragment Profile
		m_CgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
		cgGLSetOptimalOptions(m_CgFragmentProfile);
		CheckCgError();
	}

	void CgGLDevice::Shutdown()
	{
		// ����Cg����
		//cgDestroyContext(m_CgContext);
	}

	void CgGLDevice::CheckCgError()
	{
		// ��ȡ���������Cg����
		CGerror error;
		const char* errorStr = cgGetLastErrorString(&error);

		if (error != CG_NO_ERROR)
		{
			if (error == CG_COMPILER_ERROR)
			{
				AssertFatal(0, "CgGLDevice::CheckCgError() : Error on compiling Cg program!");
			}
			AssertFatal(0, "CgGLDevice::CheckCgError() : A Cg error occurred!");
		}
	}

	IGpuProgram* CgGLDevice::LoadGpuProgramFromFile(const String& filename, const String& entry, GpuProgramType type)
	{
		CGprofile* profile;
		switch(type)
		{
		case GPU_VERTEX_PROGRAM:
			profile = &m_CgVertexProfile;
			break;
		case GPU_FRAGMENT_PROGRAM:
			profile = &m_CgFragmentProfile;
			break;
		default:
			AssertFatal(0, "CgGLDevice::LoadProgramFromFile() : Invalid program type!");
		};

		CgGLProgram* program = new CgGLProgram();
		program->LoadCgProgramFromFile(filename, entry, m_CgContext, *profile);

		// ��¼Shader����
		program->m_GpuProgramType = type;

		return program;
	}

	void CgGLDevice::EnableProfile(GpuProfile profile)
	{
		switch(profile)
		{
		case GPU_PROFILE_VERTEX:
			if (m_VPEnabled) return;
			cgGLEnableProfile(m_CgVertexProfile);
			m_VPEnabled = true;
			break;
		case GPU_PROFILE_FRAGMENT:
			if (m_FPEnabled) return;
			cgGLEnableProfile(m_CgFragmentProfile);
			m_FPEnabled = true;
			break;
		default:
			AssertFatal(0, "CgGLDevice::EnableProfile() : Invalid program type!");
		};

		CheckCgError();
	}

	void CgGLDevice::DisableProfile(GpuProfile profile)
	{
		switch(profile)
		{
		case GPU_PROFILE_VERTEX:
			if (!m_VPEnabled) return;
			cgGLDisableProfile(m_CgVertexProfile);
			m_VPEnabled = false;
			break;
		case GPU_PROFILE_FRAGMENT:
			if (!m_FPEnabled) return;
			cgGLDisableProfile(m_CgFragmentProfile);
			m_FPEnabled = false;
			break;
		default:
			AssertFatal(0, "CgGLDevice::DisableProfile() : Invalid program type!");
		};

		CheckCgError();
	}

	void CgGLDevice::BindCgProgram(IGpuProgram* program)
	{
		cgGLBindProgram(static_cast<CgGLProgram*>(program)->m_CgProgram);
		CheckCgError();
	}

	CgEffect* CgGLDevice::LoadCgEffectFromFile(const String& filename)
	{
		CgEffect* effect = new CgEffect;
		effect->LoadEffectFromFile(filename, m_CgContext);

		return effect;
	}
}
