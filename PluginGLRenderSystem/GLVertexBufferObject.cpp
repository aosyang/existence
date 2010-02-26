//-----------------------------------------------------------------------------------
/// GLVertexBufferObject.cpp OpenGL VBO顶点及索引缓冲实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "GLVertexBufferObject.h"

namespace Gen
{
	static PFNGLGENBUFFERSARBPROC		vbo_glGenBuffers;
	static PFNGLBINDBUFFERARBPROC		vbo_glBindBuffer;
	static PFNGLBUFFERSUBDATAARBPROC	vbo_glBufferSubData;
	static PFNGLBUFFERDATAARBPROC		vbo_glBufferData;
	static PFNGLDELETEBUFFERSARBPROC	vbo_glDeleteBuffers;

	// 安全删除VBO缓冲
#define DELETE_VBO_BUFFER(buffer) if(buffer) { vbo_glDeleteBuffers(1, &buffer); buffer = 0; }

	void ChooseVBOMethods(bool verOver1_5)
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

	// ------------------------ GLVBOVertexBuffer

	GLVBOVertexBuffer::GLVBOVertexBuffer()
		: m_Initialized(false),
		m_VBOVertices(0),
		m_VBONormals(0),
		m_VBOColors(0),
		m_VBOTexCoords(0)
	{
	}

	GLVBOVertexBuffer::~GLVBOVertexBuffer()
	{
		Clear();
	}

	bool GLVBOVertexBuffer::CreateBuffer(int vertexFormat,
										 const float* vertexArray,
										 const float* normalArray,
										 const float* colorArray,
										 const float* textureCoordArray,
										 unsigned int vertexNum)
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

		m_Initialized = true;
		return true;
	}

	void GLVBOVertexBuffer::Clear()
	{
		if (m_Initialized)
		{
			DELETE_VBO_BUFFER(m_VBOVertices);
			DELETE_VBO_BUFFER(m_VBONormals);
			DELETE_VBO_BUFFER(m_VBOColors);
			DELETE_VBO_BUFFER(m_VBOTexCoords);
		}
		m_Initialized = false;
	}

	void GLVBOVertexBuffer::Lock() {}
	void GLVBOVertexBuffer::Unlock() {}
	void GLVBOVertexBuffer::SetVertexData(void* vertexData, unsigned int vertexNum)
	{
		//glBufferSubDataARB()
	}

	void GLVBOVertexBuffer::ModifyVertexData(VertexFormat dataFormat, int offset, int size, void* data)
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

	void GLVBOVertexBuffer::SetAsVertexDataSource()
	{
		if (m_VertexFormat & VFormat_Position)
		{
			vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glEnableClientState(GL_VERTEX_ARRAY);
		}
		else
			glDisableClientState(GL_VERTEX_ARRAY);


		if (m_VertexFormat & VFormat_Normal)
		{
			vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBONormals);
			glNormalPointer(GL_FLOAT, 0, NULL);
			glEnableClientState(GL_NORMAL_ARRAY);
		}
		else
			glDisableClientState(GL_NORMAL_ARRAY);


		if (m_VertexFormat & VFormat_Color)
		{
			vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
			glColorPointer(4, GL_FLOAT, 0, NULL);
			glEnableClientState(GL_COLOR_ARRAY);
		}
		else
			glDisableClientState(GL_COLOR_ARRAY);


		vbo_glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexCoords);
		// TODO: 指定多重纹理的uv坐标
		for (int i=0; i<8; i++)
		{
			glClientActiveTexture(GL_TEXTURE0 + i);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	// ------------------------ GLVBOIndexBuffer

	GLVBOIndexBuffer::GLVBOIndexBuffer()
	: m_VBOIndices(NULL),
	  m_FaceNum(0)
	{
		
	}

	GLVBOIndexBuffer::~GLVBOIndexBuffer()
	{
		// 销毁时清除数据
		Clear();
	}

	bool GLVBOIndexBuffer::CreateBuffer(unsigned int* faceArray, unsigned int faceNum)
	{
		// 创建VBO缓冲，指定数据
		vbo_glGenBuffers(1, &m_VBOIndices);
		vbo_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);
		vbo_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faceNum * 3, faceArray, GL_STREAM_DRAW);

		m_FaceNum = faceNum;

		return true;
	}

	void GLVBOIndexBuffer::Clear()
	{
		DELETE_VBO_BUFFER(m_VBOIndices);
		m_FaceNum = 0;
	}

	void GLVBOIndexBuffer::SetIndexData(void* indexData, unsigned int indexNum)
	{
		DELETE_VBO_BUFFER(m_VBOIndices);

		vbo_glGenBuffers(1, &m_VBOIndices);
		vbo_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);
		vbo_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexNum * 3, indexData, GL_STREAM_DRAW);

		m_FaceNum = indexNum;
	}

	void GLVBOIndexBuffer::ModifyIndexData(int offset, int size, void* data)
	{
		if (!size || !data) return;

		offset *= sizeof(unsigned int);
		vbo_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);
		vbo_glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	}

	void GLVBOIndexBuffer::SetIndexSize(int size)
	{
		m_FaceNum = size;
	}

	void GLVBOIndexBuffer::RenderPrimitive()
	{
		if (m_FaceNum)
		{
			// 使用索引缓冲对象
			vbo_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);

			glDrawElements(GL_TRIANGLES, m_FaceNum * 3, GL_UNSIGNED_INT, NULL);
		}
	}

	IVertexBuffer* FactoryCreateVBOVertexBuffer()
	{
		return new GLVBOVertexBuffer;
	}

	IIndexBuffer* FactoryCreateVBOIndexBuffer()
	{
		return new GLVBOIndexBuffer;
	}
}
