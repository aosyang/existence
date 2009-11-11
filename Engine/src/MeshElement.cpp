//-----------------------------------------------------------------------------------
/// MeshElement.cpp 网格元素实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "MeshElement.h"
#include "Engine.h"

namespace Gen
{
	MeshElement::MeshElement()
		: m_FaceArray(0), m_VertexArray(0), m_NormalArray(0), m_TexCoordArray(0),
		m_VertexNum(0), m_FaceNum(0)
	{
		m_VertexBuffer = renderer->BuildVertexBuffer();
	}

	MeshElement::~MeshElement()
	{
		ClearData();

		SAFE_DELETE(m_VertexBuffer)
	}

	void MeshElement::CreateMeshData(unsigned int vertexNum,
		unsigned int faceNum,
		unsigned int* faceArray,
		float* vertexArray,
		float* normalArray,
		float* texcoordArray)
	{
		ClearData();

		m_VertexNum = vertexNum;
		m_FaceNum = faceNum;

		m_FaceArray = new unsigned int[m_FaceNum * 3];
		memcpy(m_FaceArray, faceArray, sizeof(unsigned int) * m_FaceNum * 3);

		m_VertexArray = new float[m_VertexNum * 3];
		memcpy(m_VertexArray, vertexArray, sizeof(float) * m_VertexNum * 3);

		m_NormalArray = new float[m_VertexNum * 3];
		memcpy(m_NormalArray, normalArray, sizeof(float) * m_VertexNum * 3);

		m_TexCoordArray = new float[m_VertexNum * 2];
		memcpy(m_TexCoordArray, texcoordArray, sizeof(float) * m_VertexNum * 2);

		BuildVertexBufferData();
		UpdateOBB();
	}

	void MeshElement::CreateMeshData(MeshElementBuilder* builder)
	{
		ClearData();

		m_VertexNum = builder->m_VertexCount;
		AssertFatal(builder->m_IndexCount%3==0, "MeshElement::CreateMeshData(): Invalid index count! Must be times of 3.");
		m_FaceNum = builder->m_IndexCount / 3;

		m_FaceArray = new unsigned int[builder->m_IndexCount];
		memcpy(m_FaceArray, builder->m_Indices, sizeof(unsigned int) * builder->m_IndexCount);

		m_VertexArray = new float[builder->m_VertexCount * 3];
		memcpy(m_VertexArray,builder->m_Vertices, sizeof(float) * 3 * builder->m_VertexCount);

		m_NormalArray = new float[builder->m_VertexCount * 3];
		memcpy(m_NormalArray, builder->m_Normals, sizeof(float) * 3 * builder->m_VertexCount);

		m_TexCoordArray = new float[builder->m_VertexCount * 2];
		memcpy(m_TexCoordArray, builder->m_Texcoords, sizeof(float) * 2 * builder->m_VertexCount);

		BuildVertexBufferData();
		m_OBB = builder->m_OBB;
	}

	void MeshElement::ClearData()
	{
		SAFE_DELETE_ARRAY(m_FaceArray)
			SAFE_DELETE_ARRAY(m_VertexArray)
			SAFE_DELETE_ARRAY(m_NormalArray)
			SAFE_DELETE_ARRAY(m_TexCoordArray)
	}

	void MeshElement::UpdateOBB()
	{
		if (!m_VertexNum)
			return ;

		m_OBB.SetPoint(Vector3f(m_VertexArray));

		// 遍历所有定点，扩充obb的范围
		for (unsigned int i=1; i<m_VertexNum; i++)
			m_OBB.Expand(Vector3f(&m_VertexArray[i*3]));
	}

	void MeshElement::BuildVertexBufferData()
	{
		m_VertexBuffer->Clear();
		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0,
			m_VertexArray, m_NormalArray,
			NULL,
			m_TexCoordArray,
			m_VertexNum,
			m_FaceArray,
			m_FaceNum);
	}



	MeshElementBuilder::MeshElementBuilder()
		: m_VertexCaps(256), m_VertexCount(0),
		m_IndexCaps(250), m_IndexCount(0)
	{
		m_Indices = (unsigned int*)malloc(sizeof(unsigned int) * 256);

		m_Vertices = (float*)malloc(sizeof(float) * 3  * 256);
		m_Normals = (float*)malloc(sizeof(float) * 3  * 256);
		m_Texcoords = (float*)malloc(sizeof(float) * 2  * 256);
	}

	MeshElementBuilder::~MeshElementBuilder()
	{
		free(m_Indices);
		free(m_Vertices);
		free(m_Normals);
		free(m_Texcoords);
	}

	void MeshElementBuilder::AddVertex(float pos[3], float normal[3], float texcoord[2])
	{
		unsigned int index;
		int i;
		for (i=0; i<m_VertexCount; i++)
		{
			if (Vector3f(pos)==Vector3f(&m_Vertices[i*3]) &&
				Vector3f(normal)==Vector3f(&m_Normals[i*3]) &&
				FLOAT_EQUAL(texcoord[0], m_Texcoords[i*2]) &&
				FLOAT_EQUAL(texcoord[1], m_Texcoords[i*2+1]))
			{
				index = i;
				break;
			}
		}

		if (i==m_VertexCount)
		{
			memcpy(&m_Vertices[m_VertexCount * 3], pos, sizeof(float) * 3);
			memcpy(&m_Normals[m_VertexCount * 3], normal, sizeof(float) * 3);
			memcpy(&m_Texcoords[m_VertexCount * 2], texcoord, sizeof(float) * 2);

			m_VertexCount++;
			if (m_VertexCount>=m_VertexCaps)
			{
				m_VertexCaps = m_VertexCaps << 1;
				m_Vertices = (float*)realloc(m_Vertices, sizeof(float) * 3 * m_VertexCaps);
				m_Normals = (float*)realloc(m_Normals, sizeof(float) * 3 * m_VertexCaps);
				m_Texcoords = (float*)realloc(m_Texcoords, sizeof(float) * 2 * m_VertexCaps);
			}

			m_OBB.Expand(Vector3f(pos));
		}
		AddIndex(i);
	}

	void MeshElementBuilder::AddIndex(unsigned int index)
	{
		m_Indices[m_IndexCount] = index;

		m_IndexCount++;
		if (m_IndexCount>=m_IndexCaps)
		{
			m_IndexCaps = m_IndexCaps << 1;
			m_Indices = (unsigned int*)realloc(m_Indices, sizeof(unsigned int) * 3 * m_IndexCaps);
		}
	}
}
