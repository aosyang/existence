//-----------------------------------------------------------------------------------
/// GLHardwareFeature.h OpenGLÓ²¼þÖ§³Ö
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------


#ifndef _GLHARDWREFEATURE_H
#define _GLHARDWREFEATURE_H

#include "GL/glew.h"
#include <memory.h>
#include "EString.h"

namespace Gen
{
	class GLHardwareFeature
	{
	public:
		GLHardwareFeature();

		void DetectFeature();

		inline int GetMaxLightNum() const { return maxLightNum; }

		inline const String& GetFeatureString() const { return m_FeatureString; }

		String	m_FeatureString;

		bool	supportedVBO;
		bool	supportedFBO;
		int		maxLightNum;
		bool	supportedNonPowOf2Texture;
	};
}

#endif
