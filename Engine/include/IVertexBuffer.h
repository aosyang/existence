//-----------------------------------------------------------------------------------
/// IVertexBuffer.h 顶点缓冲接口
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IVERTEXBUFFER_H
#define _IVERTEXBUFFER_H

namespace Gen
{
	enum VertexFormat
	{
		VFormat_Position = 1 << 0,
		VFormat_Normal = 1 << 1,
		VFormat_Color = 1 << 2,
		VFormat_Texcoord0 = 1 << 3,
	};

	/// @brief
	///	渲染几何体的类型
	enum PrimitiveType
	{
		PRIM_LINES,
		PRIM_TRIANGLES,
	};

	// 顶点缓冲接口
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() {}

		// 通过数据指针创建顶点缓冲
		virtual bool CreateBuffer(	int vertexFormat,
									const float* vertexArray,
									const float* normalArray,
									const float* colorArray,
									const float* textureCoordArray,
									unsigned int vertexNum) = 0;
		virtual void Clear() = 0;

		// 锁定缓冲，以进行修改
		virtual void Lock() = 0;
		virtual void Unlock() = 0;

		// TODO: 这两个方法必须确定了数据格式之后才能作为公共接口
		virtual void SetVertexData(void* vertexData, unsigned int vertexNum) = 0;

		virtual void ModifyVertexData(VertexFormat dataFormat, int offset,  int size, void* data) = 0;

		// 指定为顶点数据来源
		virtual void SetAsVertexDataSource() = 0;

		/// @brief
		///	使用顶点缓冲渲染几何体
		virtual void RenderPrimitive(PrimitiveType type) = 0;
	};

	// 索引缓冲接口
	// TODO: 增加一个接口使用户可以设置多边形类型
	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() {}

		virtual bool CreateBuffer(unsigned int* faceArray,
								  unsigned int faceNum) = 0;
		virtual void Clear() = 0;

		virtual void Lock() = 0;
		virtual void Unlock() = 0;

		virtual void SetIndexData(void* indexData, unsigned int indexNum) = 0;

		virtual void ModifyIndexData(int offset, int size, void* data) = 0;
		virtual void SetIndexSize(int size) = 0;

		// 使用已指定的顶点数据渲染多边形
		virtual void RenderPrimitive(PrimitiveType type) = 0;
	};
}

#endif
