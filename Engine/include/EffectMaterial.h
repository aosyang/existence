#ifndef _EFFECTMATERIAL_H
#define _EFFECTMATERIAL_H

#include "BaseMaterial.h"

namespace Gen
{
	class IEffect;

	class EffectMaterial : public BaseMaterial
	{
	public:
		EffectMaterial(const String& filename);

		// ----- Overwrite BaseMaterial
		void UsePassState(unsigned int i);
		void ResetPassState(unsigned int i);

		unsigned int GetPassCount();

	protected:
		// ----- Overwrite Resource
		/// @copydoc Resource::LoadImpl()
		bool LoadImpl();
		/// @copydoc Resource::UnloadImpl()
		void UnloadImpl();

	protected:
		IEffect*	m_DeviceEffect;
	};
}

#endif
