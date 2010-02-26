//-----------------------------------------------------------------------------------
/// EmdMesh.cpp Emd������ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "EmdMesh.h"
#include "Skeleton.h"
#include "MeshElement.h"
#include <fstream>

using namespace std;

namespace Gen
{
	EmdMesh::EmdMesh(const String& filename)
		: BaseMesh(filename)
	{
	}

	EmdMesh::~EmdMesh()
	{
	}

	bool EmdMesh::LoadImpl()
	{
		BaseMesh::LoadImpl();

		// TODO: �ļ���ȡ���ڴ��У�ͨ��ָ�����
		ifstream fin;
		fin.open(m_Filename.Data(), ios_base::in|ios_base::binary);

		if (!fin.is_open())
			return false;

		char header[4];
		fin.read((char*)header, 4);

		if (strncmp(header, "EMDL", 4)!=0)		// No header, maybe old format
		{
			//Log.OutputTime();
			//Log << "Loading mesh with no version info found.\n";
			//fin.seekg(ios_base::beg);
			AssertFatal(0, "EmdMesh::LoadFromFile(): No header found");
			//LoadMesh_NoVersion(fin);
		}
		else
		{
			unsigned int ver;
			fin.read((char*)&ver, sizeof(ver));

			// �汾�ż��
			// 10����0.10�棬�Դ�����
			Log.OutputTime();
			switch (ver)
			{
			//case 10:
			//	Log << "EMD mesh version 0.10.\n";
			//	if (!LoadMesh_010(fin)) return false;
			//	break;
			case 20:
				Log << "EMD mesh version 0.20.\n";
				if (!LoadMesh_020(fin)) return false;
				break;
			default:
				// unsupported version
				AssertFatal(0, "EmdMesh::LoadFromFile() : Unsupported mesh version.");
				break;
			}
		}

		fin.close();

		// ������Ƥ��Ϣ(*.esk)�Լ�������Ϣ(*.ebh)
		SAFE_DELETE_ARRAY(m_Skin);

		// ��ԭʼģ�������ͬ������Ƥ��Ϣ
		String skinName = m_Filename;
		skinName.ToLowerCase();
		size_t pos = skinName.FindLastOf("emd");
		skinName = m_Filename;
		skinName.Replace(pos - 2, 3, "esk");

		ifstream skinfin(skinName.Data());

		// û����Ƥ������
		if (skinfin.is_open())
		{
			int skinnedVertCount;
			skinfin >> skinnedVertCount;

			m_Skin = new SkinnedVertexInfo[skinnedVertCount];

			for (int i=0; i<skinnedVertCount; i++)
			{
				int boneCount;
				skinfin >> boneCount;

				m_Skin[i].boneCount = boneCount;

				// ��ȡÿ�����������Ȩ��
				for (int j=0; j<boneCount; j++)
				{
					int boneId;
					float weight;

					skinfin >> boneId;
					skinfin >> weight;

					m_Skin[i].boneId[j] = boneId;
					m_Skin[i].weight[j] = weight;
				}

			}

			skinfin.close();
		}

		return true;
	}

	// ����0.20�汾��mesh��ʽ
	bool EmdMesh::LoadMesh_020(ifstream& fin)
	{
		unsigned int materialCount = 1;	// ע������û�в��ʵ�ģ�ͣ�Ĭ����ģ������Ϊ1
		unsigned int lump;
		fin.read((char*)&lump, sizeof(lump));

		while (!fin.eof())
		{
			switch (lump)
			{
			case EMDL_LUMP_MATERIALS:
				{
					fin.read((char*)&materialCount, sizeof(materialCount));

					for (unsigned int i=0; i<materialCount; i++)
					{
						char matName[256];
						unsigned int len;
						fin.read((char*)&len, sizeof(len));

						fin.read(matName, sizeof(char) * len);
						matName[len] = 0;

						Material* mat;
						mat = MaterialManager::Instance().GetByName(matName);

						AddMaterial(mat);
					}

					break;
				}
			case EMDL_LUMP_MESH_ELEMENTS:
				{
					// ���������meshԪ��
					for (unsigned int i=0; i<materialCount; i++)
					{
						int faceCount;
						fin.read((char*)&faceCount, sizeof(faceCount));

						unsigned int* indexArray = new unsigned int[faceCount*3];

						fin.read((char*)indexArray, sizeof(unsigned int) * faceCount * 3);

						MeshElement* elem = new MeshElement(this);
						elem->CreateMeshData(faceCount, indexArray);

						AddMeshElement(elem);

						SAFE_DELETE_ARRAY(indexArray);
					}
					break;
				}
			case EMDL_LUMP_VERTICES:
				{
					fin.read((char*)&m_VertexNum, sizeof(m_VertexNum));

					m_VertexArray = new float[m_VertexNum * 3];
					m_NormalArray = new float[m_VertexNum * 3];
					m_TexcoordArray = new float[m_VertexNum * 2];

					float* data = new float[m_VertexNum * 8];
					fin.read((char*)data, sizeof(float) * m_VertexNum * 8);

					int offset = 0;
					float maxSquaredLength = 0.0f;
					for (int i=0; i<m_VertexNum; i++)
					{

						memcpy(&m_VertexArray[i * 3], &data[offset], sizeof(float) * 3);
						offset += 3;
						memcpy(&m_NormalArray[i * 3], &data[offset], sizeof(float) * 3);
						offset += 3;
						memcpy(&m_TexcoordArray[i * 2], &data[offset], sizeof(float) * 2);
						offset += 2;

						float squaredLen = Vector3f(&m_VertexArray[i * 3]).SquaredLength();
						if (squaredLen>maxSquaredLength)
							maxSquaredLength = squaredLen;
					}

					m_BoundingRadius = sqrt(maxSquaredLength);

					// �������㻺��
					m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0,
												 m_VertexArray,
												 m_NormalArray,
												 NULL,
												 m_TexcoordArray,
												 m_VertexNum);
					SAFE_DELETE_ARRAY(data);
				}
				break;
			default:
				Log.Warning("EmdMesh::LoadMesh_020(): Invalid lump from file.");
				return false;
				break;
			}

			fin.read((char*)&lump, sizeof(lump));
		}

		UpdateElementsOBB();

		return true;

	}

}
