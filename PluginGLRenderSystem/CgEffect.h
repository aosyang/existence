#ifndef _CGEFFECT_H
#define _CGEFFECT_H

#include "IEffect.h"
#include "EString.h"

#include <Cg/cg.h>

#include <vector>

namespace Gen
{
	class CgEffect : public IEffect
	{
	public:
		CgEffect();
		~CgEffect();

		// ----- Overwrite IEffect
		void UsePassState(unsigned int i);
		void ResetPassState(unsigned int i);
		unsigned int GetPassCount();

		// ----- CgEffect Methods
		void LoadEffectFromFile(const String& filename, CGcontext context);

	private:
		CGeffect				m_CgEffect;
		std::vector<CGpass>		m_CgPasses;
	};
}

#endif
