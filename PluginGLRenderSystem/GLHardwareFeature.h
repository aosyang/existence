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

class GLHardwareFeature
{
public:
	GLHardwareFeature() { memset(this, 0, sizeof(GLHardwareFeature)); }

	void DetectFeature();

	inline int GetMaxLightNum() const { return maxLightNum; }

	bool	supportedVBO;
	bool	supportedFBO;
	int		maxLightNum;
};

#endif
