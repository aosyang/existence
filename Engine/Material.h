//-----------------------------------------------------------------------------------
/// Material.h 材质类，描述了基本材质属性
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
#include <map>

using namespace std;

class Material;

//template class ResourceManager<Material>;
ResourceManager<Material>;

typedef void(*SetMaterialAttribFunc)(Material*, const String&);
typedef map<const String, SetMaterialAttribFunc> MaterialLoadFuncMap;

typedef void(*SetRenderStateAttribFunc)(texRenderState_t*, const String&);
typedef map<const String, SetRenderStateAttribFunc> RenderStateLoadFuncMap;


class Material
{
	friend class ResourceManager<Material>;
public:

	inline const String GetName() const { return *m_Name; }

	// 材质颜色(仅在不受光照影响情况下有效)
	inline void SetColor(const Color4f& color) { m_Color = color; }
	inline const Color4f& GetColor() const { return m_Color; }

	// 是否受到光照影响
	inline void SetLighting(bool lighting) { m_Lighting = lighting; }
	inline bool GetLighting() const { return m_Lighting; }

	// 环境光颜色
	inline void SetAmbient(const Color4f& color) { m_Ambient = color; }
	inline const Color4f& GetAmbient() const { return m_Ambient; }

	// 漫反射颜色
	inline void SetDiffuse(const Color4f& color) { m_Diffuse = color; }
	inline const Color4f& GetDiffuse() const { return m_Diffuse; }

	// 高光颜色
	inline void SetSpecular(const Color4f& color) { m_Specular = color; }
	inline const Color4f& GetSpecular() const { return m_Specular; }

	// 自发光颜色
	inline void SetEmissive(const Color4f& color) { m_Emissive = color; }
	inline const Color4f& GetEmissive() const { return m_Emissive; }

	// 高光级别
	inline void SetSpecularLevel(float level) { m_SpecularLevel = level; }
	inline float GetSpecularLevel() const { return m_SpecularLevel; }

	// 材质
	//inline void SetTexture(ITexture* tex) { m_Texture = tex; }
	//inline ITexture* GetTexture() const { return m_Texture; }

	inline void SetTexture(ITexture* tex, unsigned int index = 0) { m_TextureRenderState[index].texture = tex; }
	//inline ITexture* GetTexture() const { return m_TextureRenderState.texture; }
	inline texRenderState* GetTextureRenderState(unsigned int index = 0) { return &m_TextureRenderState[index]; }

	// 指定纹理层是否启用
	inline void SetTextureLayerEnabled(unsigned int index, bool enable) { m_TextureLayerEnabled[index] = enable;}
	inline bool GetTextureLayerEnabled(unsigned int index = 0) const { return m_TextureLayerEnabled[index]; }


	// 深度缓冲写入控制
	inline void SetDepthWriting(bool writing) { m_DepthWriting = writing; }
	inline bool GetDepthWriting() const { return m_DepthWriting; }

	// 深度测试控制
	inline void SetDepthTest(bool test) { m_DepthTest = test; }
	inline bool GetDepthTest() const { return m_DepthTest; }

	// TODO: Alpha test;
	inline void SetAlphaTest(bool test) { m_AlphaTest = test; }
	inline bool GetAlphaTest() const { return m_AlphaTest; }

	inline void SetAlphaRef(float ref) { m_AlphaReference = ref; }
	inline float GetAlphaRef() const { return m_AlphaReference; }

	inline void SetVertexProgram(IGpuProgram* program) { m_VertexProgram = program; }
	inline IGpuProgram* GetVertexProgram() { return m_VertexProgram; }

	inline void SetFragmentProgram(IGpuProgram* program) { m_FragmentProgram = program; }
	inline IGpuProgram* GetFragmentProgram() { return m_FragmentProgram; }

	// 从文件读取材质
	static Material* LoadMaterial(const String& filename);
	static MaterialLoadFuncMap	s_FuncMap;
	static RenderStateLoadFuncMap s_RSFuncMap;

	void SaveToFile(const String& filename, bool outputDefault = false);

private:
	// 声明为private，这样只有ResMgr可以创建实例
	Material();
	~Material();

	static void LoadAttrib(Material* material, const String& attrib, const String& val);
	static void LoadRenderState(texRenderState* rs, const String& attrib, const String& val);

private:
	const String*	m_Name;				///< 材质名称指针
	Color4f			m_Color;			///< 材质颜色

	bool			m_Lighting;			///< 材质是否受光照影响

	Color4f			m_Ambient;			///< 环境光颜色
	Color4f			m_Diffuse;			///< 漫反射颜色
	Color4f			m_Specular;			///< 高光颜色
	Color4f			m_Emissive;			///< 自发光颜色

	float			m_SpecularLevel;	///< 高光级别

	texRenderState_t	m_TextureRenderState[8];
	bool			m_TextureLayerEnabled[8];
	// TODO: 这里ITexture换成texRenderState_t
	//ITexture*	m_Texture;			///< 材质使用的纹理贴图

	bool			m_DepthWriting;		///< 是否写入深度缓冲(FIXME: 不正确，参见PluginGLRenderSystem)
	bool			m_DepthTest;		///< 是否进行深度测试

	bool			m_AlphaTest;
	float			m_AlphaReference;

	IGpuProgram*	m_VertexProgram;
	IGpuProgram*	m_FragmentProgram;
};

#endif
