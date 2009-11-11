#include "PrimitiveMesh.h"

namespace Gen
{
	PrimitiveMesh::PrimitiveMesh()
	: BaseMesh()
	{
	}


	void PrimitiveMesh::CreateBox(float side)
	{
		ClearMeshElements();

		MeshElement* elem = new MeshElement();
		
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

		float vertices[72];
		memcpy(vertices, cubeVertex, sizeof(float) * 72);
		for (int i=0; i<72; i++)
			vertices[i] *= side;
		
		elem->CreateMeshData(24, 12, cubeIndex, vertices, cubeNormal, cubeTexcoord);

		AddMeshElement(elem);

		m_BoundingRadius = side * 0.866f;
	}

	void PrimitiveMesh::CreatePositiveYPlane(float side)
	{

		ClearMeshElements();

		MeshElement* elem = new MeshElement();

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

		float vertices[12];
		memcpy(vertices, planeVertex, sizeof(float) * 12);
		for (int i=0; i<12; i++)
			vertices[i] *= side;

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

		elem->CreateMeshData(4, 2, planeIndex, vertices, planeNormal, planeTexcoord);

		AddMeshElement(elem);

		m_BoundingRadius = side * 0.7071f;
	}

	void PrimitiveMesh::CreatePositiveZPlane(float side)
	{
		ClearMeshElements();

		MeshElement* elem = new MeshElement();
		
		static unsigned int planeIndex[] = 
		{	0, 2, 1,
			2, 0, 3};

		static float planeVertex[] =
		{
			0.5, -0.5, 0.0, 
			-0.5, -0.5, 0.0, 
			-0.5, 0.5, 0.0,
			0.5, 0.5, 0.0};

		float vertices[12];
		memcpy(vertices, planeVertex, sizeof(float) * 12);
		for (int i=0; i<12; i++)
			vertices[i] *= side;

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

		elem->CreateMeshData(4, 2, planeIndex, vertices, planeNormal, planeTexcoord);

		AddMeshElement(elem);

		m_BoundingRadius = side * 0.7071f;

	}
}
