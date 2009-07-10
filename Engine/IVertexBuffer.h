//-----------------------------------------------------------------------------------
/// IVertexBuffer.h ���㻺��ӿ�
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IVERTEXBUFFER_H
#define _IVERTEXBUFFER_H

class IVertexBuffer
{
public:
	virtual ~IVertexBuffer() {}

	virtual bool CreateBuffer(const float* vertexArray,
								const float* normalArray,
								const float* textureCoordArray,
								unsigned int vertexNum,
								unsigned int* faceArray,
								unsigned int faceNum) = 0;
	virtual void Clear() = 0;

	// �������壬�Խ����޸�
	virtual void Lock(unsigned int vertSize, unsigned int faceSize) = 0;
	virtual void Unlock() = 0;

	// TODO: ��������������ȷ�������ݸ�ʽ֮�������Ϊ�����ӿ�
	virtual void SetVertexData(void* vertexData, unsigned int vertexNum) = 0;
	virtual void SetIndexData(void* indexData, unsigned int indexNum) = 0;

	virtual void RenderBuffer() = 0;
};

#endif
