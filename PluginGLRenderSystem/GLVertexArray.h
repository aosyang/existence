//-----------------------------------------------------------------------------------
/// GLVertexArray.h OpenGL��������
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
	// ��������
	// OpenGL�ĳ�����Ⱦ����
	// ����һ������ԣ�Ч�ʽϵ�
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

		// �������壬�Խ����޸�
		void Lock();
		void Unlock();
		void SetVertexData(void* vertexData, unsigned int vertexNum);

		void ModifyVertexData(VertexFormat dataFormat, int offset,  int size, void* data);

		void SetAsVertexDataSource();

	protected:
		int				m_VertexFormat;

		float*			m_VertexArray;		///< ��������
		float*			m_NormalArray;		///< ��������
		float*			m_ColorArray;		///< ��ɫ����
		float*			m_TexCoordArray;	///< ������������

		unsigned int	m_VertexNum;
	};

	// VBO��������
	class GLIndexArray : public IIndexBuffer
	{
	public:
		GLIndexArray();
		~GLIndexArray();

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
		void RenderPrimitive();
	private:
		unsigned int*		m_FaceArray;	///< ����������
		unsigned int		m_FaceNum;		///< ���������
	};

	IVertexBuffer* FactoryCreateVertexArray();
	IIndexBuffer* FactoryCreateIndexArray();
}

#endif
