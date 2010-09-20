//-----------------------------------------------------------------------------------
/// CgGLDevice.cpp Cg for OpenGL控制设备
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
		// 创建CG环境
		m_CgContext = cgCreateContext();
		CheckCgError();

		// 注册CgFX的渲染通路状态
		cgGLRegisterStates(m_CgContext);

		// 设置参数模式为deferred
		cgSetParameterSettingMode(m_CgContext, CG_DEFERRED_PARAMETER_SETTING);

		// 获得Vertex Profile
		m_CgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
		cgGLSetOptimalOptions(m_CgVertexProfile);
		CheckCgError();

		// 获得Fragment Profile
		m_CgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
		cgGLSetOptimalOptions(m_CgFragmentProfile);
		CheckCgError();
	}

	void CgGLDevice::Shutdown()
	{
		// 销毁Cg环境
		//cgDestroyContext(m_CgContext);
	}

	void CgGLDevice::CheckCgError()
	{
		// 获取最近发生的Cg错误
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

		// 记录Shader类型
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
