#include "GLVertexBufferObject.h"

PFNGLGENBUFFERSARBPROC		GLVertexBufferObject::vbo_glGenBuffers;
PFNGLBINDBUFFERARBPROC		GLVertexBufferObject::vbo_glBindBuffer;
PFNGLBUFFERSUBDATAARBPROC	GLVertexBufferObject::vbo_glBufferSubData;
PFNGLBUFFERDATAARBPROC		GLVertexBufferObject::vbo_glBufferData;
PFNGLDELETEBUFFERSARBPROC	GLVertexBufferObject::vbo_glDeleteBuffers;

GLVertexBufferObject::GLVertexBufferObject()
: m_Initialized(false),
  m_VBOVertices(0),
  m_VBONormals(0),
  m_VBOColors(0),
  m_VBOTexCoords(0),
  m_VBOIndices(0)
{
}

GLVertexBufferObject::~GLVertexBufferObject()
{
	Clear();
}

bool GLVertexBufferObject::CreateBuffer(int vertexFormat,
										const float* vertexArray,
										const float* normalArray,
										const float* colorArray,
										const float* textureCoordArray,
										unsigned int vertexNum,
										unsigned int* faceArray,
										unsigned int faceNum)
{
	if (m_Initialized) return false;

	m_VertexFormat = vertexFormat;

	if (m_VertexFormat & VFormat_Position)
	{
		vbo_glGenBuffers(1, &m_VBOVertices);
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
		vbo_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNum * 3, vertexArray, GL_STREAM_DRAW);
	}

	if (m_VertexFormat & VFormat_Normal)
	{
		vbo_glGenBuffers(1, &m_VBONormals);
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBONormals);
		vbo_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNum * 3, normalArray, GL_STREAM_DRAW);
	}

	if (m_VertexFormat & VFormat_Color)
	{
		vbo_glGenBuffers(1, &m_VBOColors);
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
		vbo_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNum * 4, colorArray, GL_STREAM_DRAW);
	}

	if (m_VertexFormat & VFormat_Texcoord0)
	{
		vbo_glGenBuffers(1, &m_VBOTexCoords);
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexCoords);
		vbo_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNum * 2, textureCoordArray, GL_STREAM_DRAW);
	}

	
	vbo_glGenBuffers(1, &m_VBOIndices);
	vbo_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);
	vbo_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faceNum * 3, faceArray, GL_STREAM_DRAW);

	m_FaceNum = faceNum;
	m_Initialized = true;
	return true;
}

#define DELETE_VBO_BUFFER(buffer) if(buffer) { vbo_glDeleteBuffers(1, &buffer); buffer = 0; }

void GLVertexBufferObject::Clear()
{
	if (m_Initialized)
	{
		DELETE_VBO_BUFFER(m_VBOVertices);
		DELETE_VBO_BUFFER(m_VBONormals);
		DELETE_VBO_BUFFER(m_VBOColors);
		DELETE_VBO_BUFFER(m_VBOTexCoords);
		DELETE_VBO_BUFFER(m_VBOIndices);
	}
	m_Initialized = false;
	m_FaceNum = 0;
}

void GLVertexBufferObject::Lock() {}
void GLVertexBufferObject::Unlock() {}
void GLVertexBufferObject::SetVertexData(void* vertexData, unsigned int vertexNum)
{
	//glBufferSubDataARB()
}

void GLVertexBufferObject::SetIndexData(void* indexData, unsigned int indexNum) {}

void GLVertexBufferObject::ModifyVertexData(VertexFormat dataFormat, int offset, int size, void* data)
{
	if (!size || !data) return;

	switch (dataFormat)
	{
	case VFormat_Position:
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
		offset *= sizeof(float);
		break;
	case VFormat_Normal:
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBONormals);
		offset *= sizeof(float);
		break;
	case VFormat_Color:
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
		offset *= sizeof(float);
		break;
	case VFormat_Texcoord0:
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexCoords);
		offset *= sizeof(float);
		break;
	default:
		return;
	}

	vbo_glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void GLVertexBufferObject::ModifyIndexData(int offset, int size, void* data)
{
	if (!size || !data) return;

	offset *= sizeof(unsigned int);
	vbo_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);
	vbo_glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void GLVertexBufferObject::SetIndexSize(int size)
{
	m_FaceNum = size;
}

void GLVertexBufferObject::RenderBuffer()
{
	if (!m_FaceNum) return;

	if (m_VertexFormat & VFormat_Position)
	{
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glEnableClientState(GL_VERTEX_ARRAY);
	}

	if (m_VertexFormat & VFormat_Normal)
	{
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBONormals);
		glNormalPointer(GL_FLOAT, 0, NULL);
		glEnableClientState(GL_NORMAL_ARRAY);
	}

	if (m_VertexFormat & VFormat_Color)
	{
		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
		glColorPointer(4, GL_FLOAT, 0, NULL);
		glEnableClientState(GL_COLOR_ARRAY);
	}

	vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexCoords);
	// TODO: 指定多重纹理的uv坐标
	for (int i=0; i<8; i++)
	{
		glClientActiveTexture(GL_TEXTURE0 + i);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	vbo_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);

	glDrawElements(GL_TRIANGLES, m_FaceNum * 3, GL_UNSIGNED_INT, NULL);

	if (m_VertexFormat & VFormat_Position)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if (m_VertexFormat & VFormat_Normal)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if (m_VertexFormat & VFormat_Color)
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}

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
		vbo_glBufferSubData = glBufferSubData;
		vbo_glBufferData = glBufferData;
		vbo_glDeleteBuffers = glDeleteBuffers;
	}
	else
	{
		vbo_glGenBuffers = glGenBuffersARB;
		vbo_glBindBuffer = glBindBufferARB;
		vbo_glBufferSubData = glBufferSubDataARB;
		vbo_glBufferData = glBufferDataARB;
		vbo_glDeleteBuffers = glDeleteBuffersARB;
	}
}

IVertexBuffer* FactoryCreateVertexBufferObject()
{
	return new GLVertexBufferObject;
}
