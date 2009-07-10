#include "VertexData.h"

VertexData::VertexData()
: m_Size(0), 
  m_Vertices(0),
  m_RenderMode(VRM_TRIANGLES)
{}

VertexData::~VertexData()
{
	SAFE_DELETE(m_Vertices);
}

void VertexData::SetVertexData(unsigned size, void* data)
{
	SAFE_DELETE(m_Vertices);

	m_Size = size;
	m_Vertices = new VertexStruct[size];

	memcpy(m_Vertices, data, sizeof(VertexStruct) * size);
}
