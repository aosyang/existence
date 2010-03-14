//-----------------------------------------------------------------------------------
/// Material.h 材质类，描述了基本材质属性
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

	/// @brief
	///	材质类
	/// @par
	///		材质类记录模型表面的材质信息，包括受光属性、使用的纹理信息等
	class Material : public Resource
	{
		//friend class ResourceManager<Material>;
		friend class MaterialManager;
	public:
		// ----- Overwrite Resource

		/// @copydoc Resource::Trigger()
		bool Trigger();

		// ----- Material Methods

		// 材质颜色(仅在不受光照影响情况下有效)

		/// @brief
		/// 指定材质颜色
		/// @remarks
		///		当材质的光照属性关闭时，所有填充颜色将会被这个颜色取代。开启光照时这个属性无效
		inline void SetColor(const Color4f& color) { m_Color = color; }
		/// @brief
		///	获取材质颜色
		inline const Color4f& GetColor() const { return m_Color; }


		/// @brief
		/// 指定材质是否受到光照影响
		inline void SetLighting(bool lighting) { m_Lighting = lighting; }
		/// @brief
		/// 获取材质当前是否受到光照影响
		inline bool GetLighting() const { return m_Lighting; }

		/// @brief
		/// 指定材质的环境光颜色
		inline void SetAmbient(const Color4f& color) { m_Ambient = color; }
		/// @brief
		/// 获取材质的环境光颜色
		inline const Color4f& GetAmbient() const { return m_Ambient; }

		/// @brief
		/// 指定材质的漫反射颜色
		inline void SetDiffuse(const Color4f& color) { m_Diffuse = color; }
		/// @brief
		/// 获取材质的漫反射颜色
		inline const Color4f& GetDiffuse() const { return m_Diffuse; }

		/// @brief
		/// 指定材质的高光颜色
		inline void SetSpecular(const Color4f& color) { m_Specular = color; }
		/// @brief
		/// 获取材质的高光颜色
		inline const Color4f& GetSpecular() const { return m_Specular; }

		/// @brief
		/// 指定材质的自发光颜色
		inline void SetEmissive(const Color4f& color) { m_Emissive = color; }
		/// @brief
		/// 获取材质的自发光颜色
		inline const Color4f& GetEmissive() const { return m_Emissive; }


		/// @brief
		/// 指定材质的高光级别
		inline void SetSpecularLevel(float level) { m_SpecularLevel = level; }
		/// @brief
		/// 获取材质的高光级别
		inline float GetSpecularLevel() const { return m_SpecularLevel; }

		// 材质
		//inline void SetTexture(BaseTexture* tex) { m_Texture = tex; }
		//inline BaseTexture* GetTexture() const { return m_Texture; }

		/// @brief
		/// 指定某一个纹理层所使用的纹理
		/// @param tex
		///		要设定的纹理
		/// @param index
		///		要指定的纹理层索引
		void SetTexture(BaseTexture* tex, unsigned int index = 0);
		
		/// @brief
		/// 获取指定纹理层的纹理渲染状态
		/// @param index
		///		要获取的纹理层索引
		/// @returns
		///		指向纹理状态的指针
		inline TextureRenderState* GetTextureRenderState(unsigned int index = 0) { return &m_TextureRenderState[index]; }

		// 指定纹理层是否启用

		/// @brief
		///	启用/关闭某一个纹理层
		/// @param index
		///		要设置的纹理层索引
		/// @param enable
		///		启用或关闭纹理层
		inline void SetTextureLayerEnabled(unsigned int index, bool enable) { m_TextureLayerEnabled[index] = enable;}

		/// @brief
		/// 获取某一个纹理层是否已经启用
		/// @param index
		///		要获取状态的纹理层索引
		///	@returns
		///		如果指定纹理层已启用，返回true；否则返回false
		inline bool GetTextureLayerEnabled(unsigned int index = 0) const { return m_TextureLayerEnabled[index]; }


		// 深度缓冲写入控制

		/// @brief
		///	指定材质是否写入深度缓冲
		/// @remarks
		///		默认值为true
		/// @par
		///		如果设为false，当使用这个材质渲染多边形时，多边形不会写入深度缓冲
		inline void SetDepthWriting(bool writing) { m_DepthWriting = writing; }
		/// @brief
		///	获取材质是否写入深度缓冲
		inline bool GetDepthWriting() const { return m_DepthWriting; }

		// 深度测试控制

		/// @brief
		/// 指定材质是否进行深度测试
		/// @remarks
		///		默认值为true
		/// @par
		///		如果要写入缓冲区的象素深度比较失败，则该象素不会对任何缓冲区造成修改
		inline void SetDepthTest(bool test) { m_DepthTest = test; }
		/// @brief
		/// 获取材质是否进行深度测试
		inline bool GetDepthTest() const { return m_DepthTest; }

		// Alpha test
		/// @brief
		/// 指定材质是否进行Alpha测试
		/// @remarks
		///		默认值为false
		inline void SetAlphaTest(bool test) { m_AlphaTest = test; }
		/// @brief
		///	获取材质是否进行Alpha测试
		inline bool GetAlphaTest() const { return m_AlphaTest; }

		/// @brief
		/// 指定材质的Alpha参考值
		inline void SetAlphaRef(float ref) { m_AlphaReference = ref; }
		/// @brief
		/// 获取材质的Alpha参考值
		inline float GetAlphaRef() const { return m_AlphaReference; }

		/// @brief
		/// 指定材质是否进行颜色混合
		inline void SetBlending(bool blend) { m_Blending = blend; }
		/// @brief
		/// 获取材质是否进行颜色混合
		inline bool GetBlending() const { return m_Blending; }

		/// @brief
		/// 指定颜色混合的源混合因子(写入颜色)
		inline void SetSrcBlendFactor(BlendFactor factor) { m_SrcBlendFactor = factor; }
		/// @brief
		/// 获取颜色混合的源混合因子
		inline BlendFactor GetSrcBlendFactor() const { return m_SrcBlendFactor; }

		/// @brief
		/// 指定颜色混合的目标混合因子(缓冲区颜色)
		inline void SetDstBlendFactor(BlendFactor factor) { m_DstBlendFactor = factor; }
		/// @brief
		/// 获取颜色混合的目标混合因子
		inline BlendFactor GetDstBlendFactor() const { return m_DstBlendFactor; }

		inline void SetVertexProgram(IGpuProgram* program) { m_VertexProgram = program; }
		inline IGpuProgram* GetVertexProgram() { return m_VertexProgram; }

		inline void SetFragmentProgram(IGpuProgram* program) { m_FragmentProgram = program; }
		inline IGpuProgram* GetFragmentProgram() { return m_FragmentProgram; }

		// 从文件读取材质
		static MaterialLoadFuncMap	m_sFuncMap;
		static RenderStateLoadFuncMap m_sRSFuncMap;

		/// @brief
		/// 将材质保存到文件中
		/// @param filename
		///		材质保存的文件名
		/// @param outputDefault
		///		是否输出默认材质属性值(设置为false将会减少输出材质脚本占据的空间)
		void SaveToFile(const String& filename, bool outputDefault = false);

	protected:
		// ----- Overwrite Resource
		/// @copydoc Resource::LoadImpl()
		bool LoadImpl();
		/// @copydoc Resource::UnloadImpl()
		void UnloadImpl();

		// ----- Material Methods
		// 确保只有ResMgr可以创建实例

		/// @copydoc Resource::Resource(const String&)
		Material(const String& filename);
		~Material();

		/// @brief
		/// 读取材质属性
		/// @param material
		///		要设置属性的材质
		/// @param attrib
		///		要设置的属性名称
		/// @param val
		///		要设置的属性值
		/// @remarks
		///		这个方法由Material类在从文件读取材质信息时使用
		static void LoadAttrib(Material* material, const String& attrib, const String& val);

		/// @brief
		/// 读取纹理渲染状态
		/// @param rs
		///		要设置属性的纹理渲染状态
		/// @param attrib
		///		要设置的属性名称
		///	@param val
		///		要设置的属性值
		/// @remarks
		///		这个方法由Material类在从文件读取材质信息时使用
		static void LoadRenderState(TextureRenderState* rs, const String& attrib, const String& val);

	private:
		Color4f			m_Color;			///< 材质颜色

		bool			m_Lighting;			///< 材质是否受光照影响

		Color4f			m_Ambient;			///< 环境光颜色
		Color4f			m_Diffuse;			///< 漫反射颜色
		Color4f			m_Specular;			///< 高光颜色
		Color4f			m_Emissive;			///< 自发光颜色

		float			m_SpecularLevel;	///< 高光级别

		TextureRenderState	m_TextureRenderState[8];
		bool			m_TextureLayerEnabled[8];

		bool			m_DepthWriting;		///< 是否写入深度缓冲
		bool			m_DepthTest;		///< 是否进行深度测试

		bool			m_AlphaTest;		///< Alpha测试
		float			m_AlphaReference;	///< Alpha参考值

		// Blending
		bool			m_Blending;			///< 是否使用颜色混合
		BlendFactor		m_SrcBlendFactor;	///< 源混合因子
		BlendFactor		m_DstBlendFactor;	///< 目标混合因子

		IGpuProgram*	m_VertexProgram;
		IGpuProgram*	m_FragmentProgram;
	};

	/// @brief
	///	材质管理器
	/// @par
	///		引擎中所有材质的容器，使用管理器创建材质后，材质将于管理器销毁时一同卸载
	/// @see
	///		ResourceManagerBase
	class MaterialManager : public Singleton<MaterialManager>, public ResourceManagerBase
	{
		friend class Singleton<MaterialManager>;
	public:
		/// @copydoc ResourceManagerBase::CreateResourceHandles()
		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		// 创建一个新的材质
		/// @brief
		/// 创建一个新的材质
		/// @param resName
		///		新材质使用的资源名称
		/// @returns
		///		返回成功创建的材质指针
		/// @remarks
		///		资源名称可以留空，管理器将会自动分配
		Material* Create(const String& resName="");

		/// @brief
		/// 获取指定资源名称的材质
		/// @param resName
		///		要获取的材质名称
		/// @returns
		///		如果找到指定名称资源，返回资源指针；否则返回NULL
		Material* GetByName(const String& resName);
	protected:
		MaterialManager();
	};
}

#endif
