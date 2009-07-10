//-----------------------------------------------------------------------------------
/// GLVertexArray.cpp OpenGL¶¥µãÊý×é
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "GLVertexArray.h"

GLVertexArray::GLVertexArray()
: m_FaceArray(NULL),
  m_VertexArray(NULL),
  m_NormalArray(NULL),
  m_TexCoordArray(NULL),
  
  m_VertexNum(0),
  m_FaceNum(0)
{

}

GLVertexArray::~GLVertexArray()
{
	Clear();
}

bool GLVertexArray::CreateBuffer(const float* vertexArray,
								 const float* normalArray,
								 const float* textureCoordArray,
								 unsigned int vertexNum,
								 unsigned int* faceArray,
								 unsigned int faceNum)
{
	m_VertexNum = vertexNum;
	m_FaceNum = faceNum;

	m_VertexArray = new float[vertexNum * 3];
	memcpy(m_VertexArray, vertexArray, sizeof(float) * vertexNum * 3);

	m_NormalArray = new float[vertexNum * 3];
	memcpy(m_NormalArray, normalArray, sizeof(float) * vertexNum * 3);

	m_TexCoordArray = new float[vertexNum * 2];
	memcpy(m_TexCoordArray, textureCoordArray, sizeof(float) * vertexNum * 2);


	m_FaceArray = new unsigned int[faceNum * 3];
	memcpy(m_FaceArray, faceArray, sizeof(unsigned int) * faceNum * 3);

	return true;
}

void GLVertexArray::Clear()
{
	m_VertexNum = 0;
	m_FaceNum = 0;

	SAFE_DELETE_ARRAY(m_VertexArray);
	SAFE_DELETE_ARRAY(m_NormalArray);
	SAFE_DELETE_ARRAY(m_TexCoordArray)
	SAFE_DELETE_ARRAY(m_FaceArray)
}

void GLVertexArray::Lock(unsigned int vertSize, unsigned int faceSize)
{
	// nothing to do...
}

void GLVertexArray::Unlock()
{
	// nothing to do...
}

void GLVertexArray::SetVertexData(void* vertexData, unsigned int vertexNum)
{

}

void GLVertexArray::SetIndexData(void* indexData, unsigned int indexNum)
{

}

void GLVertexArray::RenderBuffer()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, m_VertexArray);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, m_NormalArray);

	for (int i=0; i<8; i++)
	{
		glClientActiveTexture(GL_TEXTURE0 + i);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, m_TexCoordArray);
	}

	glDrawElements(GL_TRIANGLES, m_FaceNum * 3, GL_UNSIGNED_INT, m_FaceArray);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	for (int i=0; i<8; i++)
	{
		glClientActiveTexture(GL_TEXTURE0 + i);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

}

IVertexBuffer* FactoryCreateVertexArray()
{
	return new GLVertexArray;
}
