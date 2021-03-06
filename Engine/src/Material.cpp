//-----------------------------------------------------------------------------------
/// Material.cpp 材质类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Material.h"
#include "TextureManager.h"
#include "Renderer.h"

#include "tinyxml/tinyxml.h"

namespace Gen
{
	MaterialLoadFuncMap	Material::m_sFuncMap;
	RenderStateLoadFuncMap Material::m_sRSFuncMap;

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
	void ReadBlending(Material* mat, const String& val);
	void ReadSrcBlendFactor(Material* mat, const String& val);
	void ReadDstBlendFactor(Material* mat, const String& val);

	void ReadRSTexture(TextureRenderState* rs, const String& val);
	void ReadRSWrapType(TextureRenderState* rs, const String& val);
	void ReadRSMinFilterType(TextureRenderState* rs, const String& val);
	void ReadRSMagFilterType(TextureRenderState* rs, const String& val);
	void ReadRSGenMode(TextureRenderState* rs, const String& val);
	void ReadRSEnvMode(TextureRenderState* rs, const String& val);

	static std::map<const String, TextureWrapType>	g_TexWrapTypeTable;
	static String g_TexWrapTypeStringTable[] =
	{
		"WRAP_TYPE_CLAMP",
		"WRAP_TYPE_REPEAT",
		"WRAP_TYPE_CLAMP_TO_EDGE",
	};

	TextureWrapType StringToWrapType(const String& str)
	{
		if (g_TexWrapTypeTable.find(str)!=g_TexWrapTypeTable.end())
			return g_TexWrapTypeTable[str];

		return WRAP_TYPE_CLAMP;
	}

	String WrapTypeToString(TextureWrapType type)
	{
		if (type<WRAP_TYPE_COUNT && type>=0)
			return g_TexWrapTypeStringTable[type];
		return g_TexWrapTypeStringTable[WRAP_TYPE_CLAMP_TO_EDGE];
	}

	int InitializeMaterialFunc()
	{
		Material::m_sFuncMap["Lighting"] = &ReadLighting;
		Material::m_sFuncMap["Color"] = &ReadColor;
		Material::m_sFuncMap["Ambient"] = &ReadAmbient;
		Material::m_sFuncMap["Diffuse"] = &ReadDiffuse;
		Material::m_sFuncMap["Specular"] = &ReadSpecular;
		Material::m_sFuncMap["Emissive"] = &ReadEmissive;
		Material::m_sFuncMap["SpecularLevel"] = &ReadSpecularLevel;
		Material::m_sFuncMap["DepthWriting"] = &ReadDepthWriting;
		Material::m_sFuncMap["DepthTest"] = &ReadDepthTest;
		Material::m_sFuncMap["AlphaTest"] = &ReadAlphaTest;
		Material::m_sFuncMap["AlphaRef"] = &ReadAlphaRef;
		Material::m_sFuncMap["Blending"] = &ReadBlending;
		Material::m_sFuncMap["SrcBlendFactor"] = &ReadSrcBlendFactor;
		Material::m_sFuncMap["DstBlendFactor"] = &ReadDstBlendFactor;

		Material::m_sRSFuncMap["Texture"] = &ReadRSTexture;
		Material::m_sRSFuncMap["WrapType"] = &ReadRSWrapType;
		Material::m_sRSFuncMap["MinFilterType"] = &ReadRSMinFilterType;
		Material::m_sRSFuncMap["MagFilterType"] = &ReadRSMagFilterType;
		Material::m_sRSFuncMap["TextureGenMode"] = &ReadRSGenMode;
		Material::m_sRSFuncMap["TextureEnvMode"] = &ReadRSEnvMode;

		g_TexWrapTypeTable["WRAP_TYPE_CLAMP"] = WRAP_TYPE_CLAMP;
		g_TexWrapTypeTable["WRAP_TYPE_REPEAT"] = WRAP_TYPE_REPEAT;
		g_TexWrapTypeTable["WRAP_TYPE_CLAMP_TO_EDGE"] = WRAP_TYPE_CLAMP_TO_EDGE;
		return 1;
	}

	static int mat_init = InitializeMaterialFunc();

	Material::Material(const String& filename)
	: BaseMaterial(filename),
	  m_Lighting(true),
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
	  m_AlphaReference(0.0f),
	  m_Blending(true),
	  m_SrcBlendFactor(BLEND_FACTOR_SRC_ALPHA),
	  m_DstBlendFactor(BLEND_FACTOR_ONE_MINUS_SRC_ALPHA),
	  m_VertexProgram(NULL),
	  m_FragmentProgram(NULL)
	{
		for (int i=0; i<8; i++)
		{
			m_TextureLayerEnabled[i] = false;
			m_DeviceTextures[i] = NULL;
		}
		m_TextureLayerEnabled[0] = true;
	}

	Material::~Material()
	{}

	bool Material::LoadImpl()
	{
		TiXmlDocument matDoc(m_Filename.Data());
		matDoc.LoadFile();

		TiXmlElement* elem = matDoc.FirstChildElement();

		if (!elem) return NULL;

		const char* name = elem->Attribute("name");
		if (m_ResourceName!=name) return false;

		TiXmlElement* mat_elem = elem->FirstChildElement();

		int renderState = 0;
		while(mat_elem)
		{
			if (String(mat_elem->Value())=="TextureRenderState")
			{
				TiXmlElement* rs_elem = mat_elem->FirstChildElement();
				if (!rs_elem)
					break;

				this->SetTextureLayerEnabled(renderState, true);

				while(rs_elem)
				{
					TextureRenderState* rs = this->GetTextureRenderState(renderState);
					LoadRenderState(rs, rs_elem->Value(), rs_elem->GetText());
					rs_elem = rs_elem->NextSiblingElement();
				}
				renderState++;
			}
			else
				LoadAttrib(this, mat_elem->Value(), mat_elem->GetText());
			mat_elem = mat_elem->NextSiblingElement();
		}

		return true;
	}
	
	void Material::UnloadImpl()
	{
		// TODO: 将所有状态设置为初值
	}

	bool Material::Trigger()
	{
		if (!Resource::Trigger()) return false;

		for (int i=0; i<8; i++)
		{
			if (m_TextureLayerEnabled[i] && (m_TextureRenderState[i].textureName!=""))
			{
				BaseTexture* tex = TextureManager::Instance().GetByName(m_TextureRenderState[i].textureName);
				if (tex)
				{
					tex->Trigger();
					m_DeviceTextures[i] = tex->GetDeviceTexture();
				}
			}
		}

		return true;
	}

	void Material::UsePassState(unsigned int i)
	{
		AssertFatal(i>=0 && i<GetPassCount(), "Material::UsePassState() : Pass index out of boundary.");
		Renderer::Instance().SetupMaterial(this);
	}

	void Material::ResetPassState(unsigned int i)
	{
		AssertFatal(i>=0 && i<GetPassCount(), "Material::ResetPassState() : Pass index out of boundary.");
	}

	void Material::SetTexture(const String& texName, unsigned int index)
	{
		BaseTexture* tex = TextureManager::Instance().GetByName(texName);

		SetTexture(tex, index);
	}

	void Material::SetTexture(BaseTexture* tex, unsigned int index)
	{
		if (tex)
		{
			m_TextureRenderState[index].textureName = tex->GetResourceName();
			if (m_IsResourceLoaded)
			{
				tex->Trigger();
				m_DeviceTextures[index] = tex->GetDeviceTexture();
			}
		}
	}

	void Material::SetVertexProgram(IGpuProgram* program)
	{
		AssertFatal(program->GetType()==GPU_VERTEX_PROGRAM, "Material::SetVertexProgram() : Invalid gpu program type");
		m_VertexProgram = program;
	}

	void Material::SetFragmentProgram(IGpuProgram* program)
	{
		AssertFatal(program->GetType()==GPU_FRAGMENT_PROGRAM, "Material::SetFragmentProgram() : Invalid gpu program type");
		m_FragmentProgram = program;
	}

	void Material::SaveToFile(const String& filename, bool outputDefault)
	{
		// 将材质保存到文件
		TiXmlDocument doc(filename.Data());
		TiXmlDeclaration decl("1.0", "UTF-8", "yes");
		doc.InsertEndChild(decl);

		TiXmlElement elem_mat("Material");
		elem_mat.SetAttribute("name", m_ResourceName.Data());

		// 属性写入xml文件，如果与默认值相等就不写入

		// 例：SAVE_MATERIAL_ATTRIB(Lighting, true) 展开为：
		//if ((GetLighting()!=true) || outputDefault) {
		//	TiXmlElement nameLighting(Lighting);
		//	TiXmlText valLighting(String(GetLighting()).Data());
		//	nameLighting.InsertEndChild(valLighting);
		//	elem_mat.InsertEndChild(nameLighting);
		//}

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

		SAVE_MATERIAL_ATTRIB(Blending, true);
		SAVE_MATERIAL_ATTRIB(SrcBlendFactor, BLEND_FACTOR_SRC_ALPHA);
		SAVE_MATERIAL_ATTRIB(DstBlendFactor, BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);

		for (int i=0; i<8; i++)
		{
			if (!m_TextureLayerEnabled[i]) continue;

			TiXmlElement elem_rs("TextureRenderState");
			TextureRenderState* rs = &m_TextureRenderState[i];

	#define SAVE_RENDER_STATE_ATTRIB(attr, member) \
			TiXmlElement name##attr(#attr); \
			TiXmlText val##attr(String(rs->member).Data()); \
			name##attr.InsertEndChild(val##attr); \
			elem_rs.InsertEndChild(name##attr);

			// 尝试保存纹理
			//if (rs->texture)
			//{
			//	// FIXME:
			//	SAVE_RENDER_STATE_ATTRIB(Texture, texture->GetName());
			//}
			//// 如果没有指定纹理，尝试保存纹理名称
			//else 
			if (rs->textureName!="")
			{
				TiXmlElement nameTexture("Texture");
				TiXmlText valTexture(rs->textureName.Data());
				nameTexture.InsertEndChild(valTexture);
				elem_rs.InsertEndChild(nameTexture);
			}
			// 如果都没有，就保存#empty标记
			else
			{
				TiXmlElement nameTexture("Texture");
				TiXmlText valTexture(String("#empty").Data());
				nameTexture.InsertEndChild(valTexture);
				elem_rs.InsertEndChild(nameTexture);
			}

			//SAVE_RENDER_STATE_ATTRIB(WrapType, wrapType);

			TiXmlElement nameWrapType("WrapType");
			TiXmlText valWrapType(WrapTypeToString(rs->wrapType).Data());
			nameWrapType.InsertEndChild(valWrapType);
			elem_rs.InsertEndChild(nameWrapType);

			SAVE_RENDER_STATE_ATTRIB(MinFilterType, minFilterType);
			SAVE_RENDER_STATE_ATTRIB(MagFilterType, magFilterType);
			SAVE_RENDER_STATE_ATTRIB(TextureGenMode, genMode);

			SAVE_RENDER_STATE_ATTRIB(TextureEnvMode, envMode);

			elem_mat.InsertEndChild(elem_rs);
		}

		doc.InsertEndChild(elem_mat);
		doc.SaveFile();
	}

	void Material::LoadAttrib(Material* material, const String& attrib, const String& val)
	{
		if (m_sFuncMap.find(attrib)!=m_sFuncMap.end())
			(*m_sFuncMap[attrib])(material, val);
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
		mat->SetDiffuse(val.ToColor4f());
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

	void ReadBlending(Material* mat, const String& val)
	{
		mat->SetBlending(val.ToBool());
	}

	void ReadSrcBlendFactor(Material* mat, const String& val)
	{
		mat->SetSrcBlendFactor((BlendFactor)val.ToInt());
	}

	void ReadDstBlendFactor(Material* mat, const String& val)
	{
		mat->SetDstBlendFactor((BlendFactor)val.ToInt());
	}

	void Material::LoadRenderState(TextureRenderState* rs, const String& attrib, const String& val)
	{
		if (m_sRSFuncMap.find(attrib)!=m_sRSFuncMap.end())
			(*m_sRSFuncMap[attrib])(rs, val);
	}

	void ReadRSTexture(TextureRenderState* rs, const String& val)
	{
		// 读取材质名称并保存，当资源被触发时载入材质
		rs->textureName = val;
	}

	void ReadRSWrapType(TextureRenderState* rs, const String& val)
	{
		//rs->wrapType = (TextureWrapType)val.ToInt();
		rs->wrapType = StringToWrapType(val);
	}

	void ReadRSMinFilterType(TextureRenderState* rs, const String& val)
	{
		rs->minFilterType = (TextureFilterType)val.ToInt();
	}

	void ReadRSMagFilterType(TextureRenderState* rs, const String& val)
	{
		rs->magFilterType = (TextureFilterType)val.ToInt();
	}

	void ReadRSGenMode(TextureRenderState* rs, const String& val)
	{
		rs->genMode = (TextureGenMode)val.ToInt();
	}

	void ReadRSEnvMode(TextureRenderState* rs, const String& val)
	{
		rs->envMode = (TextureEnvMode)val.ToInt();
	}

}
