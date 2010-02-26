//-----------------------------------------------------------------------------------
/// D3D9VertexBuffer.cpp Direct3D9顶点缓冲及索引缓冲
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "D3D9VertexBuffer.h"

namespace Gen
{
	D3D9VertexBuffer::D3D9VertexBuffer()
		: m_D3DVertexBuffer(NULL),
		  m_VertexFormat(NULL),
		  m_VertexNum(0),
		  m_Locked(false)
	{
	}

	D3D9VertexBuffer::~D3D9VertexBuffer()
	{
		Clear();
	}

	bool D3D9VertexBuffer::CreateBuffer(int vertexFormat,
										const float* vertexArray,
										const float* normalArray,
										const float* colorArray,
										const float* textureCoordArray,
										unsigned int vertexNum)
	{
		// 记录顶点格式
		m_VertexFormat = vertexFormat;

		// 创建顶点缓冲
		//if( FAILED( g_d3ddevice->CreateVertexBuffer( vertexNum * sizeof(D3DVertex),
		//                                              D3DUSAGE_WRITEONLY, VB_FVF,
		//                                              D3DPOOL_MANAGED, &m_D3DVertexBuffer, NULL ) ) )
		if( FAILED( g_d3ddevice->CreateVertexBuffer( vertexNum * sizeof(D3DVertex),
			0, VB_FVF,
			D3DPOOL_DEFAULT, &m_D3DVertexBuffer, NULL ) ) )
		{
			return false;
		}

		// 将数组形式原始数据保存到缓冲中
		D3DVertex* vert = new D3DVertex[vertexNum];

		//if (vertexArray && normalArray && textureCoordArray)
		//{

		int v = 0, n = 0, t = 0;
		for (unsigned int i=0; i<vertexNum; i++)
		{
			if (m_VertexFormat & VFormat_Position && vertexArray)
			{
				vert[i].x = vertexArray[v++];
				vert[i].y = vertexArray[v++];
				vert[i].z = vertexArray[v++];
			}

			if (m_VertexFormat & VFormat_Normal && normalArray)
			{
				vert[i].nx = -normalArray[n++];
				vert[i].ny = -normalArray[n++];
				vert[i].nz = -normalArray[n++];
			}

			//vert[i].color = 0xFFFFFFFF;

			if (m_VertexFormat & VFormat_Texcoord0 && textureCoordArray)
			{
				// 纹理坐标生成
				// 注：d3d使用不同的uv坐标，需要进行手动转换
				vert[i].u = textureCoordArray[t++];
				vert[i].v = 1.0f - textureCoordArray[t++];
			}
		}

		//}
		Lock();
		SetVertexData(vert, vertexNum);
		Unlock();

		delete [] vert;

		m_VertexNum = vertexNum;

		return true;
	}

	void D3D9VertexBuffer::Clear()
	{
		if (m_D3DVertexBuffer)
		{
			m_D3DVertexBuffer->Release();
			m_D3DVertexBuffer = NULL;
		}

		m_Vertices = NULL;
		m_VertexNum = 0;

		m_Locked = false;
	}


	void D3D9VertexBuffer::Lock()
	{
		m_D3DVertexBuffer->Lock(0, 0, (void**)(&m_Vertices), 0);
		m_Locked = true;
	}

	void D3D9VertexBuffer::Unlock()
	{
		m_D3DVertexBuffer->Unlock();
		m_Locked = false;
	}

	void D3D9VertexBuffer::SetVertexData(void* vertexData, unsigned int vertexNum)
	{
		if (!vertexNum) return;
		memcpy(m_Vertices, vertexData, sizeof(D3DVertex) * vertexNum);
	}

	void D3D9VertexBuffer::ModifyVertexData(VertexFormat dataFormat, int offset,  int size, void* data)
	{
	}

	void D3D9VertexBuffer::SetAsVertexDataSource()
	{
		g_d3ddevice->SetStreamSource(0, this->m_D3DVertexBuffer, 0, sizeof(D3DVertex));
		g_d3ddevice->SetFVF(VB_FVF);
	}

	// -------------------------- D3D9IndexBuffer

	D3D9IndexBuffer::D3D9IndexBuffer()
		: m_D3DIndexBuffer(NULL),
		  m_VertexNum(0),
		  m_FaceNum(0),
		  m_Locked(false)
	{
	}

	bool D3D9IndexBuffer::CreateBuffer(	unsigned int* faceArray,
										unsigned int faceNum )
	{

		// 创建索引缓冲
		//if( FAILED( g_d3ddevice->CreateIndexBuffer( 3 * faceNum * sizeof(unsigned int),
		//														D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, 
		//														&m_D3DIndexBuffer, NULL ) ) )
		if( FAILED( g_d3ddevice->CreateIndexBuffer( 3 * faceNum * sizeof(unsigned int),
			0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, 
			&m_D3DIndexBuffer, NULL ) ) )
		{
			return false;
		}

		if (faceArray)
		{
			Lock();
			SetIndexData(faceArray, faceNum);
			Unlock();
		}

		m_FaceNum = faceNum;

		return true;
	}

	void D3D9IndexBuffer::Clear()
	{
		if (m_D3DIndexBuffer)
		{
			m_D3DIndexBuffer->Release();
			m_D3DIndexBuffer = NULL;
		}

		m_Indices = NULL;
		m_FaceNum = 0;

		m_Locked = false;
	}

	void D3D9IndexBuffer::Lock()
	{
		m_D3DIndexBuffer->Lock(0, 0, (void**)(&m_Indices), 0);
		m_Locked = true;
	}

	void D3D9IndexBuffer::Unlock()
	{
		m_D3DIndexBuffer->Unlock();
		m_Locked = false;
	}

	void D3D9IndexBuffer::SetIndexData(void* indexData, unsigned int indexNum)
	{
		if (!indexNum) return;
		memcpy(m_Indices, indexData, sizeof(unsigned int) * indexNum * 3);
	}

	void D3D9IndexBuffer::ModifyIndexData(int offset, int size, void* data)
	{
	}

	void D3D9IndexBuffer::SetIndexSize(int size)
	{
	}

	void D3D9IndexBuffer::RenderPrimitive()
	{
		g_d3ddevice->SetIndices(this->m_D3DIndexBuffer);
		//m_D3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, this->m_VertexNum/3 );
		g_d3ddevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this->m_VertexNum, 0, this->m_FaceNum);
	}
}
