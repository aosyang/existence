#ifndef _GLVERTEXBUFFEROBJECT_H
#define _GLVERTEXBUFFEROBJECT_H

#include "GLRenderSystem.h"
#include "IVertexBuffer.h"

namespace Gen
{
	class GLVertexBufferObject : public IVertexBuffer
	{
	public:
		GLVertexBufferObject();
		~GLVertexBufferObject();

		bool CreateBuffer(int vertexFormat,
			const float* vertexArray,
			const float* normalArray,
			const float* colorArray,
			const float* textureCoordArray,
			unsigned int vertexNum,
			unsigned int* faceArray,
			unsigned int faceNum);
		void Clear();

		void Lock();
		void Unlock();
		void SetVertexData(void* vertexData, unsigned int vertexNum);
		void SetIndexData(void* indexData, unsigned int indexNum);

		void ModifyVertexData(VertexFormat dataFormat, int offset,  int size, void* data);
		void ModifyIndexData(int offset, int size, void* data);
		void SetIndexSize(int size);

		void RenderBuffer();

		static void ChooseFunc(bool verOver1_5);

	protected:
		int					m_VertexFormat;
		unsigned int		m_VBOVertices;
		unsigned int		m_VBONormals;
		unsigned int		m_VBOColors;
		unsigned int		m_VBOTexCoords;

		unsigned int		m_VBOIndices;

		unsigned int		m_FaceNum;
		bool				m_Initialized : 1;

		static PFNGLGENBUFFERSARBPROC		vbo_glGenBuffers;
		static PFNGLBINDBUFFERARBPROC		vbo_glBindBuffer;
		static PFNGLBUFFERSUBDATAARBPROC	vbo_glBufferSubData;
		static PFNGLBUFFERDATAARBPROC		vbo_glBufferData;
		static PFNGLDELETEBUFFERSARBPROC	vbo_glDeleteBuffers;

	};

	IVertexBuffer* FactoryCreateVertexBufferObject();
}

#endif
