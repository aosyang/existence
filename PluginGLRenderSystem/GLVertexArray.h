//-----------------------------------------------------------------------------------
/// GLVertexArray.h OpenGL顶点数组
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _GLVERTEXARRAY_H
#define _GLVERTEXARRAY_H

#include "GLRenderSystem.h"
#include "IVertexBuffer.h"

namespace Gen
{
	// 顶点数组
	// OpenGL的常规渲染数据
	// 具有一定灵活性，效率较低
	class GLVertexArray : public IVertexBuffer
	{
	public:
		GLVertexArray();
		~GLVertexArray();

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

	protected:
		int				m_VertexFormat;

		float*			m_VertexArray;		///< 顶点数组
		float*			m_NormalArray;		///< 法线数组
		float*			m_ColorArray;		///< 颜色数组
		float*			m_TexCoordArray;	///< 纹理坐标数组

		unsigned int	m_VertexNum;
	};

	// VBO索引缓冲
	class GLIndexArray : public IIndexBuffer
	{
	public:
		GLIndexArray();
		~GLIndexArray();

		// 创建索引缓冲
		bool CreateBuffer(unsigned int* faceArray, unsigned int faceNum);

		// 清除缓冲数据
		void Clear();

		void Lock() {}
		void Unlock() {}

		void SetIndexData(void* indexData, unsigned int indexNum);

		void ModifyIndexData(int offset, int size, void* data);
		void SetIndexSize(int size);

		// 渲染多边形
		void RenderPrimitive();
	private:
		unsigned int*		m_FaceArray;	///< 三角形数组
		unsigned int		m_FaceNum;		///< 多边形数量
	};

	IVertexBuffer* FactoryCreateVertexArray();
	IIndexBuffer* FactoryCreateIndexArray();
}

#endif
