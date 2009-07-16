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
					  unsigned int vertexNum,
					  unsigned int* faceArray,
					  unsigned int faceNum);
	void Clear();

	// 锁定缓冲，以进行修改
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
	unsigned int*	m_FaceArray;		///< 三角形数组
	
	float*			m_VertexArray;		///< 顶点数组
	float*			m_NormalArray;		///< 法线数组
	float*			m_ColorArray;		///< 颜色数组
	float*			m_TexCoordArray;	///< 纹理坐标数组

	unsigned int	m_VertexNum;
	unsigned int	m_FaceNum;

};

IVertexBuffer* FactoryCreateVertexArray();

#endif
