//-----------------------------------------------------------------------------------
/// IVertexBuffer.h ���㻺��ӿ�
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IVERTEXBUFFER_H
#define _IVERTEXBUFFER_H

enum VertexFormat
{
	VFormat_Position = 1 << 0,
	VFormat_Normal = 1 << 1,
	VFormat_Color = 1 << 2,
	VFormat_Texcoord0 = 1 << 3,
};

class IVertexBuffer
{
public:
	virtual ~IVertexBuffer() {}

	// ͨ������ָ�봴�����㻺��
	virtual bool CreateBuffer(	int vertexFormat,
								const float* vertexArray,
								const float* normalArray,
								const float* colorArray,
								const float* textureCoordArray,
								unsigned int vertexNum,
								unsigned int* faceArray,
								unsigned int faceNum) = 0;
	virtual void Clear() = 0;

	// �������壬�Խ����޸�
	virtual void Lock() = 0;
	virtual void Unlock() = 0;

	// TODO: ��������������ȷ�������ݸ�ʽ֮�������Ϊ�����ӿ�
	virtual void SetVertexData(void* vertexData, unsigned int vertexNum) = 0;
	virtual void SetIndexData(void* indexData, unsigned int indexNum) = 0;

	virtual void ModifyVertexData(VertexFormat dataFormat, int offset,  int size, void* data) = 0;
	virtual void ModifyIndexData(int offset, int size, void* data) = 0;
	virtual void SetIndexSize(int size) = 0;

	virtual void RenderBuffer() = 0;
};

#endif