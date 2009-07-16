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

unsigned int ResourceManager<Mesh>::m_sIndex = 0;
ResourceManager<Mesh>::LoadFunc ResourceManager<Mesh>::m_sLoadFunc = &Mesh::LoadMeshFromFile;

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

Mesh* Mesh::LoadMeshFromFile(const string& filename)
{
	Mesh* mesh = new Mesh();
	mesh->ClearMeshElements();
	mesh->m_BoundingRadius = 0;

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
		LoadMesh_NoVersion(mesh, fin);
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
			LoadMesh_010(mesh, fin);
			break;
		default:
			// unsupported version
			AssertFatal(0, "Mesh::LoadFromFile() : Unsupported mesh version.");
			break;
		}
	}

	fin.close();

	return mesh;

}

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
