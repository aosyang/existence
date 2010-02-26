//-----------------------------------------------------------------------------------
/// Material.h �����࣬�����˻�����������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Texture.h"
#include "Color4f.h"
#include "ResourceManager.h"
#include <map>

using namespace std;

namespace Gen
{
	class Material;

	typedef void(*SetMaterialAttribFunc)(Material*, const String&);
	typedef map<const String, SetMaterialAttribFunc> MaterialLoadFuncMap;

	typedef void(*SetRenderStateAttribFunc)(TextureRenderState*, const String&);
	typedef map<const String, SetRenderStateAttribFunc> RenderStateLoadFuncMap;

	class MaterialManager;

	class Material : public Resource
	{
		//friend class ResourceManager<Material>;
		friend class MaterialManager;
	public:
		// ----- Overwrite Resource
		bool Trigger();

		// ----- Material Methods

		// ������ɫ(���ڲ��ܹ���Ӱ���������Ч)
		inline void SetColor(const Color4f& color) { m_Color = color; }
		inline const Color4f& GetColor() const { return m_Color; }

		// �Ƿ��ܵ�����Ӱ��
		inline void SetLighting(bool lighting) { m_Lighting = lighting; }
		inline bool GetLighting() const { return m_Lighting; }

		// ��������ɫ
		inline void SetAmbient(const Color4f& color) { m_Ambient = color; }
		inline const Color4f& GetAmbient() const { return m_Ambient; }

		// ��������ɫ
		inline void SetDiffuse(const Color4f& color) { m_Diffuse = color; }
		inline const Color4f& GetDiffuse() const { return m_Diffuse; }

		// �߹���ɫ
		inline void SetSpecular(const Color4f& color) { m_Specular = color; }
		inline const Color4f& GetSpecular() const { return m_Specular; }

		// �Է�����ɫ
		inline void SetEmissive(const Color4f& color) { m_Emissive = color; }
		inline const Color4f& GetEmissive() const { return m_Emissive; }

		// �߹⼶��
		inline void SetSpecularLevel(float level) { m_SpecularLevel = level; }
		inline float GetSpecularLevel() const { return m_SpecularLevel; }

		// ����
		//inline void SetTexture(BaseTexture* tex) { m_Texture = tex; }
		//inline BaseTexture* GetTexture() const { return m_Texture; }

		void SetTexture(BaseTexture* tex, unsigned int index = 0);
		//inline BaseTexture* GetTexture() const { return m_TextureRenderState.texture; }
		inline TextureRenderState* GetTextureRenderState(unsigned int index = 0) { return &m_TextureRenderState[index]; }

		// ָ��������Ƿ�����
		inline void SetTextureLayerEnabled(unsigned int index, bool enable) { m_TextureLayerEnabled[index] = enable;}
		inline bool GetTextureLayerEnabled(unsigned int index = 0) const { return m_TextureLayerEnabled[index]; }


		// ��Ȼ���д�����
		inline void SetDepthWriting(bool writing) { m_DepthWriting = writing; }
		inline bool GetDepthWriting() const { return m_DepthWriting; }

		// ��Ȳ��Կ���
		inline void SetDepthTest(bool test) { m_DepthTest = test; }
		inline bool GetDepthTest() const { return m_DepthTest; }

		// Alpha test
		inline void SetAlphaTest(bool test) { m_AlphaTest = test; }
		inline bool GetAlphaTest() const { return m_AlphaTest; }

		inline void SetAlphaRef(float ref) { m_AlphaReference = ref; }
		inline float GetAlphaRef() const { return m_AlphaReference; }

		inline void SetBlending(bool blend) { m_Blending = blend; }
		inline bool GetBlending() const { return m_Blending; }

		inline void SetSrcBlendFactor(BlendFactor factor) { m_SrcBlendFactor = factor; }
		inline BlendFactor GetSrcBlendFactor() const { return m_SrcBlendFactor; }

		inline void SetDstBlendFactor(BlendFactor factor) { m_DstBlendFactor = factor; }
		inline BlendFactor GetDstBlendFactor() const { return m_DstBlendFactor; }

		inline void SetVertexProgram(IGpuProgram* program) { m_VertexProgram = program; }
		inline IGpuProgram* GetVertexProgram() { return m_VertexProgram; }

		inline void SetFragmentProgram(IGpuProgram* program) { m_FragmentProgram = program; }
		inline IGpuProgram* GetFragmentProgram() { return m_FragmentProgram; }

		// ���ļ���ȡ����
		static MaterialLoadFuncMap	m_sFuncMap;
		static RenderStateLoadFuncMap m_sRSFuncMap;

		void SaveToFile(const String& filename, bool outputDefault = false);

	protected:
		// ----- Overwrite Resource
		bool LoadImpl();
		void UnloadImpl();

		// ----- Material Methods
		// ȷ��ֻ��ResMgr���Դ���ʵ��
		Material(const String& filename);
		~Material();

		static void LoadAttrib(Material* material, const String& attrib, const String& val);
		static void LoadRenderState(TextureRenderState* rs, const String& attrib, const String& val);

	private:
		Color4f			m_Color;			///< ������ɫ

		bool			m_Lighting;			///< �����Ƿ��ܹ���Ӱ��

		Color4f			m_Ambient;			///< ��������ɫ
		Color4f			m_Diffuse;			///< ��������ɫ
		Color4f			m_Specular;			///< �߹���ɫ
		Color4f			m_Emissive;			///< �Է�����ɫ

		float			m_SpecularLevel;	///< �߹⼶��

		TextureRenderState	m_TextureRenderState[8];
		bool			m_TextureLayerEnabled[8];

		bool			m_DepthWriting;		///< �Ƿ�д����Ȼ���
		bool			m_DepthTest;		///< �Ƿ������Ȳ���

		bool			m_AlphaTest;
		float			m_AlphaReference;

		// Blending
		bool			m_Blending;			///< �Ƿ�ʹ����ɫ���
		BlendFactor		m_SrcBlendFactor;	///< Դ�������
		BlendFactor		m_DstBlendFactor;	///< Ŀ��������

		IGpuProgram*	m_VertexProgram;
		IGpuProgram*	m_FragmentProgram;
	};

	class MaterialManager : public Singleton<MaterialManager>, public ResourceManagerBase
	{
		friend class Singleton<MaterialManager>;
	public:
		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		// ����һ���µĲ���
		Material* Create(const String& resName="");

		Material* GetByName(const String& resName);
	protected:
		MaterialManager();
	};
}

#endif
