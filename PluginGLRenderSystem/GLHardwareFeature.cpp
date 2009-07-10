#include "GLHardwareFeature.h"

void GLHardwareFeature::DetectFeature()
{
	if (glewIsSupported("GL_ARB_vertex_buffer_object") == GL_TRUE)
	{
		supportedVBO = true;
	}

	glGetIntegerv(GL_MAX_LIGHTS, &maxLightNum);
}
