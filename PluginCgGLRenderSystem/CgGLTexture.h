//-----------------------------------------------------------------------------------
/// CgGLTexture.h OpenGL纹理类，实现了OpenGL版本的ITexture
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _CgGLTexture_H
#define _CgGLTexture_H

#include "ITexture.h"
#include "CgGLRenderSystem.h"
//#include <Cg/cg.h>
//#include <Cg/cgGL.h>

class CgGLRenderSystem;

class CgGLTexture : public ITexture
{
public:
	CgGLTexture();

	void SetWrapType(TextureWrapType type) { m_WrapType = type; }
	int GetWrapType() const { return m_WrapType; }

	void SetMinFilterType(TextureFilterType type) { m_MinFilterType = type; }
	int GetMinFilterType() const { return m_MinFilterType; }
	void SetMagFilterType(TextureFilterType type) { m_MagFilterType = type; }
	int GetMagFilterType() const { return m_MagFilterType; }

	void SetUseEyeSpaceTexCoord(bool useEyeSpace) { m_UseEyeSpaceTexCoord = useEyeSpace; }
	bool GetUseEyeSpaceTexCoord() const { return m_UseEyeSpaceTexCoord; }
	void SetEyeSpaceMatrix(const Matrix4& matrix) { m_EyeSpaceMatrix = matrix; }
	Matrix4& GetEyeSpaceMatrix() { return m_EyeSpaceMatrix; }

	void SetVertexProgram(const String& filename, const String& entry);
	CGprogram GetVertexProgram() const { return m_CgVertexProgram; }
	void SetFragmentProgram(const String& filename, const String& entry);
	CGprogram GetFragmentProgram() const { return m_CgFragmentProgram; }

	// TODO: 统一Get
	CGparameter GetModelViewProjMat() { return cgGetNamedParameter(m_CgVertexProgram, "matModelViewProj"); }
	CGparameter GetModelViewMat() { return cgGetNamedParameter(m_CgVertexProgram, "matModelView"); }

	// ----- GLTexture Methods

	inline unsigned int GetGLTextureID() const { return m_GLTextureID; }
	inline unsigned int* GetGLTextureIDPtr() { return &m_GLTextureID; }

	static CgGLRenderSystem*	m_CgGLRenderer;

private:


	int				m_WrapType;
	
	int				m_MinFilterType;
	int				m_MagFilterType;

	bool			m_UseEyeSpaceTexCoord;
	Matrix4			m_EyeSpaceMatrix;

	unsigned int	m_GLTextureID;

	Matrix4			m_ModelViewProj;

	CGprogram		m_CgVertexProgram,
					m_CgFragmentProgram;
};


#endif
