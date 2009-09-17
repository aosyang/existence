#include "GLHardwareFeature.h"

GLHardwareFeature::GLHardwareFeature()
: supportedVBO(false),
  supportedFBO(false),
  maxLightNum(0)
{
}

void GLHardwareFeature::DetectFeature()
{
	m_FeatureString = "OpenGL Render System hardware feature: ";

	// VBO支持
	m_FeatureString += "VBO supports ";
	if (glewIsSupported("GL_ARB_vertex_buffer_object") == GL_TRUE)
	{
		supportedVBO = true;
		m_FeatureString += "Yes; ";
	}
	else
	{
		m_FeatureString += "No; ";
	}

	// FBO支持
	m_FeatureString += "FBO supports ";
	if (glewIsSupported("GL_EXT_framebuffer_object") == GL_TRUE)
	{
		supportedFBO = true;
		m_FeatureString += "Yes; ";
	}
	else
	{
		m_FeatureString += "No; ";
	}

	m_FeatureString += "Max light number ";
	glGetIntegerv(GL_MAX_LIGHTS, &maxLightNum);
	m_FeatureString += String(maxLightNum) + "; ";

	m_FeatureString += "Non pow of 2 texture supports ";
	if (__GLEW_ARB_texture_non_power_of_two==GL_FALSE)
	{
		supportedNonPowOf2Texture = false;
		m_FeatureString += "No; ";
	}
	else
	{
		m_FeatureString += "Yes; ";
	}


	//int max_fragment_instructions = 0;
	//glGetProgramivARB(  GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &max_fragment_instructions);

	//int max_vertex_instructions = 0;
	//glGetProgramivARB(  GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB,	&max_vertex_instructions);
}
