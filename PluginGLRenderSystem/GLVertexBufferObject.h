//-----------------------------------------------------------------------------------
/// GLVertexBufferObject.h 使用OpenGL VBO实现顶点及索引缓冲
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _GLVERTEXBUFFEROBJECT_H
#define _GLVERTEXBUFFEROBJECT_H

#include "GLRenderSystem.h"
#include "IVertexBuffer.h"

namespace Gen
{
	// 根据opengl版本，为vbo选择合适的函数
	void ChooseVBOMethods(bool verOver1_5);

	class GLVBOVertexBuffer : public IVertexBuffer
	{
	public:
		GLVBOVertexBuffer();
		~GLVBOVertexBuffer();

		bool CreateBuffer(int vertexFormat,
						  const float* vertexArray,
						  const float* normalArray,
						  const float* colorArray,
						  const float* textureCoordArray,
						  unsigned int vertexNum);
		void Clear();

		void Lock();
		void Unlock();

		void SetVertexData(void* vertexData, unsigned int vertexNum);

		void ModifyVertexData(VertexFormat dataFormat, int offset,  int size, void* data);

		void SetAsVertexDataSource();

		/// @copydoc IVertexBuffer::RenderPrimitive(PrimitiveType)
		void RenderPrimitive(PrimitiveType type);

	protected:
		int					m_VertexFormat;
		unsigned int		m_VBOVertices;
		unsigned int		m_VBONormals;
		unsigned int		m_VBOColors;
		unsigned int		m_VBOTexCoords;

		unsigned int		m_VertexNum;

		bool				m_Initialized : 1;

	};

	// VBO索引缓冲
	class GLVBOIndexBuffer : public IIndexBuffer
	{
	public:
		GLVBOIndexBuffer();
		~GLVBOIndexBuffer();

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
		void RenderPrimitive(PrimitiveType type);
	private:
		unsigned int		m_VBOIndices;	///< VBO索引对象
		unsigned int		m_FaceNum;		///< 多边形数量
	};

	IVertexBuffer* FactoryCreateVBOVertexBuffer();
	IIndexBuffer* FactoryCreateVBOIndexBuffer();
}

#endif
