//-----------------------------------------------------------------------------------
/// VertexData.h 顶点数据
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _VERTEXDATA_H
#define _VERTEXDATA_H

#include "Platform.h"

enum VertexRenderMode
{
	VRM_POINTS,
	VRM_LINES,
	VRM_LINE_LOOP,
	VRM_LINE_STRIP,
	VRM_TRIANGLES,
	VRM_TRIANGLE_STRIP,
	VRM_TRIANGLE_FAN,
	VRM_QUADS,
	VRM_QUAD_STRIP,
	VRM_POLYGON,
};

struct VertexStruct
{
	float vx, vy, vz;		///< 顶点坐标
	float nx, ny, nz;		///< 法线方向
	float u, v;				///< 纹理坐标
};

class VertexData
{
public:
	VertexData();
	~VertexData();
	
	unsigned int GetSize() { return m_Size; }

	void SetVertexData(unsigned size, void* data);
private:
	unsigned int		m_Size;
	VertexStruct*		m_Vertices;

	VertexRenderMode	m_RenderMode;
};

#endif