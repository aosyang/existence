//-----------------------------------------------------------------------------------
/// IVertexBuffer.h 顶点缓冲接口
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

	// 锁定缓冲，以进行修改
	virtual void Lock(unsigned int vertSize, unsigned int faceSize) = 0;
	virtual void Unlock() = 0;

	// TODO: 这两个方法必须确定了数据格式之后才能作为公共接口
	virtual void SetVertexData(void* vertexData, unsigned int vertexNum) = 0;
	virtual void SetIndexData(void* indexData, unsigned int indexNum) = 0;

	virtual void RenderBuffer() = 0;
};

#endif
