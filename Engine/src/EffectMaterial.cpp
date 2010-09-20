#include "EffectMaterial.h"
#include "Renderer.h"
#include "IEffect.h"

namespace Gen
{
	EffectMaterial::EffectMaterial(const String& filename)
		: BaseMaterial(filename),
		  m_DeviceEffect(NULL)
	{
	}
	void EffectMaterial::UsePassState(unsigned int i)
	{
		if (m_DeviceEffect) m_DeviceEffect->UsePassState(i);
	}

	void EffectMaterial::ResetPassState(unsigned int i)
	{
		if (m_DeviceEffect) m_DeviceEffect->ResetPassState(i);
	}

	unsigned int EffectMaterial::GetPassCount()
	{
		if (m_DeviceEffect) return m_DeviceEffect->GetPassCount();
	}

	bool EffectMaterial::LoadImpl()
	{
		m_DeviceEffect = Renderer::Instance().LoadEffect(m_Filename);

		return m_DeviceEffect != NULL;
	}

	void EffectMaterial::UnloadImpl()
	{
		SAFE_DELETE(m_DeviceEffect);
	}
}
