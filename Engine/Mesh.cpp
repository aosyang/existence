//-----------------------------------------------------------------------------------
/// Mesh.cpp 网格类
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Mesh.h"
#include "Engine.h"

#include <fstream>

using namespace std;

Mesh::Mesh()
:
  m_BoundingRadius(0.0f)
{
	//m_MeshColor = Color::WHITE;
	//m_MeshColor = Color4f(1.0f, 0.0f, 1.0f);
	//SetMeshColor(Color::WHITE);

	//m_MeshMat.Identity();
}

Mesh::~Mesh()
{
	ClearMeshElements();
}

bool Mesh::LoadFromFile(const string& filename)
{

	ClearMeshElements();
	m_BoundingRadius = 0;

	// TODO: 以后放到一个公共地方调用，只需要调用一次
	// 设置语言，正确读取带中文名的文件
	locale   langLocale("");
	setlocale(LC_ALL, langLocale.name().data());

	ifstream fin;
	fin.open(filename.data(), ios_base::in|ios_base::binary);

	if (!fin.is_open())
		return false;

	char header[4];
	fin.read((char*)header, 4);

//	if (strcmp(header, "EMDL"))		// No header, maybe old format
	if (!(header[0]=='E' &&
		header[1]=='M' &&
		header[2]=='D' &&
		header[3]=='L'))
	{
		Log.OutputTime();
		Log << "Loading mesh with no version info found.\n";
		fin.seekg(ios_base::beg);
		LoadMesh_NoVersion(this, fin);
	}
	else
	{
		unsigned int ver;
		fin.read((char*)&ver, sizeof(ver));

		// 版本号检查
		// 10代表0.10版，以此类推
		Log.OutputTime();
		switch (ver)
		{
		case 10:
			Log << "Loading mesh version 0.10.\n";
			LoadMesh_010(this, fin);
			break;
		default:
			// unsupported version
			AssertFatal(0, "Mesh::LoadFromFile() : Unsupported mesh version.");
			break;
		}
	}

	fin.close();

	return true;

}

//bool Mesh::LoadFromFile(const char* filename)
//{
//	// TODO: 以后放到一个公共地方调用，只需要调用一次
//	// 设置语言，正确读取带中文名的文件
//	locale   langLocale("");
//	setlocale(LC_ALL, langLocale.name().data());
//
//	ifstream fin;
//	fin.open(filename);
//
//	if (!fin.is_open())
//		return false;
//
//	// Slow...
//	// TODO: Try binary file
//
//	fin >> m_FaceNum;
//
//	m_FaceArray = new unsigned int[m_FaceNum * 3];
//
//	for (unsigned int i=0; i<m_FaceNum*3; i++)
//	{
//		fin >> m_FaceArray[i];
//	}
//
//	fin >> m_VertexNum;
//
//	m_VertexArray = new float[m_VertexNum * 3];
//	m_NormalArray = new float[m_VertexNum * 3];
//	m_TexCoordArray = new float[m_VertexNum * 2];
//
//	for (unsigned int i=0; i<m_VertexNum; i++)
//	{
//		fin >> m_VertexArray[i * 3];
//		fin >> m_VertexArray[i * 3 + 1];
//		fin >> m_VertexArray[i * 3 + 2];
//
//		fin >> m_NormalArray[i * 3];
//		fin >> m_NormalArray[i * 3 + 1];
//		fin >> m_NormalArray[i * 3 + 2];
//
//		fin >> m_TexCoordArray[i * 2];
//		fin >> m_TexCoordArray[i * 2 + 1];
//	}
//	
//	ofstream test_out("test_txt.out");
//	for (int i=0; i<m_FaceNum; i++)
//	{
//		test_out << m_FaceArray[i * 3] << " " << m_FaceArray[i * 3 + 1] << " " << m_FaceArray[i * 3 + 2] << endl;
//	}
//
//	//for (int i=0; i<m_VertexNum; i++)
//	//{
//	//	test_out << m_VertexArray[i * 3];
//	//	test_out << m_VertexArray[i * 3 + 1];
//	//	test_out << m_VertexArray[i * 3 + 2];
//
//	//	test_out << m_NormalArray[i * 3];
//	//	test_out << m_NormalArray[i * 3 + 1];
//	//	test_out << m_NormalArray[i * 3 + 2];
//
//	//	test_out << m_TexCoordArray[i * 2];
//	//	test_out << m_TexCoordArray[i * 2 + 1];
//
//	//	test_out << endl;
//	////test_out.write((char*)m_VertexArray, sizeof(float) * 3 * m_VertexNum);
//	////test_out.write((char*)m_NormalArray, sizeof(float) * 3 * m_VertexNum);
//	////test_out.write((char*)m_TexCoordArray, sizeof(float) * 2 * m_VertexNum);
//	//}
//	test_out.close();
//	fin.close();
//
//	return true;
//
//}

void Mesh::CreateBox(float side)
{
	ClearMeshElements();

	MeshElement* elem = new MeshElement();
	elem->CreateBox(side);

	AddMeshElement(elem);

	m_BoundingRadius = side * 0.866f;
}

void Mesh::CreatePositiveYPlane(float side)
{
	ClearMeshElements();

	MeshElement* elem = new MeshElement();
	elem->CreatePositiveYPlane(side);

	AddMeshElement(elem);

	m_BoundingRadius = side * 0.7071f;
}

void Mesh::AddMeshElement(MeshElement* elem)
{
	m_MeshElements.push_back(elem);
	m_OBB.Expand(elem->GetOBB());
}


void Mesh::ClearMeshElements()
{
	vector<MeshElement*>::iterator iter;
	for (iter=m_MeshElements.begin(); iter!=m_MeshElements.end(); iter++)
	{
		delete (*iter);
	}

	m_OBB.Reset();

	m_MeshElements.clear();

	m_Materials.clear();
}


bool LoadMesh_NoVersion(Mesh* mesh, ifstream& fin)
{
	bool result;
	MeshElement* elem = new MeshElement();
	float radius;
	result = LoadMeshElement_NoVersion(elem, fin, radius);
	mesh->m_BoundingRadius;

	if (result)
	{
		if (radius>mesh->m_BoundingRadius) mesh->m_BoundingRadius = radius;
		mesh->AddMeshElement(elem);

		mesh->m_OBB.Expand(elem->m_OBB);
	}

	return result;
}

bool LoadMesh_010(Mesh* mesh, ifstream& fin)
{
	unsigned int matNum = 1;	// 注：对于没有材质的模型，默认子模型数量为1
	unsigned int lump;
	fin.read((char*)&lump, sizeof(lump));
	while (!fin.eof())
	{
		switch (lump)
		{
		case EMDL_LUMP_MESH_ELEMENTS:
			{
				//unsigned int matNum;
				//fin.read((char*)&matNum, sizeof(matNum));
				for (unsigned int i=0; i<matNum; i++)
					if (!LoadMesh_NoVersion(mesh, fin))
						return false;
				break;
			}
		case EMDL_LUMP_TEXTURES:
			{
				fin.read((char*)&matNum, sizeof(matNum));

				for (unsigned int i=0; i<matNum; i++)
				{
					char texName[256];
					unsigned int len;
					fin.read((char*)&len, sizeof(len));

					fin.read(texName, sizeof(char) * len);
					texName[len] = 0;

					Material* mat;
					if ((mat=ResourceManager<Material>::Instance().GetByName(texName)) == NULL)
					{
						mat = ResourceManager<Material>::Instance().Create(texName);
						mat->SetTexture(renderer->GetTexture(texName));
					}
					mesh->AddMaterial(mat);
				}

				break;
			}
		default:
			AssertFatal(0, "LoadMesh_010() : Invalid lump from file.");
			break;
		}

		fin.read((char*)&lump, sizeof(lump));

	}

	return true;

}
