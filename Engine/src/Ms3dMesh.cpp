//-----------------------------------------------------------------------------------
/// Ms3dMesh.cpp ms3d网格类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Ms3dMesh.h"

namespace Gen
{
	Ms3dMesh::Ms3dMesh()
		: BaseMesh()
	{
	}

	IMesh* Ms3dMesh::ManagerLoad(const String& filename)
	{
		Ms3dMesh* mesh = new Ms3dMesh();
		if (!mesh->LoadFromFile(filename))
		{
			delete mesh;
			return NULL;
		}

		return mesh;
	}

	bool Ms3dMesh::LoadFromFile(const String& filename)
	{
		ifstream inputFile(filename.Data(), ios::in|ios::binary);

		if (inputFile.fail())	// 无法打开文件
			return false;

		inputFile.seekg(0, ios::end);
		long fileSize = inputFile.tellg();
		inputFile.seekg(0, ios::beg);

		byte*  pBuffer = new byte[fileSize];
		inputFile.read((char*)pBuffer, fileSize);
		inputFile.close();

		const byte* pPtr = pBuffer;
		Ms3dHeader* pHeader = (Ms3dHeader*)pPtr;
		pPtr += sizeof(Ms3dHeader);

		if (strncmp(pHeader->id, "MS3D000000", 10)!=0)
			return false;		// 不是合法的Milkshape3D模型

		if (pHeader->version<3 || pHeader->version>4)
			return false;		// 仅仅支持1.3和1.4的格式

		// 读取并顶点信息
		int nVertices = *(word*)pPtr;
		pPtr += sizeof(word);
		Ms3dVertex*	pVertex = (Ms3dVertex*)pPtr;
		pPtr += sizeof(Ms3dVertex) * nVertices;

		// 读取索引信息
		int nTriangles = *(word*)pPtr;
		pPtr += sizeof(word);
		Ms3dTriangle* pTriangle = (Ms3dTriangle*)pPtr;
		pPtr += sizeof(Ms3dTriangle) * nTriangles;

		int nGroups = *(word*)pPtr;
		pPtr += sizeof(word);

		for (int i=0; i<nGroups; i++)
		{
			pPtr += sizeof(byte);	// flags
			pPtr += 32;				// name

			word nGroupTriangles = *(word*)pPtr;
			pPtr += sizeof(word);

			MeshElementBuilder builder;

			for (int j=0; j<nGroupTriangles; j++)
			{
				int index = *(word*)pPtr;

				Ms3dTriangle* tri = &pTriangle[index];
				for (int k=0; k<3; k++)
				{
					float uv[2] = { tri->u[k], tri->v[k] };
					builder.AddVertex(pVertex[tri->vertexIndices[k]].pos,
						tri->vertexNormals[k],
						uv);
				}

				pPtr += sizeof(word);
			}

			MeshElement* elem = new MeshElement();
			elem->CreateMeshData(&builder);
			AddMeshElement(elem);

			char materialIndex = *(char*)pPtr;
			pPtr += sizeof(char);

		}

		// TODO: 在这里为模型指定材质信息
		int nMaterials = *(word*)pPtr;
		pPtr += sizeof(word);

		for (int i=0; i<nMaterials; i++)
		{
			Ms3dMaterial* pMaterial = (Ms3dMaterial*)pPtr;
			pPtr += sizeof(Ms3dMaterial);
		}

		delete [] pBuffer;

		return true;
	}
}
