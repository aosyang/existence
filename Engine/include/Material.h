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

	/// @brief
	///	������
	/// @par
	///		�������¼ģ�ͱ���Ĳ�����Ϣ�������ܹ����ԡ�ʹ�õ�������Ϣ��
	class Material : public Resource
	{
		//friend class ResourceManager<Material>;
		friend class MaterialManager;
	public:
		// ----- Overwrite Resource

		/// @copydoc Resource::Trigger()
		bool Trigger();

		// ----- Material Methods

		// ������ɫ(���ڲ��ܹ���Ӱ���������Ч)

		/// @brief
		/// ָ��������ɫ
		/// @remarks
		///		�����ʵĹ������Թر�ʱ�����������ɫ���ᱻ�����ɫȡ������������ʱ���������Ч
		inline void SetColor(const Color4f& color) { m_Color = color; }
		/// @brief
		///	��ȡ������ɫ
		inline const Color4f& GetColor() const { return m_Color; }


		/// @brief
		/// ָ�������Ƿ��ܵ�����Ӱ��
		inline void SetLighting(bool lighting) { m_Lighting = lighting; }
		/// @brief
		/// ��ȡ���ʵ�ǰ�Ƿ��ܵ�����Ӱ��
		inline bool GetLighting() const { return m_Lighting; }

		/// @brief
		/// ָ�����ʵĻ�������ɫ
		inline void SetAmbient(const Color4f& color) { m_Ambient = color; }
		/// @brief
		/// ��ȡ���ʵĻ�������ɫ
		inline const Color4f& GetAmbient() const { return m_Ambient; }

		/// @brief
		/// ָ�����ʵ���������ɫ
		inline void SetDiffuse(const Color4f& color) { m_Diffuse = color; }
		/// @brief
		/// ��ȡ���ʵ���������ɫ
		inline const Color4f& GetDiffuse() const { return m_Diffuse; }

		/// @brief
		/// ָ�����ʵĸ߹���ɫ
		inline void SetSpecular(const Color4f& color) { m_Specular = color; }
		/// @brief
		/// ��ȡ���ʵĸ߹���ɫ
		inline const Color4f& GetSpecular() const { return m_Specular; }

		/// @brief
		/// ָ�����ʵ��Է�����ɫ
		inline void SetEmissive(const Color4f& color) { m_Emissive = color; }
		/// @brief
		/// ��ȡ���ʵ��Է�����ɫ
		inline const Color4f& GetEmissive() const { return m_Emissive; }


		/// @brief
		/// ָ�����ʵĸ߹⼶��
		inline void SetSpecularLevel(float level) { m_SpecularLevel = level; }
		/// @brief
		/// ��ȡ���ʵĸ߹⼶��
		inline float GetSpecularLevel() const { return m_SpecularLevel; }

		// ����
		//inline void SetTexture(BaseTexture* tex) { m_Texture = tex; }
		//inline BaseTexture* GetTexture() const { return m_Texture; }

		/// @brief
		/// ָ��ĳһ���������ʹ�õ�����
		/// @param tex
		///		Ҫ�趨������
		/// @param index
		///		Ҫָ�������������
		void SetTexture(BaseTexture* tex, unsigned int index = 0);
		
		/// @brief
		/// ��ȡָ��������������Ⱦ״̬
		/// @param index
		///		Ҫ��ȡ�����������
		/// @returns
		///		ָ������״̬��ָ��
		inline TextureRenderState* GetTextureRenderState(unsigned int index = 0) { return &m_TextureRenderState[index]; }

		// ָ��������Ƿ�����

		/// @brief
		///	����/�ر�ĳһ�������
		/// @param index
		///		Ҫ���õ����������
		/// @param enable
		///		���û�ر������
		inline void SetTextureLayerEnabled(unsigned int index, bool enable) { m_TextureLayerEnabled[index] = enable;}

		/// @brief
		/// ��ȡĳһ��������Ƿ��Ѿ�����
		/// @param index
		///		Ҫ��ȡ״̬�����������
		///	@returns
		///		���ָ������������ã�����true�����򷵻�false
		inline bool GetTextureLayerEnabled(unsigned int index = 0) const { return m_TextureLayerEnabled[index]; }


		// ��Ȼ���д�����

		/// @brief
		///	ָ�������Ƿ�д����Ȼ���
		/// @remarks
		///		Ĭ��ֵΪtrue
		/// @par
		///		�����Ϊfalse����ʹ�����������Ⱦ�����ʱ������β���д����Ȼ���
		inline void SetDepthWriting(bool writing) { m_DepthWriting = writing; }
		/// @brief
		///	��ȡ�����Ƿ�д����Ȼ���
		inline bool GetDepthWriting() const { return m_DepthWriting; }

		// ��Ȳ��Կ���

		/// @brief
		/// ָ�������Ƿ������Ȳ���
		/// @remarks
		///		Ĭ��ֵΪtrue
		/// @par
		///		���Ҫд�뻺������������ȱȽ�ʧ�ܣ�������ز�����κλ���������޸�
		inline void SetDepthTest(bool test) { m_DepthTest = test; }
		/// @brief
		/// ��ȡ�����Ƿ������Ȳ���
		inline bool GetDepthTest() const { return m_DepthTest; }

		// Alpha test
		/// @brief
		/// ָ�������Ƿ����Alpha����
		/// @remarks
		///		Ĭ��ֵΪfalse
		inline void SetAlphaTest(bool test) { m_AlphaTest = test; }
		/// @brief
		///	��ȡ�����Ƿ����Alpha����
		inline bool GetAlphaTest() const { return m_AlphaTest; }

		/// @brief
		/// ָ�����ʵ�Alpha�ο�ֵ
		inline void SetAlphaRef(float ref) { m_AlphaReference = ref; }
		/// @brief
		/// ��ȡ���ʵ�Alpha�ο�ֵ
		inline float GetAlphaRef() const { return m_AlphaReference; }

		/// @brief
		/// ָ�������Ƿ������ɫ���
		inline void SetBlending(bool blend) { m_Blending = blend; }
		/// @brief
		/// ��ȡ�����Ƿ������ɫ���
		inline bool GetBlending() const { return m_Blending; }

		/// @brief
		/// ָ����ɫ��ϵ�Դ�������(д����ɫ)
		inline void SetSrcBlendFactor(BlendFactor factor) { m_SrcBlendFactor = factor; }
		/// @brief
		/// ��ȡ��ɫ��ϵ�Դ�������
		inline BlendFactor GetSrcBlendFactor() const { return m_SrcBlendFactor; }

		/// @brief
		/// ָ����ɫ��ϵ�Ŀ��������(��������ɫ)
		inline void SetDstBlendFactor(BlendFactor factor) { m_DstBlendFactor = factor; }
		/// @brief
		/// ��ȡ��ɫ��ϵ�Ŀ��������
		inline BlendFactor GetDstBlendFactor() const { return m_DstBlendFactor; }

		inline void SetVertexProgram(IGpuProgram* program) { m_VertexProgram = program; }
		inline IGpuProgram* GetVertexProgram() { return m_VertexProgram; }

		inline void SetFragmentProgram(IGpuProgram* program) { m_FragmentProgram = program; }
		inline IGpuProgram* GetFragmentProgram() { return m_FragmentProgram; }

		// ���ļ���ȡ����
		static MaterialLoadFuncMap	m_sFuncMap;
		static RenderStateLoadFuncMap m_sRSFuncMap;

		/// @brief
		/// �����ʱ��浽�ļ���
		/// @param filename
		///		���ʱ�����ļ���
		/// @param outputDefault
		///		�Ƿ����Ĭ�ϲ�������ֵ(����Ϊfalse�������������ʽű�ռ�ݵĿռ�)
		void SaveToFile(const String& filename, bool outputDefault = false);

	protected:
		// ----- Overwrite Resource
		/// @copydoc Resource::LoadImpl()
		bool LoadImpl();
		/// @copydoc Resource::UnloadImpl()
		void UnloadImpl();

		// ----- Material Methods
		// ȷ��ֻ��ResMgr���Դ���ʵ��

		/// @copydoc Resource::Resource(const String&)
		Material(const String& filename);
		~Material();

		/// @brief
		/// ��ȡ��������
		/// @param material
		///		Ҫ�������ԵĲ���
		/// @param attrib
		///		Ҫ���õ���������
		/// @param val
		///		Ҫ���õ�����ֵ
		/// @remarks
		///		���������Material���ڴ��ļ���ȡ������Ϣʱʹ��
		static void LoadAttrib(Material* material, const String& attrib, const String& val);

		/// @brief
		/// ��ȡ������Ⱦ״̬
		/// @param rs
		///		Ҫ�������Ե�������Ⱦ״̬
		/// @param attrib
		///		Ҫ���õ���������
		///	@param val
		///		Ҫ���õ�����ֵ
		/// @remarks
		///		���������Material���ڴ��ļ���ȡ������Ϣʱʹ��
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

		bool			m_AlphaTest;		///< Alpha����
		float			m_AlphaReference;	///< Alpha�ο�ֵ

		// Blending
		bool			m_Blending;			///< �Ƿ�ʹ����ɫ���
		BlendFactor		m_SrcBlendFactor;	///< Դ�������
		BlendFactor		m_DstBlendFactor;	///< Ŀ��������

		IGpuProgram*	m_VertexProgram;
		IGpuProgram*	m_FragmentProgram;
	};

	/// @brief
	///	���ʹ�����
	/// @par
	///		���������в��ʵ�������ʹ�ù������������ʺ󣬲��ʽ��ڹ���������ʱһͬж��
	/// @see
	///		ResourceManagerBase
	class MaterialManager : public Singleton<MaterialManager>, public ResourceManagerBase
	{
		friend class Singleton<MaterialManager>;
	public:
		/// @copydoc ResourceManagerBase::CreateResourceHandles()
		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		// ����һ���µĲ���
		/// @brief
		/// ����һ���µĲ���
		/// @param resName
		///		�²���ʹ�õ���Դ����
		/// @returns
		///		���سɹ������Ĳ���ָ��
		/// @remarks
		///		��Դ���ƿ������գ������������Զ�����
		Material* Create(const String& resName="");

		/// @brief
		/// ��ȡָ����Դ���ƵĲ���
		/// @param resName
		///		Ҫ��ȡ�Ĳ�������
		/// @returns
		///		����ҵ�ָ��������Դ��������Դָ�룻���򷵻�NULL
		Material* GetByName(const String& resName);
	protected:
		MaterialManager();
	};
}

#endif
