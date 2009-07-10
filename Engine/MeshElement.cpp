#include "MeshElement.h"
#include "Engine.h"

MeshElement::MeshElement()
: m_FaceArray(0), m_VertexArray(0), m_NormalArray(0), m_TexCoordArray(0),
  m_VertexNum(0), m_FaceNum(0), m_Material(0)
{
	m_VertexBuffer = renderer->BuildVertexBuffer();
}

MeshElement::~MeshElement()
{
	SAFE_DELETE_ARRAY(m_FaceArray)
	SAFE_DELETE_ARRAY(m_VertexArray)
	SAFE_DELETE_ARRAY(m_NormalArray)
	SAFE_DELETE_ARRAY(m_TexCoordArray)

	SAFE_DELETE(m_VertexBuffer)
}


void MeshElement::CreateBox(float side)
{
	m_FaceNum = 12;
	m_VertexNum = 24;

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

	m_FaceArray = new unsigned int[m_FaceNum * 3];

	memcpy(m_FaceArray, &cubeIndex, sizeof(unsigned int) * m_FaceNum * 3);

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

	m_VertexArray = new float[m_VertexNum * 3];

	memcpy(m_VertexArray, &cubeVertex, sizeof(float) * m_VertexNum * 3);
	for (unsigned int i=0; i<m_VertexNum * 3; i++)
		m_VertexArray[i] *= side;

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

	m_NormalArray = new float[m_VertexNum * 3];
	memcpy(m_NormalArray, &cubeNormal, sizeof(float) * m_VertexNum * 3);

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

	m_TexCoordArray = new float[m_VertexNum * 2];
	memcpy(m_TexCoordArray, &cubeTexcoord, sizeof(float) * m_VertexNum * 2);

	BuildVertexBufferData();
	UpdateOBB();
}


void MeshElement::CreatePositiveYPlane(float side)
{
	m_FaceNum = 2;
	m_VertexNum = 4;

	static unsigned int planeIndex[] = 
	{	0, 1, 2,
		2, 3, 0 };

	m_FaceArray = new unsigned int[m_FaceNum * 3];

	memcpy(m_FaceArray, &planeIndex, sizeof(unsigned int) * m_FaceNum * 3);

	static float planeVertex[] =
	{
		0.5, 0.0, -0.5, 
		-0.5, 0.0, -0.5, 
		-0.5, 0.0, 0.5, 
		0.5, 0.0, 0.5};

	m_VertexArray = new float[m_VertexNum * 3];

	memcpy(m_VertexArray, &planeVertex, sizeof(float) * m_VertexNum * 3);
	for (unsigned int i=0; i<m_VertexNum * 3; i++)
		m_VertexArray[i] *= side;

	static float planeNormal[] = 
	{
	0	,	1	,	0	,
	0	,	1	,	0	,
	0	,	1	,	0	,
	0	,	1	,	0	,
	-1	,	0	,	0};

	m_NormalArray = new float[m_VertexNum * 3];
	memcpy(m_NormalArray, &planeNormal, sizeof(float) * m_VertexNum * 3);

	static float planeTexcoord[] =
	{
		1	,	1	,
		0	,	1	,
		0	,	0	,
		1	,	0};

	m_TexCoordArray = new float[m_VertexNum * 2];
	memcpy(m_TexCoordArray, &planeTexcoord, sizeof(float) * m_VertexNum * 2);

	BuildVertexBufferData();
	UpdateOBB();
}

void MeshElement::UpdateOBB()
{
	if (!m_VertexNum)
		return ;

	m_OBB.SetPoint(Vector3f(m_VertexArray));

	for (int i=1; i<m_VertexNum; i++)
		m_OBB.Expand(Vector3f(&m_VertexArray[i*3]));
}

void MeshElement::BuildVertexBufferData()
{
	m_VertexBuffer->Clear();
	m_VertexBuffer->CreateBuffer(m_VertexArray, m_NormalArray, m_TexCoordArray, m_VertexNum, m_FaceArray, m_FaceNum);
}


bool LoadMeshElement_NoVersion(MeshElement* mesh, ifstream& fin, float& boundingRadius)
{
	fin.read((char*)&mesh->m_FaceNum, sizeof(mesh->m_FaceNum));

	mesh->m_FaceArray = new unsigned int[mesh->m_FaceNum * 3];

	fin.read((char*)mesh->m_FaceArray, sizeof(mesh->m_FaceNum) * 3 * mesh->m_FaceNum);
	fin.read((char*)&mesh->m_VertexNum, sizeof(mesh->m_VertexNum));

	mesh->m_VertexArray = new float[mesh->m_VertexNum * 3];
	mesh->m_NormalArray = new float[mesh->m_VertexNum * 3];
	mesh->m_TexCoordArray = new float[mesh->m_VertexNum * 2];

	// this hacks...
	struct vertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};

	vertex* vertices = new vertex[mesh->m_VertexNum];
	fin.read((char*)vertices, sizeof(vertex) * mesh->m_VertexNum);

	float maxSquaredLength = 0.0f;

	for (unsigned int i=0; i<mesh->m_VertexNum; i++)
	{
		 mesh->m_VertexArray[i * 3] = vertices[i].x;
		 mesh->m_VertexArray[i * 3 + 1] = vertices[i].y;
		 mesh->m_VertexArray[i * 3 + 2] = vertices[i].z;

		 mesh->m_NormalArray[i * 3] = vertices[i].nx;
		 mesh->m_NormalArray[i * 3 + 1] = vertices[i].ny;
		 mesh->m_NormalArray[i * 3 + 2] = vertices[i].nz;

		 mesh->m_TexCoordArray[i * 2] = vertices[i].u;
		 mesh->m_TexCoordArray[i * 2 + 1] = vertices[i].v;

		 float squaredLen = Vector3f(vertices[i].x, vertices[i].y, vertices[i].z).SquaredLength();
		 if (squaredLen > maxSquaredLength) maxSquaredLength = squaredLen;
	}

	boundingRadius = sqrt(maxSquaredLength);
	
	delete [] vertices;

	mesh->BuildVertexBufferData();
	mesh->UpdateOBB();

	return true;
}

bool LoadMeshElement_010(MeshElement* mesh, ifstream& fin)
{
	float radius;
	return LoadMeshElement_NoVersion(mesh, fin, radius);
}
