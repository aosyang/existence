#ifndef _GLVERTEXBUFFEROBJECT_H
#define _GLVERTEXBUFFEROBJECT_H

#include "GLRenderSystem.h"
#include "IVertexBuffer.h"

class GLVertexBufferObject : public IVertexBuffer
{
public:
	GLVertexBufferObject();
	~GLVertexBufferObject();

	bool CreateBuffer(const float* vertexArray,
					  const float* normalArray,
					  const float* textureCoordArray,
					  unsigned int vertexNum,
					  unsigned int* faceArray,
					  unsigned int faceNum);
	void Clear();

	void Lock(unsigned int vertSize, unsigned int faceSize);
	void Unlock();
	void SetVertexData(void* vertexData, unsigned int vertexNum);
	void SetIndexData(void* indexData, unsigned int indexNum);

	void RenderBuffer();

	static void ChooseFunc(bool verOver1_5);

protected:
	unsigned int		m_VBOVertices;
	unsigned int		m_VBONormals;
	unsigned int		m_VBOTexCoords;

	unsigned int		m_VBOIndices;

	unsigned int		m_FaceNum;
	bool				m_Initialized : 1;

	static PFNGLGENBUFFERSARBPROC		vbo_glGenBuffers;
	static PFNGLBINDBUFFERARBPROC		vbo_glBindBuffer;
	static PFNGLBUFFERDATAARBPROC		vbo_glBufferData;
	static PFNGLDELETEBUFFERSARBPROC	vbo_glDeleteBuffers;

};

IVertexBuffer* FactoryCreateVertexBufferObject();

#endif
