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

class GLVertexArray : public IVertexBuffer
{
public:
	GLVertexArray();
	~GLVertexArray();

	bool CreateBuffer(const float* vertexArray,
					  const float* normalArray,
					  const float* textureCoordArray,
					  unsigned int vertexNum,
					  unsigned int* faceArray,
					  unsigned int faceNum);
	void Clear();

	// �������壬�Խ����޸�
	void Lock(unsigned int vertSize, unsigned int faceSize);
	void Unlock();
	void SetVertexData(void* vertexData, unsigned int vertexNum);
	void SetIndexData(void* indexData, unsigned int indexNum);

	void RenderBuffer();

protected:
	unsigned int*	m_FaceArray;		///< ����������
	
	float*			m_VertexArray;		///< ��������
	float*			m_NormalArray;		///< ��������
	float*			m_TexCoordArray;	///< ������������

	unsigned int	m_VertexNum;
	unsigned int	m_FaceNum;

};

IVertexBuffer* FactoryCreateVertexArray();

#endif
