#include "GLVertexBufferObject.h"

PFNGLGENBUFFERSARBPROC		GLVertexBufferObject::vbo_glGenBuffers;
PFNGLBINDBUFFERARBPROC		GLVertexBufferObject::vbo_glBindBuffer;
PFNGLBUFFERDATAARBPROC		GLVertexBufferObject::vbo_glBufferData;
PFNGLDELETEBUFFERSARBPROC	GLVertexBufferObject::vbo_glDeleteBuffers;

GLVertexBufferObject::GLVertexBufferObject()
: m_Initialized(false)
{
}

GLVertexBufferObject::~GLVertexBufferObject()
{
	Clear();
}

bool GLVertexBufferObject::CreateBuffer(const float* vertexArray,
										const float* normalArray,
										const float* textureCoordArray,
										unsigned int vertexNum,
										unsigned int* faceArray,
										unsigned int faceNum)
{
	vbo_glGenBuffers(1, &m_VBOVertices);
	vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
	vbo_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNum * 3, vertexArray, GL_STATIC_DRAW);

	vbo_glGenBuffers(1, &m_VBONormals);
	vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBONormals);
	vbo_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNum * 3, normalArray, GL_STATIC_DRAW);

	vbo_glGenBuffers(1, &m_VBOTexCoords);
	vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexCoords);
	vbo_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNum * 2, textureCoordArray, GL_STATIC_DRAW);

	
	vbo_glGenBuffers(1, &m_VBOIndices);
	vbo_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);
	vbo_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faceNum * 3, faceArray, GL_STATIC_DRAW);

	m_FaceNum = faceNum;
	m_Initialized = true;
	return true;
}

void GLVertexBufferObject::Clear()
{
	if (m_Initialized)
	{
		vbo_glDeleteBuffers(1, &m_VBOVertices);
		vbo_glDeleteBuffers(1, &m_VBONormals);
		vbo_glDeleteBuffers(1, &m_VBOTexCoords);
		vbo_glDeleteBuffers(1, &m_VBOIndices);
	}

	m_FaceNum = 0;
}

void GLVertexBufferObject::Lock(unsigned int vertSize, unsigned int faceSize) {}
void GLVertexBufferObject::Unlock() {}
void GLVertexBufferObject::SetVertexData(void* vertexData, unsigned int vertexNum) {}
void GLVertexBufferObject::SetIndexData(void* indexData, unsigned int indexNum) {}


void GLVertexBufferObject::RenderBuffer()
{
	vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBONormals);
	glNormalPointer(GL_FLOAT, 0, NULL);

	vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexCoords);
	// TODO: 指定多重纹理的uv坐标
	for (int i=0; i<8; i++)
	{
		glClientActiveTexture(GL_TEXTURE0 + i);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	vbo_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);

	glDrawElements(GL_TRIANGLES, m_FaceNum * 3, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	for (int i=0; i<8; i++)
	{
		glClientActiveTexture(GL_TEXTURE0 + i);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void GLVertexBufferObject::ChooseFunc(bool verOver1_5)
{
	if (verOver1_5)
	{
		vbo_glGenBuffers = glGenBuffers;
		vbo_glBindBuffer = glBindBuffer;
		vbo_glBufferData = glBufferData;
		vbo_glDeleteBuffers = glDeleteBuffers;
	}
	else
	{
		vbo_glGenBuffers = glGenBuffersARB;
		vbo_glBindBuffer = glBindBufferARB;
		vbo_glBufferData = glBufferDataARB;
		vbo_glDeleteBuffers = glDeleteBuffersARB;
	}
}

IVertexBuffer* FactoryCreateVertexBufferObject()
{
	return new GLVertexBufferObject;
}
