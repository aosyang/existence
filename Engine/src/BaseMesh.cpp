//-----------------------------------------------------------------------------------
/// BaseMesh.h 网格模型基类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "BaseMesh.h"
#include "Engine.h"
#include "MeshElement.h"
#include "Renderer.h"
#include "Skeleton.h"

namespace Gen
{
	BaseMesh::BaseMesh(const String& filename)
		: Resource(filename),
		  m_BoundingRadius(0.0f),
		  m_VertexNum(0),
		  m_VertexArray(NULL),
		  m_NormalArray(NULL),
		  m_ColorArray(NULL),
		  m_TexcoordArray(NULL),
		  m_VertexBuffer(NULL),
		  m_Skin(NULL)
	{
	}

	BaseMesh::~BaseMesh()
	{
	}

	bool BaseMesh::LoadImpl()
	{
		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
		m_OBB.Reset();

		return true;
	}

	void BaseMesh::UnloadImpl()
	{
		ClearMeshElements();
		m_BoundingRadius = 0;

		SAFE_DELETE(m_VertexBuffer);

		// 删除蒙皮信息
		SAFE_DELETE_ARRAY(m_Skin);
	}

	int BaseMesh::GetElementCount() const
	{
		return (int)m_MeshElements.size();
	}

	//void BaseMesh::BuildVertexBuffer()
	//{
	//	m_VertexBuffer->Clear();

	//	// TODO: 顶点格式保存下来
	//	m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0,
	//								 m_VertexArray,
	//								 m_NormalArray,
	//								 m_ColorArray,
	//								 m_TexcoordArray,
	//								 m_VertexNum);
	//}

	IVertexBuffer* BaseMesh::BuildVertexBufferCopy()
	{
		IVertexBuffer* vb = Renderer::Instance().BuildVertexBuffer();
		vb->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0,
						 m_VertexArray,
						 m_NormalArray,
						 m_ColorArray,
						 m_TexcoordArray,
						 m_VertexNum);
		return vb;
	}

	void BaseMesh::BuildMesh(const MeshBuilder* builder)
	{
		ClearMeshElements();

		m_VertexNum = builder->m_VertexCount;
		m_VertexArray = new float[m_VertexNum * 3];
		memcpy(m_VertexArray, builder->m_Vertices, sizeof(float) * 3 * m_VertexNum);

		m_NormalArray = new float[m_VertexNum * 3];
		memcpy(m_NormalArray, builder->m_Normals, sizeof(float) * 3 * m_VertexNum);

		m_TexcoordArray = new float[m_VertexNum * 2];
		memcpy(m_TexcoordArray, builder->m_Texcoords, sizeof(float) * 2 * m_VertexNum);

		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0,
									 m_VertexArray,
									 m_NormalArray,
									 m_ColorArray,
									 m_TexcoordArray,
									 m_VertexNum);

		for (unsigned int i=0; i<builder->m_MeshElementCount; i++)
		{
			MeshElement* elem = new MeshElement(this);
			elem->CreateMeshData(builder->m_MeshElements[i].m_IndexCount / 3, builder->m_MeshElements[i].m_Indices);

			AddMeshElement(elem);
		}

		UpdateElementsOBB();
	}

	MeshElement* BaseMesh::GetElement(unsigned int index)
	{
		return m_MeshElements[index];
	}

	Material* BaseMesh::GetMaterial(unsigned int index)
	{
		if (index>=0 && index<m_Materials.size())
			return m_Materials[index];
		return NULL;
	}

	Vector3f BaseMesh::GetVertexByIndex(unsigned int index) const
	{
		AssertFatal((int)index<m_VertexNum, "MeshElement::GetVertexByIndex() : Index out of boundary.");

		float px = m_VertexArray[index*3];
		float py = m_VertexArray[index*3+1];
		float pz = m_VertexArray[index*3+2];

		return Vector3f(px, py, pz);
	}

	void BaseMesh::AddMeshElement(MeshElement* elem)
	{
		// 新元素添加到子元素列表，并更新obb尺寸
		m_MeshElements.push_back(elem);
		//m_OBB.Expand(elem->GetOBB());
	}

	void BaseMesh::UpdateElementsOBB()
	{
		for (vector<MeshElement*>::iterator iter=m_MeshElements.begin();
			 iter!=m_MeshElements.end();
			 iter++)
		{
			(*iter)->UpdateOBB();
			m_OBB.Expand((*iter)->GetOBB());
		}
	}

	void BaseMesh::ClearMeshElements()
	{
		vector<MeshElement*>::iterator iter;
		for (iter=m_MeshElements.begin(); iter!=m_MeshElements.end(); iter++)
		{
			delete (*iter);
		}

		m_OBB.Reset();
		m_MeshElements.clear();
		m_Materials.clear();
		m_VertexBuffer->Clear();
		m_VertexNum = 0;

		SAFE_DELETE_ARRAY(m_VertexArray);
		SAFE_DELETE_ARRAY(m_NormalArray);
		SAFE_DELETE_ARRAY(m_ColorArray);
		SAFE_DELETE_ARRAY(m_TexcoordArray);
	}

	// ----------------------------------- Mesh Builder

	MeshBuilder::MeshBuilder(unsigned int elementCount)
		: m_VertexCaps(256), m_VertexCount(0),
		  m_MeshElementCount(elementCount),
		  m_MeshElements(NULL)
	{
		m_MeshElements = new MeshElementBuilder[elementCount];

		m_Vertices = (float*)malloc(sizeof(float) * 3  * 256);
		m_Normals = (float*)malloc(sizeof(float) * 3  * 256);
		m_Texcoords = (float*)malloc(sizeof(float) * 2  * 256);
	}

	MeshBuilder::~MeshBuilder()
	{
		SAFE_DELETE_ARRAY(m_MeshElements);

		free(m_Vertices);
		free(m_Normals);
		free(m_Texcoords);

	}

	bool MeshBuilder::AddVertex(float pos[3], float normal[3], float texcoord[2], unsigned int meshElementIndex)
	{
		// 如果子模型索引超出上限，添加失败
		if (meshElementIndex>=m_MeshElementCount) return false;

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

		// 没找到相同的顶点
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
		AddIndex(i, meshElementIndex);

		return true;
	}

	void MeshBuilder::AddIndex(unsigned int index, unsigned int meshElementIndex)
	{
		m_MeshElements[meshElementIndex].AddIndex(index);
	}

	MeshElementBuilder::MeshElementBuilder()
		: m_IndexCaps(250), m_IndexCount(0)
	{
		m_Indices = (unsigned int*)malloc(sizeof(unsigned int) * 256);
	}

	MeshElementBuilder::~MeshElementBuilder()
	{
		free(m_Indices);
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
