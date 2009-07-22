//-----------------------------------------------------------------------------------
/// CgGLTexture.cpp Cg OpenGL²ÄÖÊÀà
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "CgGLTexture.h"

CgGLRenderSystem* CgGLTexture::m_CgGLRenderer;

CgGLTexture::CgGLTexture()
: m_WrapType(WRAP_TYPE_REPEAT),
  m_MinFilterType(FILTER_TYPE_LINEAR_MIPMAP_NEAREST),
  m_MagFilterType(FILTER_TYPE_LINEAR),
  m_UseEyeSpaceTexCoord(false),
  m_GLTextureID(0),
  m_CgVertexProgram(0),
  m_CgFragmentProgram(0)
{
	m_EyeSpaceMatrix.Identity();
}

void CgGLTexture::SetVertexProgram(const String& filename, const String& entry)
{
	m_CgVertexProgram = m_CgGLRenderer->LoadProgramFromFile(filename, entry, CG_VERTEX_PROGRAM);
}

void CgGLTexture::SetFragmentProgram(const String& filename, const String& entry)
{
	m_CgFragmentProgram = m_CgGLRenderer->LoadProgramFromFile(filename, entry, CG_FRAGMENT_PROGRAM);	
}
