//-----------------------------------------------------------------------------------
/// MeshElement.h ����Ԫ���࣬������е�һ���ʵ�������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MESHELEMENT_H
#define _MESHELEMENT_H

#include "Vector3f.h"
#include "Debug.h"
#include "Material.h"
#include "OBB.h"
#include "IVertexBuffer.h"

#include <fstream>



namespace Gen
{
	//class MeshElementBuilder;

	class BaseMesh;

	class MeshElement
	{
		friend class BaseMesh;
	public:
		MeshElement(BaseMesh* mesh);
		~MeshElement();

		// �����ݴ���ģ��
		void CreateMeshData(unsigned int faceNum,
							unsigned int* faceArray);

		//void CreateMeshData(MeshElementBuilder* builder);

		void ClearData();

		// ͨ��������ȡ������
		void GetFaceByIndex(unsigned int index, unsigned int& x, unsigned int& y, unsigned int& z) const
		{
			AssertFatal(index<m_FaceNum, "MeshElement::GetFaceByIndex() : Index out of boundary.");

			x = m_FaceArray[index*3];
			y = m_FaceArray[index*3+1];
			z = m_FaceArray[index*3+2];
		}

		unsigned int GetFaceNum() const { return m_FaceNum; }

		inline IIndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; }

		OBB& GetOBB() { return m_OBB; }

	private:
		// �������㣬����ģ�͵İ�Χ����Ϣ
		void UpdateOBB();

		// ʹ�õ�ǰ���ݹ��춥�㻺��
		void BuildVertexBufferData();

	private:
		unsigned int*	m_FaceArray;		///< ����������

		//float*			m_VertexArray;		///< ��������
		//float*			m_NormalArray;		///< ��������
		//float*			m_TexCoordArray;	///< ������������

		unsigned int	m_FaceNum;

		BaseMesh*		m_Mesh;				///< ָ��mesh��ָ�룬���ڻ�ȡ������Ϣ
		IIndexBuffer*	m_IndexBuffer;
		OBB				m_OBB;
	};


	//// ģ��Ԫ�ع�����
	//// ͨ��һ��һ����Ӷ���ķ�ʽ����ģ��Ԫ��
	//// �Զ����ɶ�������
	////
	//// ע�⣺�������������������Ĺ��̿��ܻ����������
	//class MeshElementBuilder
	//{
	//	friend class MeshElement;
	//public:
	//	MeshElementBuilder();
	//	~MeshElementBuilder();

	//	void AddVertex(float pos[3], float normal[3], float texcoord[2]);
	//private:
	//	void AddIndex(unsigned int index);
	//private:
	//	unsigned int*	m_Indices;
	//	float*			m_Vertices;
	//	float*			m_Normals;
	//	float*			m_Texcoords;

	//	int				m_VertexCaps;
	//	int				m_VertexCount;
	//	int				m_IndexCaps;
	//	int				m_IndexCount;

	//	OBB				m_OBB;
	//};

}

#endif

