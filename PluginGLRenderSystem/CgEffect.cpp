#include "CgEffect.h"
#include "CgGLDevice.h"

namespace Gen
{
	CgEffect::CgEffect()
		: m_CgEffect(NULL)
	{
	}

	CgEffect::~CgEffect()
	{
		if (m_CgEffect) cgDestroyEffect(m_CgEffect);
	}

	void CgEffect::UsePassState(unsigned int i)
	{
		cgSetPassState(m_CgPasses[i]);
		CgGLDevice::CheckCgError();
	}

	void CgEffect::ResetPassState(unsigned int i)
	{
		cgResetPassState(m_CgPasses[i]);
		CgGLDevice::CheckCgError();
	}

	unsigned int CgEffect::GetPassCount()
	{
		return m_CgPasses.size();
	}

	void CgEffect::LoadEffectFromFile(const String& filename, CGcontext context)
	{
		// 从文件创建Effect
		m_CgEffect = cgCreateEffectFromFile(context, filename.Data(), NULL);
		CgGLDevice::CheckCgError();

		// 找到第一个可用的technique
		CGtechnique technique = cgGetFirstTechnique(m_CgEffect);
		while (technique && cgValidateTechnique(technique)==CG_FALSE)
		{
			technique = cgGetNextTechnique(technique);
		}

		AssertFatal(technique, "CgEffect::LoadEffectFromFile() : Cannot find valid technique.");

		// 依次获取Pass
		CGpass pass = cgGetFirstPass(technique);
		while (pass)
		{
			m_CgPasses.push_back(pass);
			pass = cgGetNextPass(pass);
		}
	}
}
