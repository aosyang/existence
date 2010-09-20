//-----------------------------------------------------------------------------------
/// GLVertexArray.cpp OpenGL¶¥µãÊý×é
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "GLVertexArray.h"
#include "GLEnums.h"

namespace Gen
{
	GLVertexArray::GLVertexArray()
		: m_VertexArray(NULL),
		  m_NormalArray(NULL),
		  m_ColorArray(NULL),
		  m_TexCoordArray(NULL),
		  m_VertexNum(0)
	{

	}

	GLVertexArray::~GLVertexArray()
	{
		Clear();
	}

	bool GLVertexArray::CreateBuffer(int vertexFormat,
									 const float* vertexArray,
									 const float* normalArray,
									 const float* colorArray,
									 const float* textureCoordArray,
									 unsigned int vertexNum)
	{
		m_VertexFormat = vertexFormat;

		m_VertexNum = vertexNum;

		if (m_VertexFormat & VFormat_Position)
		{
			m_VertexArray = new float[vertexNum * 3];
			if (vertexArray)
				memcpy(m_VertexArray, vertexArray, sizeof(float) * vertexNum * 3);
		}

		if (m_VertexFormat & VFormat_Normal)
		{
			m_NormalArray = new float[vertexNum * 3];
			if (normalArray)
				memcpy(m_NormalArray, normalArray, sizeof(float) * vertexNum * 3);
		}

		if (m_VertexFormat & VFormat_Color)
		{
			m_ColorArray = new float[vertexNum * 4];
			if (colorArray)
				memcpy(m_ColorArray, colorArray, sizeof(float) * vertexNum * 4);
		}

		if (m_VertexFormat & VFormat_Texcoord0)
		{
			m_TexCoordArray = new float[vertexNum * 2];
			if (textureCoordArray)
				memcpy(m_TexCoordArray, textureCoordArray, sizeof(float) * vertexNum * 2);
		}

		return true;
	}

	void GLVertexArray::Clear()
	{
		m_VertexNum = 0;
		SAFE_DELETE_ARRAY(m_VertexArray);
		SAFE_DELETE_ARRAY(m_NormalArray);
		SAFE_DELETE_ARRAY(m_ColorArray);
		SAFE_DELETE_ARRAY(m_TexCoordArray);
	}

	void GLVertexArray::Lock()
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


	void GLVertexArray::ModifyVertexData(VertexFormat dataFormat, int offset,  int size, void* data)
	{
		if (!size || !data) return;

		switch (dataFormat)
		{
		case VFormat_Position:
			memcpy(m_VertexArray + offset, data, size);
			break;
		case VFormat_Normal:
			memcpy(m_NormalArray + offset, data, size);
			break;
		case VFormat_Color:
			memcpy(m_ColorArray + offset, data, size);
			break;
		case VFormat_Texcoord0:
			memcpy(m_TexCoordArray + offset, data, size);
			break;
		default:
			return;
		}
	}


	void GLVertexArray::SetAsVertexDataSource()
	{
		if (!m_VertexNum) return;

		if (m_VertexFormat & VFormat_Position)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, m_VertexArray);
		}
		else
		{
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		if (m_VertexFormat & VFormat_Normal)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, m_NormalArray);
		}
		else
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}

		if (m_VertexFormat & VFormat_Color)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, m_ColorArray);
		}
		else
		{
			glDisableClientState(GL_COLOR_ARRAY);
		}

		for (int i=0; i<8; i++)
		{
			glClientActiveTexture(GL_TEXTURE0 + i);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, m_TexCoordArray);
		}
	}

	void GLVertexArray::RenderPrimitive(PrimitiveType type)
	{
		glDrawArrays(GetGLPrimtiveType(type), 0, m_VertexNum);
	}

	GLIndexArray::GLIndexArray()
		: m_FaceNum(0), m_FaceArray(NULL)
	{
	}

	GLIndexArray::~GLIndexArray()
	{
		Clear();
	}

	bool GLIndexArray::CreateBuffer(unsigned int* faceArray, unsigned int faceNum)
	{
		m_FaceArray = new unsigned int[faceNum * 3];
		if (faceArray)
			memcpy(m_FaceArray, faceArray, sizeof(unsigned int) * faceNum * 3);

		m_FaceNum = faceNum;

		return true;
	}

	void GLIndexArray::Clear()
	{
		m_FaceNum = 0;

		SAFE_DELETE_ARRAY(m_FaceArray);

	}

	void GLIndexArray::SetIndexData(void* indexData, unsigned int indexNum)
	{
		SAFE_DELETE_ARRAY(m_FaceArray);
		m_FaceArray = new unsigned int[indexNum * 3];
		memcpy(m_FaceArray, indexData, sizeof(unsigned int) * indexNum * 3);
		m_FaceNum = indexNum;
	}

	void GLIndexArray::ModifyIndexData(int offset, int size, void* data)
	{
		if (!size || !data) return;

		memcpy(m_FaceArray + offset, data, size);
	}

	void GLIndexArray::SetIndexSize(int size)
	{
		m_FaceNum = size;
	}

	void GLIndexArray::RenderPrimitive(PrimitiveType type)
	{
		glDrawElements(GetGLPrimtiveType(type), m_FaceNum * 3, GL_UNSIGNED_INT, m_FaceArray);
	}


	IVertexBuffer* FactoryCreateVertexArray()
	{
		return new GLVertexArray;
	}

	IIndexBuffer* FactoryCreateIndexArray()
	{
		return new GLIndexArray;
	}
}
