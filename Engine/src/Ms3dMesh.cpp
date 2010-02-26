//-----------------------------------------------------------------------------------
/// Ms3dMesh.cpp ms3d网格类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Ms3dMesh.h"
#include "FileSystem.h"
#include "Material.h"
#include "TextureManager.h"

namespace Gen
{
	Ms3dMesh::Ms3dMesh(const String& filename)
		: BaseMesh(filename)
	{
	}

	bool Ms3dMesh::LoadImpl()
	{
		BaseMesh::LoadImpl();

		ifstream inputFile(m_Filename.Data(), ios::in|ios::binary);

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

		// 读取分组信息
		int nGroups = *(word*)pPtr;
		pPtr += sizeof(word);

		// 使用MeshBuilder构造模型
		// TODO: 考虑模型的材质分组
		MeshBuilder builder(nGroups);

		// 记录材质列表和材质id列表
		vector<Material*>	materialList;
		vector<char>		materialIdList;

		for (int i=0; i<nGroups; i++)
		{
			pPtr += sizeof(byte);	// flags
			pPtr += 32;				// name

			word nGroupTriangles = *(word*)pPtr;
			pPtr += sizeof(word);

			for (int j=0; j<nGroupTriangles; j++)
			{
				int index = *(word*)pPtr;

				Ms3dTriangle* tri = &pTriangle[index];
				for (int k=0; k<3; k++)
				{
					float uv[2] = { tri->u[k], 1.0f - tri->v[k] };
					builder.AddVertex(pVertex[tri->vertexIndices[k]].pos,
									  tri->vertexNormals[k],
									  uv, i);
				}

				pPtr += sizeof(word);
			}

			//MeshElement* elem = new MeshElement(this);
			//elem->CreateMeshData(&builder);
			//AddMeshElement(elem);

			char materialIndex = *(char*)pPtr;
			materialIdList.push_back(materialIndex);
			pPtr += sizeof(char);

		}

		BuildMesh(&builder);

		// TODO: 在这里为模型指定材质信息
		int nMaterials = *(word*)pPtr;
		pPtr += sizeof(word);

		for (int i=0; i<nMaterials; i++)
		{
			Ms3dMaterial* pMaterial = (Ms3dMaterial*)pPtr;

			String matOriginalName;
			matOriginalName.Format("%d_%s", i, pMaterial->name);

			// 为模型创建材质
			String matName = m_ResourceName + CORRECT_SLASH + matOriginalName;

			// 创建一个新材质
			Material* material = MaterialManager::Instance().Create(matName);

			material->SetAmbient(Color4f(pMaterial->ambient));
			material->SetDiffuse(Color4f(pMaterial->diffuse));
			material->SetEmissive(Color4f(pMaterial->emissive));
			material->SetSpecular(Color4f(pMaterial->specular));
			material->SetSpecularLevel(pMaterial->shininess);

			if (String(pMaterial->texture)!=String(""))
			{

				String texResName = CanonicalizePath(pMaterial->texture);
				size_t pos = m_ResourceName.FindLastOf(CORRECT_SLASH);
				String path = "";
				if (pos!=String::npos)
					path = m_ResourceName.Substr(0, pos + 1);
				texResName = path + texResName;

				BaseTexture* tex = TextureManager::Instance().GetByName(texResName);
				if (tex)
				{
					tex->Trigger();
					material->GetTextureRenderState()->texture = tex->GetDeviceTexture();
				}
				material->GetTextureRenderState()->textureName = texResName;
			}

			materialList.push_back(material);

			pPtr += sizeof(Ms3dMaterial);
		}

		// 分配材质
		for (size_t i=0; i<materialIdList.size(); i++)
		{
			m_Materials.push_back(materialList[materialIdList[i]]);
		}

		delete [] pBuffer;

		return true;
	}
}
