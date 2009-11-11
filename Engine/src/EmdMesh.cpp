//-----------------------------------------------------------------------------------
/// EmdMesh.cpp Emd网格类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "EmdMesh.h"

#include <fstream>

using namespace std;

//template <> unsigned int ResourceManager<EmdMesh>::m_sIndex = 0;
//template <> ResourceManager<EmdMesh>::LoadFunc ResourceManager<EmdMesh>::m_sLoadFunc = &EmdMesh::ManagerLoad;

namespace Gen
{
	EmdMesh::EmdMesh()
		: BaseMesh()
	{
	}

	EmdMesh::~EmdMesh()
	{
	}

	IMesh* EmdMesh::ManagerLoad(const String& filename)
	{
		EmdMesh* mesh = new EmdMesh();
		if (!mesh->LoadFromFile(filename))
		{
			delete mesh;
			return NULL;
		}

		return mesh;
	}

	bool EmdMesh::LoadFromFile(const String& filename)
	{
		ClearMeshElements();
		m_BoundingRadius = 0;

		// TODO: 文件读取到内存中，通过指针访问
		ifstream fin;
		fin.open(filename.Data(), ios_base::in|ios_base::binary);

		if (!fin.is_open())
			return false;

		char header[4];
		fin.read((char*)header, 4);

		if (strncmp(header, "EMDL", 4)!=0)		// No header, maybe old format
		{
			Log.OutputTime();
			Log << "Loading mesh with no version info found.\n";
			fin.seekg(ios_base::beg);
			LoadMesh_NoVersion(fin);
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
				Log << "EMD mesh version 0.10.\n";
				if (!LoadMesh_010(fin))
				{
					return false;
				}
				break;
			default:
				// unsupported version
				AssertFatal(0, "EmdMesh::LoadFromFile() : Unsupported mesh version.");
				break;
			}
		}

		fin.close();

		m_Filename = filename;

		return true;
	}

	bool EmdMesh::LoadMesh_NoVersion(ifstream& fin)
	{
		bool result;
		MeshElement* elem = new MeshElement();
		float radius;
		result = LoadMeshElement_NoVersion(elem, fin, radius);

		if (result)
		{
			if (radius>m_BoundingRadius) m_BoundingRadius = radius;
			AddMeshElement(elem);

			m_OBB.Expand(elem->GetOBB());
		}

		return result;
	}

	bool EmdMesh::LoadMesh_010(ifstream& fin)
	{
		unsigned int materialCount = 1;	// 注：对于没有材质的模型，默认子模型数量为1
		unsigned int lump;
		fin.read((char*)&lump, sizeof(lump));
		while (!fin.eof())
		{
			switch (lump)
			{
			case EMDL_LUMP_MESH_ELEMENTS:
				{
					//unsigned int materialCount;
					//fin.read((char*)&materialCount, sizeof(materialCount));
					for (unsigned int i=0; i<materialCount; i++)
						if (!LoadMesh_NoVersion(fin))
							return false;
					break;
				}
			case EMDL_LUMP_TEXTURES:
				{
					fin.read((char*)&materialCount, sizeof(materialCount));

					for (unsigned int i=0; i<materialCount; i++)
					{
						char texName[256];
						unsigned int len;
						fin.read((char*)&len, sizeof(len));

						fin.read(texName, sizeof(char) * len);
						texName[len] = 0;

						Material* mat;
						mat = ResourceManager<Material>::Instance().GetByName(texName);

						// 材质如果没找到，从纹理创建相应材质
						// TODO: 使用了材质读取以后放弃这个方法

						//if ((mat=ResourceManager<Material>::Instance().GetByName(texName)) == NULL)
						//{
						//	mat = ResourceManager<Material>::Instance().Create(texName);
						//	mat->SetTexture(renderer->GetTexture(texName));
						//}
						AddMaterial(mat);
					}

					break;
				}
			default:
				Log.Warning("EmdMesh::LoadMesh_010(): Invalid lump from file.");
				return false;
				break;
			}

			fin.read((char*)&lump, sizeof(lump));

		}

		return true;

	}

	bool EmdMesh::LoadMeshElement_NoVersion(MeshElement* elem, ifstream& fin, float& boundingRadius)
	{
		unsigned int faceCount;
		unsigned int vertexCount;

		fin.read((char*)&faceCount, sizeof(faceCount));

		unsigned int* faceArray = new unsigned int[faceCount * 3];

		fin.read((char*)faceArray, sizeof(faceCount) * 3 * faceCount);
		fin.read((char*)&vertexCount, sizeof(vertexCount));

		float* vertexArray = new float[vertexCount * 3];
		float* normalArray = new float[vertexCount * 3];
		float* texcoordArray = new float[vertexCount * 2];

		// this hacks...
		struct vertex
		{
			float x, y, z;
			float nx, ny, nz;
			float u, v;
		};

		vertex* vertices = new vertex[vertexCount];
		fin.read((char*)vertices, sizeof(vertex) * vertexCount);

		float maxSquaredLength = 0.0f;

		for (unsigned int i=0; i<vertexCount; i++)
		{
			vertexArray[i * 3] = vertices[i].x;
			vertexArray[i * 3 + 1] = vertices[i].y;
			vertexArray[i * 3 + 2] = vertices[i].z;

			normalArray[i * 3] = vertices[i].nx;
			normalArray[i * 3 + 1] = vertices[i].ny;
			normalArray[i * 3 + 2] = vertices[i].nz;

			texcoordArray[i * 2] = vertices[i].u;
			texcoordArray[i * 2 + 1] = vertices[i].v;

			float squaredLen = Vector3f(vertices[i].x, vertices[i].y, vertices[i].z).SquaredLength();
			if (squaredLen > maxSquaredLength) maxSquaredLength = squaredLen;
		}

		boundingRadius = sqrt(maxSquaredLength);

		delete [] vertices;

		elem->CreateMeshData(vertexCount, faceCount, faceArray, vertexArray, normalArray, texcoordArray);

		delete [] faceArray;
		delete [] vertexArray;
		delete [] normalArray;
		delete [] texcoordArray;

		return true;
	}

	bool EmdMesh::LoadMeshElement_010(MeshElement* elem, ifstream& fin)
	{
		float radius;
		return LoadMeshElement_NoVersion(elem, fin, radius);
	}
}
