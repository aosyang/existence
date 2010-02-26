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

using namespace std;

namespace Gen
{
	class IVertexBuffer;
	class Material;
	struct SkinnedVertexInfo;
	class Skeleton;
	class MeshElement;
	class MeshBuilder;

	// TODO: ����һ��static mesh�࣬ʹ�����޷��޸ģ���Լ�ڴ�ռ��
	class BaseMesh : public Resource
	{
		friend class MeshManager;
	public:
		BaseMesh(const String& filename);
		~BaseMesh();

		// ----- BaseMesh Methods

		int GetElementCount() const;
		MeshElement* GetElement(unsigned int index);

		// ʹ�õ�ǰ�������ɶ��㻺��
		// Note: ���Ŀǰ�������ڹ�������
		//void BuildVertexBuffer();

		// �Ե�ǰ�Ķ��㴴�����㻺��ĸ���
		IVertexBuffer* BuildVertexBufferCopy();

		// ʹ��ģ�͹���������ģ��
		void BuildMesh(const MeshBuilder* builder);

		Material* GetMaterial(unsigned int index);

		inline float GetBoundingRadius() const { return m_BoundingRadius; }
		inline const OBB& GetOBB() const { return m_OBB; }

		// ͨ��������ȡ����
		Vector3f GetVertexByIndex(unsigned int index) const;

		// ��ȡ���������Խ����޸�
		inline int GetVertexCount() const { return m_VertexNum; }
		inline float* GetVertexArray() { return m_VertexArray; }
		inline float* GetNormalArray() { return m_NormalArray; }
		inline float* GetColorArray() { return m_ColorArray; }
		inline float* GetTexCoordArray() { return m_TexcoordArray; }

		inline IVertexBuffer* GetVertexBuffer() const { return m_VertexBuffer; }

		inline SkinnedVertexInfo* GetSkin() const { return m_Skin; }
	protected:
		// ----- Overwrite Resource
		bool LoadImpl();
		void UnloadImpl();

		// ----- BaseMesh Methods

		// ���ģ����Ԫ��
		void AddMeshElement(MeshElement* elem);

		// ������������Ԫ�ص�obb
		void UpdateElementsOBB();

		// ���������Ԫ��
		void ClearMeshElements();

	protected:
		float					m_BoundingRadius;		///< ��Χ��뾶
		OBB						m_OBB;					///< ��Χ�гߴ�

		vector<MeshElement*>	m_MeshElements;			///< ��Ԫ�ؼ���
		vector<Material*>		m_Materials;

		int						m_VertexNum;			///< ������Ŀ
		float*					m_VertexArray;			///< ��������
		float*					m_NormalArray;			///< ��������
		float*					m_ColorArray;
		float*					m_TexcoordArray;
		IVertexBuffer*			m_VertexBuffer;			///< ģ�͵Ķ��㼯��

		SkinnedVertexInfo*		m_Skin;					///< ��Ƥ��Ϣ
	};

	class MeshElementBuilder;

	// Mesh������
	class MeshBuilder
	{
		friend class BaseMesh;
	public:
		MeshBuilder(unsigned int elementCount);
		~MeshBuilder();

		void AddVertex(float pos[3], float normal[3], float texcoord[2], unsigned int meshElementIndex);
	private:
		void AddIndex(unsigned int index, unsigned int meshElementIndex);
	private:
		float*			m_Vertices;
		float*			m_Normals;
		float*			m_Texcoords;

		int				m_VertexCaps;
		int				m_VertexCount;

		unsigned int	m_MeshElementCount;
		MeshElementBuilder*	m_MeshElements;

		OBB				m_OBB;
	};

	class MeshElementBuilder
	{
		friend class BaseMesh;
	public:
		MeshElementBuilder();
		~MeshElementBuilder();

		void AddIndex(unsigned int index);

	private:
		unsigned int*	m_Indices;
		int				m_IndexCaps;
		int				m_IndexCount;
	};
}

#endif

