//-----------------------------------------------------------------------------------
/// Material.cpp 材质类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Material.h"
#include "Engine.h"

#include "tinyxml.h"

template <> unsigned int ResourceManager<Material>::m_sIndex = 0;
template <> ResourceManager<Material>::LoadFunc ResourceManager<Material>::m_sLoadFunc = &Material::LoadMaterial;
MaterialLoadFuncMap	Material::s_FuncMap;
RenderStateLoadFuncMap Material::s_RSFuncMap;

void ReadLighting(Material* mat, const String& val);
void ReadColor(Material* mat, const String& val);
void ReadAmbient(Material* mat, const String& val);
void ReadDiffuse(Material* mat, const String& val);
void ReadSpecular(Material* mat, const String& val);
void ReadEmissive(Material* mat, const String& val);
void ReadSpecularLevel(Material* mat, const String& val);
void ReadDepthWriting(Material* mat, const String& val);
void ReadDepthTest(Material* mat, const String& val);
void ReadAlphaTest(Material* mat, const String& val);
void ReadAlphaRef(Material* mat, const String& val);

void ReadRSTexture(texRenderState_t* rs, const String& val);
void ReadRSWrapType(texRenderState_t* rs, const String& val);
void ReadRSMinFilterType(texRenderState_t* rs, const String& val);
void ReadRSMagFilterType(texRenderState_t* rs, const String& val);
void ReadRSGenMode(texRenderState_t* rs, const String& val);
void ReadRSBlending(texRenderState_t* rs, const String& val);
void ReadRSSrcBlendFactor(texRenderState_t* rs, const String& val);
void ReadRSDstBlendFactor(texRenderState_t* rs, const String& val);
void ReadRSEnvMode(texRenderState_t* rs, const String& val);

int InitializeMaterialFunc()
{
	Material::s_FuncMap["Lighting"] = &ReadLighting;
	Material::s_FuncMap["Color"] = &ReadColor;
	Material::s_FuncMap["Ambient"] = &ReadAmbient;
	Material::s_FuncMap["Diffuse"] = &ReadDiffuse;
	Material::s_FuncMap["Specular"] = &ReadSpecular;
	Material::s_FuncMap["Emissive"] = &ReadEmissive;
	Material::s_FuncMap["SpecularLevel"] = &ReadSpecularLevel;
	Material::s_FuncMap["DepthWriting"] = &ReadDepthWriting;
	Material::s_FuncMap["DepthTest"] = &ReadDepthTest;
	Material::s_FuncMap["AlphaTest"] = &ReadAlphaTest;
	Material::s_FuncMap["AlphaRef"] = &ReadAlphaRef;

	Material::s_RSFuncMap["Texture"] = &ReadRSTexture;
	Material::s_RSFuncMap["WrapType"] = &ReadRSWrapType;
	Material::s_RSFuncMap["MinFilterType"] = &ReadRSMinFilterType;
	Material::s_RSFuncMap["MagFilterType"] = &ReadRSMagFilterType;
	Material::s_RSFuncMap["TextureGenMode"] = &ReadRSGenMode;
	Material::s_RSFuncMap["Blending"] = &ReadRSBlending;
	Material::s_RSFuncMap["TextureBlendFactorSrc"] = &ReadRSSrcBlendFactor;
	Material::s_RSFuncMap["TextureBlendFactorDst"] = &ReadRSDstBlendFactor;
	Material::s_RSFuncMap["TextureEnvMode"] = &ReadRSEnvMode;
	return 1;
}

static int mat_init = InitializeMaterialFunc();

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

Material* Material::LoadMaterial(const String& filename)
{
	TiXmlDocument matDoc(filename.Data());
	matDoc.LoadFile();

	TiXmlElement* elem = matDoc.FirstChildElement();

	if (!elem) return NULL;

	const char* name = elem->Attribute("name");
	if (!name) return NULL;

	TiXmlElement* mat_elem = elem->FirstChildElement();
	Material* material = new Material();
	int renderState = 0;
	while(mat_elem)
	{
		if (String(mat_elem->Value())=="TextureRenderState")
		{
			TiXmlElement* rs_elem = mat_elem->FirstChildElement();
			if (!rs_elem)
				break;

			material->SetTextureLayerEnabled(renderState, true);

			while(rs_elem)
			{
				texRenderState_t* rs = material->GetTextureRenderState(renderState);
				LoadRenderState(rs, rs_elem->Value(), rs_elem->GetText());
				rs_elem = rs_elem->NextSiblingElement();
			}
			renderState++;
		}
		else
			LoadAttrib(material, mat_elem->Value(), mat_elem->GetText());
		mat_elem = mat_elem->NextSiblingElement();
	}

	return material;
}

void Material::SaveToFile(const String& filename, bool outputDefault)
{
	// 将材质保存到文件
	TiXmlDocument doc(filename.Data());
	TiXmlDeclaration decl("1.0", "gb2312", "yes");
	doc.InsertEndChild(decl);

	TiXmlElement elem_mat("Material");
	elem_mat.SetAttribute("name", m_Name->Data());

	// 属性写入xml文件，如果与默认值相等就不写入
#define SAVE_MATERIAL_ATTRIB(attr, def_val) \
	if ((Get##attr()!=def_val) || outputDefault) { \
		TiXmlElement name##attr(#attr); \
		TiXmlText val##attr(String(Get##attr()).Data()); \
		name##attr.InsertEndChild(val##attr); \
		elem_mat.InsertEndChild(name##attr); \
	}

	//TiXmlElement lighting("Lighting");
	//TiXmlText lighting_val(String(GetLighting()).Data());
	//lighting.InsertEndChild(lighting_val);
	//elem_mat.InsertEndChild(lighting);
	SAVE_MATERIAL_ATTRIB(Lighting, true);
	SAVE_MATERIAL_ATTRIB(Color, Color4f(1.0f, 1.0f, 1.0f, 1.0f));
	SAVE_MATERIAL_ATTRIB(Ambient, Color4f(0.5f, 0.5f, 0.5f));
	SAVE_MATERIAL_ATTRIB(Diffuse, Color4f(0.8f, 0.8f, 0.8f));
	SAVE_MATERIAL_ATTRIB(Specular, Color4f(0.0f, 0.0f, 0.0f));
	SAVE_MATERIAL_ATTRIB(Emissive, Color4f(0.0f, 0.0f, 0.0f));
	SAVE_MATERIAL_ATTRIB(SpecularLevel, 0.0f);
	SAVE_MATERIAL_ATTRIB(DepthWriting, true);
	SAVE_MATERIAL_ATTRIB(DepthTest, true);
	SAVE_MATERIAL_ATTRIB(AlphaTest, false);
	SAVE_MATERIAL_ATTRIB(AlphaRef, 0.0f);

	for (int i=0; i<8; i++)
	{
		if (!m_TextureLayerEnabled[i]) continue;

		TiXmlElement elem_rs("TextureRenderState");
		texRenderState_t* rs = &m_TextureRenderState[i];

#define SAVE_RENDER_STATE_ATTRIB(attr, member) \
		TiXmlElement name##attr(#attr); \
		TiXmlText val##attr(String(rs->member).Data()); \
		name##attr.InsertEndChild(val##attr); \
		elem_rs.InsertEndChild(name##attr);

		SAVE_RENDER_STATE_ATTRIB(Texture, texture->GetName());
		SAVE_RENDER_STATE_ATTRIB(WrapType, wrapType);
		SAVE_RENDER_STATE_ATTRIB(MinFilterType, minFilterType);
		SAVE_RENDER_STATE_ATTRIB(MagFilterType, magFilterType);
		SAVE_RENDER_STATE_ATTRIB(TextureGenMode, genMode);

		SAVE_RENDER_STATE_ATTRIB(Blending, useBlending);
		SAVE_RENDER_STATE_ATTRIB(TextureBlendFactorSrc, srcBlendFactor);
		SAVE_RENDER_STATE_ATTRIB(TextureBlendFactorDst, dstBlendFactor);

		SAVE_RENDER_STATE_ATTRIB(TextureEnvMode, envMode);

		elem_mat.InsertEndChild(elem_rs);
	}

	doc.InsertEndChild(elem_mat);
	doc.SaveFile();
}

void Material::LoadAttrib(Material* material, const String& attrib, const String& val)
{
	if (s_FuncMap.find(attrib)!=s_FuncMap.end())
		(*s_FuncMap[attrib])(material, val);
}

void ReadLighting(Material* mat, const String& val)
{
	mat->SetLighting(val.ToBool());
}

void ReadColor(Material* mat, const String& val)
{
	mat->SetColor(val.ToColor4f());
}

void ReadAmbient(Material* mat, const String& val)
{
	mat->SetAmbient(val.ToColor4f());
}

void ReadDiffuse(Material* mat, const String& val)
{
	mat->SetColor(val.ToColor4f());
}

void ReadSpecular(Material* mat, const String& val)
{
	mat->SetSpecular(val.ToColor4f());
}

void ReadEmissive(Material* mat, const String& val)
{
	mat->SetEmissive(val.ToColor4f());
}

void ReadSpecularLevel(Material* mat, const String& val)
{
	mat->SetSpecularLevel(val.ToFloat());
}

void ReadDepthWriting(Material* mat, const String& val)
{
	mat->SetDepthWriting(val.ToBool());
}

void ReadDepthTest(Material* mat, const String& val)
{
	mat->SetDepthTest(val.ToBool());
}

void ReadAlphaTest(Material* mat, const String& val)
{
	mat->SetAlphaTest(val.ToBool());
}

void ReadAlphaRef(Material* mat, const String& val)
{
	mat->SetAlphaRef(val.ToFloat());
}

void Material::LoadRenderState(texRenderState* rs, const String& attrib, const String& val)
{
	if (s_RSFuncMap.find(attrib)!=s_RSFuncMap.end())
		(*s_RSFuncMap[attrib])(rs, val);
}

void ReadRSTexture(texRenderState_t* rs, const String& val)
{
	rs->texture = renderer->GetTexture(val);
}

void ReadRSWrapType(texRenderState_t* rs, const String& val)
{
	rs->wrapType = (TextureWrapType)val.ToInt();
}

void ReadRSMinFilterType(texRenderState_t* rs, const String& val)
{
	rs->minFilterType = (TextureFilterType)val.ToInt();
}

void ReadRSMagFilterType(texRenderState_t* rs, const String& val)
{
	rs->magFilterType = (TextureFilterType)val.ToInt();
}

void ReadRSGenMode(texRenderState_t* rs, const String& val)
{
	rs->genMode = (TextureGenMode)val.ToInt();
}

void ReadRSBlending(texRenderState_t* rs, const String& val)
{
	rs->useBlending = val.ToBool();
}

void ReadRSSrcBlendFactor(texRenderState_t* rs, const String& val)
{
	rs->srcBlendFactor = (TextureBlendFactor)val.ToInt();
}

void ReadRSDstBlendFactor(texRenderState_t* rs, const String& val)
{
	rs->dstBlendFactor = (TextureBlendFactor)val.ToInt();
}

void ReadRSEnvMode(texRenderState_t* rs, const String& val)
{
	rs->envMode = (TextureEnvMode)val.ToInt();
}


