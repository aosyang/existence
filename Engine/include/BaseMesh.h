//-----------------------------------------------------------------------------------
/// BaseMesh.h ����ģ�ͻ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _BASEMESH_H
#define _BASEMESH_H

#include "OBB.h"
#include "Resource.h"

#include <vector>



namespace Gen
{
	class IVertexBuffer;
	struct SkinnedVertexInfo;
	class Skeleton;
	class MeshElement;
	class MeshBuilder;
	class BaseMaterial;

	// TODO: ����һ��static mesh�࣬ʹ�����޷��޸ģ���Լ�ڴ�ռ��

	/// @brief
	/// ����ģ�ͻ���
	/// @par
	///		��һ������ģ�ͼ��غ�����ģ�ͻ����¼�����ģ�͵Ķ���������Ϣ�ԡ�����������ģ��
	///		�Լ���ģ��ʹ�õ���Ƥ��Ϣ��ͬʱ����һ�ݶ��㻺��������Ⱦ
	/// @remarks
	///		ģ�͵����������¼�ڸ���������ģ���У����㻺��ͳһ��¼����ģ�ͻ����С�
	///		���ڹ�������һ���Զ����ж�����б��Σ�ͬʱ��Ӱ��һ��ģ��ӵ�ж����ͬ���ʵ�������ģ�͵������
	class BaseMesh : public Resource
	{
		friend class MeshManager;
	public:
		/// @copydoc Resource::Resource(const String&)
		BaseMesh(const String& filename);
		~BaseMesh();

		// ----- BaseMesh Methods

		/// ��ȡ������ģ�͵ĸ���
		int GetElementCount() const;

		/// ͨ��������ȡ������ģ��
		MeshElement* GetElement(unsigned int index);

		// ʹ�õ�ǰ�������ɶ��㻺��
		// Note: ���Ŀǰ�������ڹ�������
		//void BuildVertexBuffer();

		///	@brief
		/// �Ե�ǰ�Ķ��㴴�����㻺��ĸ���
		/// @remarks
		///		����������ڹ�������ģ�ͣ����Ƴ��Ķ�������Ϊÿ��ģ�Ͳ���Ψһ���α�
		/// @see
		///		SkeletalMeshObject
		IVertexBuffer* BuildVertexBufferCopy();

		///	@brief
		/// ʹ��ģ�͹�������������ģ��
		/// @note
		///		�������ģ���Ѿ����أ������������ո�ģ�͵Ķ��㼰������ģ�͵�������Ϣ
		void BuildMesh(const MeshBuilder* builder);

		/// @brief
		/// ��ȡָ�������Ĳ���
		/// @param index
		///		�����ȡ�Ĳ�������
		/// @remarks
		///		������Ŀ����ģ����Ŀ��ͬ
		/// @returns
		///		�ɹ���ȡ���ʷ���ָ����ʵ�ָ�룬��������������ʷ�Χ�򷵻�NULL
		BaseMaterial* GetMaterial(unsigned int index);

		/// ��ȡģ�͵İ�Χ��뾶
		inline float GetBoundingRadius() const { return m_BoundingRadius; }

		/// ��ȡģ�͵�OBB
		inline const OBB& GetOBB() const { return m_OBB; }

		///	@brief
		/// ͨ��������ȡ����
		/// @param index
		///		��������
		Vector3f GetVertexByIndex(unsigned int index) const;

		// ��ȡ���������Խ����޸�

		/// ��ȡģ�͵Ķ������
		inline int GetVertexCount() const { return m_VertexNum; }

		/// ��ȡģ�͵Ķ�������
		inline float* GetVertexArray() { return m_VertexArray; }

		/// ��ȡģ�͵ķ�������
		inline float* GetNormalArray() { return m_NormalArray; }

		/// ��ȡģ�͵���ɫ����
		inline float* GetColorArray() { return m_ColorArray; }

		/// ��ȡģ�͵�������������
		inline float* GetTexCoordArray() { return m_TexcoordArray; }

		/// ��ȡģ�͵Ķ��㻺��
		inline IVertexBuffer* GetVertexBuffer() const { return m_VertexBuffer; }

		/// ��ȡģ�͵Ķ�����Ƥ��Ϣ
		inline SkinnedVertexInfo* GetSkin() const { return m_Skin; }
	protected:
		// ----- Overwrite Resource
		bool LoadImpl();
		void UnloadImpl();

		// ----- BaseMesh Methods

		/// ���ģ����Ԫ��
		void AddMeshElement(MeshElement* elem);

		/// ��������������Ԫ���Լ���ģ�������OBB
		void UpdateElementsOBB();

		/// �������������Ԫ��
		void ClearMeshElements();

	protected:
		float					m_BoundingRadius;		///< ��Χ��뾶
		OBB						m_OBB;					///< ��Χ�гߴ�

		std::vector<MeshElement*>	m_MeshElements;			///< ������Ԫ�ؼ���
		std::vector<BaseMaterial*>	m_Materials;			///< ������Ԫ�ز����б�

		int						m_VertexNum;			///< ������Ŀ
		float*					m_VertexArray;			///< ��������
		float*					m_NormalArray;			///< ��������
		float*					m_ColorArray;			///< ��ɫ����
		float*					m_TexcoordArray;		///< ������������
		IVertexBuffer*			m_VertexBuffer;			///< ģ�͵Ķ��㻺��

		SkinnedVertexInfo*		m_Skin;					///< ��Ƥ��Ϣ
	};

	class MeshElementBuilder;

	// Mesh������

	/// @brief
	/// ����ģ�͹�����
	/// @par
	///		����ģ�͹���������ͨ�������Ӷ���ķ�ʽ����һ������ģ��
	/// @remarks
	///		��ǰ�汾�Ĺ�����ʵ�ֿ��ܻ���Ч�����⣬Ӧ����������Ϸ�߼���ʹ��
	class MeshBuilder
	{
		friend class BaseMesh;
	public:
		/// ���캯��
		/// @param elementCount
		///		ָ�������������ģ�ͻ���Ϊ������ģ��
		MeshBuilder(unsigned int elementCount);
		~MeshBuilder();

		/// @brief
		/// ���һ������
		/// @param pos
		///		���������
		/// @param normal
		///		����ķ��߷���
		/// @param texcoord
		///		�������������
		/// @param meshElementIndex
		///		�ö�����������ģ�����
		/// @returns
		///		�����Ӷ���ɹ�������true�������������������ģ���������ޣ�����false�����Ե�ǰ���ݽ����޸�
		/// @remarks
		///		��Ӷ���󣬶�����������һ�����������������������������������ݲ���
		bool AddVertex(float pos[3], float normal[3], float texcoord[2], unsigned int meshElementIndex);
	private:

		/// @brief
		/// ���һ������
		/// @param index
		///		������
		/// @param meshElementIndex
		///		��������������ģ�����
		void AddIndex(unsigned int index, unsigned int meshElementIndex);
	private:
		float*			m_Vertices;				///< ��������
		float*			m_Normals;				///< ��������
		float*			m_Texcoords;			///< ������������

		int				m_VertexCaps;			///< ��������
		int				m_VertexCount;			///< ��ǰ��������

		unsigned int	m_MeshElementCount;		///< ������ģ������
		MeshElementBuilder*	m_MeshElements;		///< ������ģ�͹������б�

		OBB				m_OBB;					///< ��Χ��
	};

	/// @brief
	/// ������ģ�͹�����
	/// @remarks
	///		�����Ӧ����MeshBuilder�������
	class MeshElementBuilder
	{
		friend class BaseMesh;
		friend class MeshBuilder;
	public:
		/// @brief
		/// ���һ������
		/// @param index
		///		��ӵ�������
		/// @remarks
		///		����������Զ����ڲ������������
		void AddIndex(unsigned int index);

	private:
		/// @brief
		/// ���캯��
		/// @remarks
		///		��Ӧֱ�Ӵ���������MeshBuilder�������
		MeshElementBuilder();
		~MeshElementBuilder();

	private:
		unsigned int*	m_Indices;			///< ��������
		int				m_IndexCaps;		///< ��������
		int				m_IndexCount;		///< ��ǰ��������
	};
}

#endif

