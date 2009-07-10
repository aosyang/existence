//-----------------------------------------------------------------------------------
/// Material.cpp 材质类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Material.h"

Material::Material()
: m_Lighting(true),
  //m_Ambient(0.2f, 0.2f, 0.2f),
  m_Ambient(0.5f, 0.5f, 0.5f),
  m_Diffuse(0.8f, 0.8f, 0.8f),
  m_Specular(0.0f, 0.0f, 0.0f),
  m_Emissive(0.0f, 0.0f, 0.0f),
  m_SpecularLevel(0.0f),
  //m_Texture(0),
  m_DepthWriting(true),
  m_DepthTest(true),
  m_AlphaTest(false),
  m_AlphaReference(0.0f)
{
	for (int i=1; i<8; i++)
		m_TextureLayerEnabled[i] = false;
	m_TextureLayerEnabled[0] = true;
}

Material::~Material()
{}

unsigned int ResourceManager<Material>::m_sIndex = 0;
