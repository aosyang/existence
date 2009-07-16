#include "GLHardwareFeature.h"

void GLHardwareFeature::DetectFeature()
{
	if (glewIsSupported("GL_ARB_vertex_buffer_object") == GL_TRUE)
	{
		supportedVBO = true;
	}

	glGetIntegerv(GL_MAX_LIGHTS, &maxLightNum);

	//int max_fragment_instructions = 0;
	//glGetProgramivARB(  GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &max_fragment_instructions);

	//int max_vertex_instructions = 0;
	//glGetProgramivARB(  GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB,	&max_vertex_instructions);
}
