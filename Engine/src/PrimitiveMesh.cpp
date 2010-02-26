//-----------------------------------------------------------------------------------
/// PrimitiveMesh.cpp 基础几何体类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "PrimitiveMesh.h"
#include "MeshElement.h"
#include "Engine.h"
#include "Renderer.h"

namespace Gen
{
	PrimitiveMesh::PrimitiveMesh(const String& filename)
	: BaseMesh(filename)
	{

	}

	bool PrimitiveMesh::LoadImpl()
	{
		// 这类几何体无法通过资源管理器装载

		return false;
	}

	void PrimitiveMesh::CreateBox(float side)
	{
		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
		ClearMeshElements();

		MeshElement* elem = new MeshElement(this);
		
		static unsigned int cubeIndex[] = 
		{
			0	,	1	,	2	,
			2	,	3	,	0	,
			4	,	5	,	6	,
			6	,	7	,	4	,
			8	,	9	,	10	,
			10	,	11	,	8	,
			12	,	13	,	14	,
			14	,	15	,	12	,
			16	,	17	,	18	,
			18	,	19	,	16	,
			20	,	21	,	22	,
			22  ,	23	,	20	};

		static float cubeVertex[] =
		{
			-0.5, -0.5, -0.5, 
			-0.5, 0.5, -0.5, 
			0.5, 0.5, -0.5, 
			0.5, -0.5, -0.5, 
			-0.5, -0.5, 0.5, 
			0.5, -0.5, 0.5, 
			0.5, 0.5, 0.5, 
			-0.5, 0.5, 0.5, 
			-0.5, -0.5, -0.5, 
			0.5, -0.5, -0.5, 
			0.5, -0.5, 0.5, 
			-0.5, -0.5, 0.5, 
			0.5, -0.5, -0.5, 
			0.5, 0.5, -0.5, 
			0.5, 0.5, 0.5, 
			0.5, -0.5, 0.5, 
			0.5, 0.5, -0.5, 
			-0.5, 0.5, -0.5, 
			-0.5, 0.5, 0.5, 
			0.5, 0.5, 0.5, 
			-0.5, 0.5, -0.5, 
			-0.5, -0.5, -0.5, 
			-0.5, -0.5, 0.5, 
			-0.5, 0.5, 0.5 };

		static float cubeNormal[] = 
		{0, 0, -1, 
		0, 0, -1, 
		0, 0, -1, 
		0, 0, -1, 
		0, 0, 1, 
		0, 0, 1, 
		0, 0, 1, 
		0, 0, 1, 
		0, -1, 0, 
		0, -1, 0, 
		0, -1, 0, 
		0, -1, 0, 
		1, 0, 0, 
		1, 0, 0, 
		1, 0, 0, 
		1, 0, 0, 
		0, 1, 0, 
		0, 1, 0, 
		0, 1, 0, 
		0, 1, 0, 
		-1, 0, 0, 
		-1, 0, 0, 
		-1, 0, 0, 
		-1, 0, 0 };

		static float cubeTexcoord[] =
		{
			1, 0, 
			1, 1, 
			0, 1, 
			0, 0, 
			0, 0, 
			1, 0, 
			1, 1, 
			0, 1, 
			0, 0, 
			1, 0, 
			1, 1, 
			0, 1, 
			0, 0, 
			1, 0, 
			1, 1, 
			0, 1, 
			0, 0, 
			1, 0, 
			1, 1, 
			0, 1, 
			0, 0, 
			1, 0, 
			1, 1, 
			0, 1};

		m_VertexArray = new float[72];
		memcpy(m_VertexArray, cubeVertex, sizeof(float) * 72);
		for (int i=0; i<72; i++)
			m_VertexArray[i] *= side;

		m_VertexNum = 24;
		
		//elem->CreateMeshData(24, 12, cubeIndex, vertices, cubeNormal, cubeTexcoord);
		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0,
									 m_VertexArray,
									 cubeNormal,
									 NULL,
									 cubeTexcoord,
									 m_VertexNum);
		elem->CreateMeshData(12, cubeIndex);

		AddMeshElement(elem);
		UpdateElementsOBB();

		m_BoundingRadius = side * 0.866f;
		m_IsResourceLoaded = true;
	}

	void PrimitiveMesh::CreatePositiveYPlane(float side)
	{
		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
		ClearMeshElements();

		MeshElement* elem = new MeshElement(this);

		static unsigned int planeIndex[] = 
		{	0, 1, 2,
			2, 3, 0 };

		static float planeVertex[] =
		{
			0.5, 0.0, -0.5, 
			-0.5, 0.0, -0.5, 
			-0.5, 0.0, 0.5, 
			0.5, 0.0, 0.5
		};

		m_VertexArray = new float[12];
		memcpy(m_VertexArray, planeVertex, sizeof(float) * 12);
		for (int i=0; i<12; i++)
			m_VertexArray[i] *= side;

		static float planeNormal[] = 
		{
		0	,	1	,	0	,
		0	,	1	,	0	,
		0	,	1	,	0	,
		0	,	1	,	0	,
		-1	,	0	,	0};

		static float planeTexcoord[] =
		{
			1	,	1	,
			0	,	1	,
			0	,	0	,
			1	,	0
		};

		m_VertexNum = 4;

		//elem->CreateMeshData(4, 2, planeIndex, vertices, planeNormal, planeTexcoord);
		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0,
									 m_VertexArray,
									 planeNormal,
									 NULL,
									 planeTexcoord,
									 m_VertexNum);
		elem->CreateMeshData(2, planeIndex);

		AddMeshElement(elem);
		UpdateElementsOBB();

		m_BoundingRadius = side * 0.7071f;
		m_IsResourceLoaded = true;
	}

	void PrimitiveMesh::CreatePositiveZPlane(float side)
	{
		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
		ClearMeshElements();

		MeshElement* elem = new MeshElement(this);
		
		static unsigned int planeIndex[] = 
		{	0, 2, 1,
			2, 0, 3};

		static float planeVertex[] =
		{
			0.5, -0.5, 0.0, 
			-0.5, -0.5, 0.0, 
			-0.5, 0.5, 0.0,
			0.5, 0.5, 0.0};

		m_VertexArray = new float[12];
		memcpy(m_VertexArray, planeVertex, sizeof(float) * 12);
		for (int i=0; i<12; i++)
			m_VertexArray[i] *= side;

		static float planeNormal[] = 
		{
		0	,	0	,	1	,
		0	,	0	,	1	,
		0	,	0	,	1	,
		0	,	0	,	1	,
		-1	,	0	,	0};

		static float planeTexcoord[] =
		{
			1	,	1	,
			0	,	1	,
			0	,	0	,
			1	,	0};

		m_VertexNum = 4;

		//elem->CreateMeshData(4, 2, planeIndex, vertices, planeNormal, planeTexcoord);
		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0,
									 m_VertexArray,
									 planeNormal,
									 NULL,
									 planeTexcoord,
									 m_VertexNum);
		elem->CreateMeshData(2, planeIndex);

		AddMeshElement(elem);
		UpdateElementsOBB();

		m_BoundingRadius = side * 0.7071f;
		m_IsResourceLoaded = true;
	}
}
