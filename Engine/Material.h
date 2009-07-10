//-----------------------------------------------------------------------------------
/// Material.h �����࣬�����˻�����������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "ITexture.h"
#include "Color4f.h"
#include "ResourceManager.h"

class Material;

ResourceManager<Material>;

class Material
{
	friend class ResourceManager<Material>;
public:

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
	//inline void SetTexture(ITexture* tex) { m_Texture = tex; }
	//inline ITexture* GetTexture() const { return m_Texture; }

	inline void SetTexture(ITexture* tex, unsigned int index = 0) { m_TextureRenderState[index].texture = tex; }
	//inline ITexture* GetTexture() const { return m_TextureRenderState.texture; }
	inline texRenderState* GetTextureRenderState(unsigned int index = 0) { return &m_TextureRenderState[index]; }

	// ָ��������Ƿ�����
	inline void SetTextureLayerEnabled(unsigned int index, bool enable) { m_TextureLayerEnabled[index] = enable;}
	inline bool GetTextureLayerEnabled(unsigned int index = 0) const { return m_TextureLayerEnabled[index]; }


	// ��Ȼ���д�����
	inline void SetDepthWriting(bool writing) { m_DepthWriting = writing; }
	inline bool GetDepthWriting() const { return m_DepthWriting; }

	// ��Ȳ��Կ���
	inline void SetDepthTest(bool test) { m_DepthTest = test; }
	inline bool GetDepthTest() const { return m_DepthTest; }

	// TODO: Alpha test;
	inline void SetAlphaTest(bool test) { m_AlphaTest = test; }
	inline bool GetAlphaTest() const { return m_AlphaTest; }

	inline void SetAlphaReference(float ref) { m_AlphaReference = ref; }
	inline float GetAlphaReference() const { return m_AlphaReference; }

private:
	// ����Ϊprivate������ֻ��ResMgr���Դ���ʵ��
	Material();
	~Material();

private:
	Color4f		m_Color;			///< ������ɫ

	bool		m_Lighting;			///< �����Ƿ��ܹ���Ӱ��

	Color4f		m_Ambient;			///< ��������ɫ
	Color4f		m_Diffuse;			///< ��������ɫ
	Color4f		m_Specular;			///< �߹���ɫ
	Color4f		m_Emissive;			///< �Է�����ɫ

	float		m_SpecularLevel;	///< �߹⼶��

	texRenderState_t	m_TextureRenderState[8];
	bool		m_TextureLayerEnabled[8];
	// TODO: ����ITexture����texRenderState_t
	//ITexture*	m_Texture;			///< ����ʹ�õ�������ͼ

	bool		m_DepthWriting;		///< �Ƿ�д����Ȼ���(FIXME: ����ȷ���μ�PluginGLRenderSystem)
	bool		m_DepthTest;		///< �Ƿ������Ȳ���

	bool		m_AlphaTest;
	float		m_AlphaReference;
};

#endif
