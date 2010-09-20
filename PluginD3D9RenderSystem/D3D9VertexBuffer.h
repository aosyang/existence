//-----------------------------------------------------------------------------------
/// D3D9VertexBuffer.h Direct3D9顶点缓冲及索引缓冲
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _D3D9VERTEXBUFFER_H
#define _D3D9VERTEXBUFFER_H

#include "D3D9RenderSystem.h"
#include "IVertexBuffer.h"

namespace Gen
{

	class D3D9Renderer;
	extern IDirect3DDevice9* g_d3ddevice;

#define VB_FVF (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

	typedef struct
	{
		float x, y, z;
		float nx, ny, nz;
		//DWORD color;
		float u, v;
	} D3DVertex;

	// d3d9顶点缓冲
	class D3D9VertexBuffer : public IVertexBuffer
	{
		friend class D3D9Renderer;
	public:
		D3D9VertexBuffer();
		~D3D9VertexBuffer();

		bool CreateBuffer(int vertexFormat,
						  const float* vertexArray,
						  const float* normalArray,
						  const float* colorArray,
						  const float* textureCoordArray,
						  unsigned int vertexNum);
		void Clear();

		// 锁定缓冲，以进行修改
		void Lock();
		void Unlock();
		void SetVertexData(void* vertexData, unsigned int vertexNum);

		void ModifyVertexData(VertexFormat dataFormat, int offset,  int size, void* data);
		void SetAsVertexDataSource();

		void RenderPrimitive(PrimitiveType type);

		UINT GetVertexNum() const { return m_VertexNum; }
	private:
		IDirect3DVertexBuffer9*		m_D3DVertexBuffer;

		int							m_VertexFormat;
		void*						m_Vertices;
		UINT						m_VertexNum;

		UINT						m_VertexSize;
		DWORD						m_FVF;				///< 顶点格式

		bool						m_Locked;	///< 标识缓冲是否已锁定
	};

	// d3d9索引缓冲
	class D3D9IndexBuffer : public IIndexBuffer
	{
	public:
		D3D9IndexBuffer();

		bool CreateBuffer(	unsigned int* faceArray,
							unsigned int faceNum );

		void Clear();

		void Lock();
		void Unlock();

		void SetIndexData(void* indexData, unsigned int indexNum);
		void ModifyIndexData(int offset, int size, void* data);
		void SetIndexSize(int size);

		void RenderPrimitive();

		void SetVertexNum(UINT vertNum) { m_VertexNum = vertNum; }

	private:
		IDirect3DIndexBuffer9*		m_D3DIndexBuffer;

		void*						m_Indices;
		UINT						m_VertexNum;
		UINT						m_FaceNum;

		bool						m_Locked;	///< 标识缓冲是否已锁定
	};
}

#endif
