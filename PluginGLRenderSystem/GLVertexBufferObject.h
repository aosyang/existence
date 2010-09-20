//-----------------------------------------------------------------------------------
/// GLVertexBufferObject.h ʹ��OpenGL VBOʵ�ֶ��㼰��������
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
	// ����opengl�汾��Ϊvboѡ����ʵĺ���
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

	// VBO��������
	class GLVBOIndexBuffer : public IIndexBuffer
	{
	public:
		GLVBOIndexBuffer();
		~GLVBOIndexBuffer();

		// ������������
		bool CreateBuffer(unsigned int* faceArray, unsigned int faceNum);

		// �����������
		void Clear();

		void Lock() {}
		void Unlock() {}

		void SetIndexData(void* indexData, unsigned int indexNum);

		void ModifyIndexData(int offset, int size, void* data);
		void SetIndexSize(int size);

		// ��Ⱦ�����
		void RenderPrimitive(PrimitiveType type);
	private:
		unsigned int		m_VBOIndices;	///< VBO��������
		unsigned int		m_FaceNum;		///< ���������
	};

	IVertexBuffer* FactoryCreateVBOVertexBuffer();
	IIndexBuffer* FactoryCreateVBOIndexBuffer();
}

#endif
