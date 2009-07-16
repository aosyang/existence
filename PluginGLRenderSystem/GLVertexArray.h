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
					  unsigned int vertexNum,
					  unsigned int* faceArray,
					  unsigned int faceNum);
	void Clear();

	// �������壬�Խ����޸�
	void Lock();
	void Unlock();
	void SetVertexData(void* vertexData, unsigned int vertexNum);
	void SetIndexData(void* indexData, unsigned int indexNum);

	void ModifyVertexData(VertexFormat dataFormat, int offset,  int size, void* data);
	void ModifyIndexData(int offset, int size, void* data);
	void SetIndexSize(int size);

	void RenderBuffer();

protected:
	int				m_VertexFormat;
	unsigned int*	m_FaceArray;		///< ����������
	
	float*			m_VertexArray;		///< ��������
	float*			m_NormalArray;		///< ��������
	float*			m_ColorArray;		///< ��ɫ����
	float*			m_TexCoordArray;	///< ������������

	unsigned int	m_VertexNum;
	unsigned int	m_FaceNum;

};

IVertexBuffer* FactoryCreateVertexArray();

#endif
