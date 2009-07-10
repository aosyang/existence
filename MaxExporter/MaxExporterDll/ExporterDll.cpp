#include "Max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "igame.h"
#include "igameobject.h"
#include "IGameModifier.h"
#include "INode.h"

#include "VertexInfo.h"

#include <map>
#include <fstream>

using namespace std;

void WriteMesh(ofstream* out, int index, IGameMaterial* material, IGameMesh* mesh);

// 版本号 0.10
#define EMDL_VERSION 10

// TODO: 使用与Mesh共同的头文件定义这些枚举值
enum EMDL_LUMP
{
	EMDL_LUMP_TEXTURES = 100,
	EMDL_LUMP_MESH_ELEMENTS,
};

TCHAR matFilename[1024];

void WriteBoneNodes(ofstream& fout, IGameNode* node, GMatrix& parentMat, TimeValue t, int level = 0)
{
	//for (int i=0; i<level; i++) fout << "\t";
	//fout << node->GetName() << endl;

	for (int i=0; i<level; i++) fout << "\t";
	fout << node->GetChildCount() << endl;

	GMatrix local = node->GetWorldTM(t);
	GMatrix mat = local/* * parentMat.Inverse()*/;

	//mat.Inverse

	for (int i=0; i<level; i++) fout << "\t";
	fout << mat[0].x << " " << mat[1].x << " " << mat[2].x << " " << mat[3].x << endl;
	for (int i=0; i<level; i++) fout << "\t";
	fout << mat[0].y << " " << mat[1].y << " " << mat[2].y << " " << mat[3].y << endl;
	for (int i=0; i<level; i++) fout << "\t";
	fout << mat[0].z << " " << mat[1].z << " " << mat[2].z << " " << mat[3].z << endl;
	for (int i=0; i<level; i++) fout << "\t";
	fout << mat[0].w << " " << mat[1].w << " " << mat[2].w << " " << mat[3].w << endl;

	for (int i=0; i<node->GetChildCount(); i++)
	{
		WriteBoneNodes(fout, node->GetNodeChild(i), local, t, level+1);
	}

}


void WriteMeshSkinInfo(ofstream& fout, IGameSkin* skin)
{
	GMatrix skinTM;
	skin->GetInitSkinTM(skinTM);

	for (int v=0; v<skin->GetNumOfSkinnedVerts(); v++)
	{
		fout << "顶点序号: " << v << endl;

		// 每个顶点受到多个骨骼影响
		// TODO 每顶点超出4个权重需要控制在4个以内
		for (int b=0; b<skin->GetNumberOfBones(v); b++)
		{
			IGameNode* node = skin->GetIGameBone(v, b);
			unsigned long boneId = skin->GetBoneID(v, b);
			float weight = skin->GetWeight(v, b);

			fout << "  骨骼: " << boneId << "  权重: " << weight << " Name: " << node->GetName() << endl;
		}
	}
}

extern "C" __declspec(dllexport) int DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{

	IGameScene * pIgame = GetIGameInterface();

	UserCoord Whacky = {
		1,	//Right Handed
		1,	//X axis goes right
		2,	//Y Axis goes up
		5,	//Z Axis goes out.
		1,	//U Tex axis is right
		0,  //V Tex axis is up
	};	
	IGameConversionManager * cm = GetConversionManager();
	cm->SetUserCoordSystem(Whacky);

	pIgame->InitialiseIGame(false);
	pIgame->SetStaticFrame(0);

	locale   langLocale("");
	setlocale(LC_ALL, langLocale.name().data());

	ofstream out;
	out.open(name, ios_base::out|ios_base::binary);

	string filename(name);
	string path;
	size_t pos = filename.find_last_of('\\');
	if (pos != string::npos)
	{
		path = filename.substr(0, pos + 1);
		filename = filename.substr(pos + 1);
	}

	pos = filename.find_last_of('.');
	if (pos != string::npos)
		filename = filename.substr(0, pos);

	filename += ".emt";

	strcpy(matFilename, filename.data());


	// 写入文件头
	out.write("EMDL", 4);

	// 写入版本号
	unsigned int ver = EMDL_VERSION;
	out.write((char*)&ver, sizeof(ver));

	ofstream tempout("E:/testout.txt");

	// 遍历场景中所有节点
	for(int loop = 0; loop < pIgame->GetTopLevelNodeCount(); loop++)
	{
		IGameNode *pGameNode = pIgame->GetTopLevelNode(loop);
		IGameObject *pObj;
		pObj = pGameNode->GetIGameObject();

		////动画关键帧参考
		//IGameKeyTab key;

		//IGameControl* pCtrl = pGameNode->GetIGameControl();
		//pCtrl->GetFullSampledKeys(key, 25, IGAME_TM, false);
		//key[0].

		switch(pObj->GetIGameType())
		{
		case IGameObject::IGAME_MESH:
			{
				pObj->InitializeData();

				// 获取材质
				IGameMaterial *pMaterial = NULL;
				pMaterial = pGameNode->GetNodeMaterial();

				// 获取Mesh
				IGameMesh *pMesh = (IGameMesh*)pObj;

				//here we extract info from mesh
				WriteMesh(&out, loop, pMaterial, pMesh);

				// 导出蒙皮信息
				int modifierNum = pObj->GetNumModifiers();
				for (int i=0; i<modifierNum; i++)
				{
					IGameModifier* modif = pObj->GetIGameModifier(i);

					// 如果不是蒙皮修改器，继续找下一个
					if (!modif->IsSkin()) continue;

					ofstream fskin("E:/skin.txt");
					WriteMeshSkinInfo(fskin, static_cast<IGameSkin*>(modif));
					fskin.close();

					break;
				}

				break;
			}
		case IGameObject::IGAME_BONE:
			{
				static bool doneWithBone = false;

				if (!doneWithBone)
				{
					//int ticks = pIgame->GetSceneTicks();
					//pIgame->GetSceneStartTime();

					ofstream fscene("E:/scene.txt");

					// 每帧tick数，在FPS 30情况下是160，FPS 15时为320
					// 每秒4800ticks
					fscene << "Scene Ticks: " << pIgame->GetSceneTicks() << endl;
					fscene << "Start time: " << pIgame->GetSceneStartTime() << endl;
					fscene << "End time: " << pIgame->GetSceneEndTime() << endl;
					fscene.close();

					// TODO: 计算需要导出的帧数和采样频率
					//		 计算导出动画的播放速度
					
					tempout << 100 << endl << endl;

					GMatrix mat;
					mat.SetIdentity();
					for (int i=0; i<100; i++)
						WriteBoneNodes(tempout, pGameNode, mat, i * 160);
					doneWithBone = true;
				}
				break;
			}
		}
	}

	tempout.close();

	out.close();

	pIgame->ReleaseIGame();

	return TRUE;
}


void WriteMesh(ofstream* out, int index, IGameMaterial* material, IGameMesh* mesh)
{
	//if (material)
	//{
	//	*out << material->GetMaterialName() << " ";
	//	*out << material->GetSubMaterialCount() << endl;
	//}

	//material->GetSubMaterialCount

	int totalFaceNum = mesh->GetNumberOfFaces();

	typedef map<unsigned int, VertexInfo> VertexMap;


	if (material)
	{
		// 输出材质路径，临时
		ofstream fout(matFilename);

		map<int, int> matIdList;
		int matNum = 0;

		int submatNum = material->GetSubMaterialCount();

		// 子材质
		if (submatNum)
		{
			// 修正所有平面使用的matID
			for (int i=0; i<totalFaceNum; i++)
			{
				FaceEx* face = mesh->GetFace(i);
				face->matID %= submatNum;
			}

			// 首先，遍历所有表面，确定材质数目
			for (int i=0; i<totalFaceNum; i++)
			{
				int matID = mesh->GetFaceMaterialID(i) % submatNum;
				if (matIdList.find(matID)==matIdList.end())
				{
					// Error
					matIdList[matID] = matNum++;
					//fout << matID << endl;
					//fout << material->GetSubMaterial(matID)->GetMaterialName() << endl;
					if (material->GetSubMaterial(matID)->GetIGameTextureMap(0))
					{
						fout << material->GetSubMaterial(matID)->GetMaterialName() << "\t\t= ";
						fout << material->GetSubMaterial(matID)->GetIGameTextureMap(0)->GetBitmapFileName() << endl;
					}

				}
				//mesh_mat->getid
			}

		}
		else	// 没有子材质
		{
			matNum = 1;

			for (int i=0; i<totalFaceNum; i++)
			{
				FaceEx* face = mesh->GetFace(i);
				face->matID = 0;
			}

			if (material->GetIGameTextureMap(0))
			{
				fout << material->GetMaterialName() << "\t\t= ";
				fout << material->GetIGameTextureMap(0)->GetBitmapFileName() << endl;
			}
		}
		fout.close();

		//for (map<int, int>::iterator iter=matIdList.begin(); iter!=matIdList.end(); iter++)
		//material->GetSubMaterial(1)->GetMaterialName();

		// TODO: 保存所有材质
		unsigned int mat_header = EMDL_LUMP_TEXTURES;
		out->write((char*)&mat_header, sizeof(mat_header));

		// 输出材质数(即子模型数目)
		out->write((char*)&matNum, sizeof(matNum));

		for (int m=0; m<matNum; m++)
			//for (map<int, int>::iterator iter=matIdList.begin(); iter!=matIdList.end(); iter++)
		{
			//int count = material->GetSubMaterialCount();

			string matName;
			if (matNum==1 && !material->GetSubMaterial(m))
				matName = material->GetMaterialName();
			else
				matName= material->GetSubMaterial(m)->GetMaterialName();
			unsigned int len = matName.size();
			out->write((char*)&len, sizeof(len));

			out->write(matName.c_str(),  sizeof(char) * len);
		}

		// Mesh单元
		unsigned int submesh_header = EMDL_LUMP_MESH_ELEMENTS;
		out->write((char*)&submesh_header, sizeof(submesh_header));

		for (int m=0; m<matNum; m++)
		{
			// 根据材质，确定MeshElement顺序
			Tab<FaceEx*> faceTab = mesh->GetFacesFromMatID(m);

			int faceNum = faceTab.Count();
			VertexMap mapVert;
			//vector<unsigned int>	vecIndex;
			unsigned int*	vecIndex = new unsigned int[faceNum * 3];
			int vertNum = 0;

			for (int i=0; i<faceTab.Count(); i++)
			{
				FaceEx* face = faceTab[i];

				// 遍历每个多边形的所有顶点
				for (int v=0; v<3; v++)
				{
					VertexInfo vert;

					Point3 vertex = mesh->GetVertex(face->vert[v], true);
					Point3 normal = mesh->GetNormal(face->norm[v], true);
					Point2 texcoord = mesh->GetTexVertex(face->texCoord[v]);

					vert.x = vertex.x;
					vert.y = vertex.y;
					vert.z = vertex.z;

					vert.nx = normal.x;
					vert.ny = normal.y;
					vert.nz = normal.z;

					vert.u = texcoord.x;
					vert.v = texcoord.y;

					//vecIndex.push_back(vertNum);
					vecIndex[i * 3 + v] = vertNum;
					mapVert[vertNum] = vert;
					vertNum++;
				}
			}

			VertexInfo* vertices = new VertexInfo[mapVert.size()];

			for (VertexMap::iterator iter = mapVert.begin(); iter != mapVert.end(); iter++)
			{
				unsigned int index = iter->first;
				vertices[index] = iter->second;
			}

			// 导出面
			out->write((char*)&faceNum, sizeof(faceNum));
			out->write((char*)vecIndex, sizeof(unsigned int) * 3 * faceNum);

			// 导出顶点
			out->write((char*)&vertNum, sizeof(vertNum));
			out->write((char*)&(vertices[0]), sizeof(VertexInfo) * mapVert.size());

			delete [] vecIndex;
			delete [] vertices;

		}
	}
	else	// 没有材质的模型
	{

		unsigned int submesh_header = EMDL_LUMP_MESH_ELEMENTS;
		out->write((char*)&submesh_header, sizeof(submesh_header));

		VertexMap mapVert;
		//vector<unsigned int>	vecIndex;
		unsigned int*	vecIndex = new unsigned int[totalFaceNum * 3];
		int vertNum = 0;

		// 根据材质组导出面信息
		//   导出材质组数量

		// TODO: 根据材质ID获取多边形
		//Tab<FaceEx*> faceTab = mesh->GetFacesFromMatID(0);
		//int n = faceTab.Count();
		//for (int i=0; i<n; i++)
		//{
		//	faceNum = faceTab.Count();
		//	FaceEx* face = faceTab[i];

		//	// 输出：
		//	//  材质ID
		//	//  使用该材质的多边形数量
		//	//  使用该材质的多边形列表
		//}

		// 遍历所有多边形(三角面)
		for (int i=0; i<totalFaceNum; i++)
		{
			FaceEx* face = mesh->GetFace(i);

			// 遍历每个多边形的所有顶点
			for (int v=0; v<3; v++)
			{
				VertexInfo vert;


				Point3 vertex = mesh->GetVertex(face->vert[v], true);
				Point3 normal = mesh->GetNormal(face->norm[v], true);
				Point2 texcoord = mesh->GetTexVertex(face->texCoord[v]);

				vert.x = vertex.x;
				vert.y = vertex.y;
				vert.z = vertex.z;

				vert.nx = normal.x;
				vert.ny = normal.y;
				vert.nz = normal.z;

				vert.u = texcoord.x;
				vert.v = texcoord.y;

				//vecIndex.push_back(vertNum);
				vecIndex[i * 3 + v] = vertNum;
				mapVert[vertNum] = vert;
				vertNum++;

			}
		}

		VertexInfo* vertices = new VertexInfo[mapVert.size()];

		for (VertexMap::iterator iter = mapVert.begin(); iter != mapVert.end(); iter++)
		{
			unsigned int index = iter->first;
			vertices[index] = iter->second;
		}

		// 导出面
		out->write((char*)&totalFaceNum, sizeof(totalFaceNum));
		out->write((char*)vecIndex, sizeof(unsigned int) * 3 * totalFaceNum);

		// 导出顶点
		out->write((char*)&vertNum, sizeof(vertNum));
		out->write((char*)&(vertices[0]), sizeof(VertexInfo) * mapVert.size());

		delete [] vecIndex;
		delete [] vertices;
	}
}

//
//void WriteMesh(ofstream* out, int index, IGameMaterial* material, IGameMesh* mesh)
//{
//	//*out << index << ":";
//
//	//if (material)
//	//{
//	//	*out << material->GetMaterialName() << " ";
//	//	*out << material->GetSubMaterialCount() << endl;
//	//}
//
//	//*out << " ";
//	//*out << "Num of verts:" << mesh->GetNumberOfVerts() << " ";
//
//	//*out << " ----FACES----" << endl;
//
//	int faceNum = mesh->GetNumberOfFaces();
//	//*out << "Face num:" << faceNum << endl;
//
//	typedef map<unsigned int, VertexInfo> VertexMap;
//
//	VertexMap mapVert;
//	vector<unsigned int>	vecIndex;
//	int vertNum = 0;
//
//	// 遍历所有多边形(三角面)
//	for (int i=0; i<faceNum; i++)
//	{
//		FaceEx* face = mesh->GetFace(i);
//		//*out << " " << i << ":" << endl;
//
//		// 遍历每个多边形的所有顶点
//		for (int v=0; v<3; v++)
//		{
//			VertexInfo vert;
//
//			Point3 vertex = mesh->GetVertex(face->vert[v], true);
//			Point3 normal = mesh->GetNormal(face->norm[v], true);
//			Point2 texcoord = mesh->GetTexVertex(face->texCoord[v]);
//
//			vert.x = vertex.x;
//			vert.y = vertex.y;
//			vert.z = vertex.z;
//
//			vert.nx = normal.x;
//			vert.ny = normal.y;
//			vert.nz = normal.z;
//
//			vert.u = texcoord.x;
//			vert.v = texcoord.y;
//
//			// TODO: 由于查找相同顶点算法速度过慢，目前不采用。需要一个更快的比较方法
//
//			// 查找该顶点是否已经存在于列表中
//			//VertexMap::iterator iter;
//			//for (iter = mapVert.begin(); iter != mapVert.end(); iter++)
//			//{
//			//	//iter = mapVert.find(vert);
//			//	if (iter->second == vert)
//			//		break;
//			//}
//
//			//if (iter != mapVert.end())
//			//{
//			//	// 若表中已包含该顶点信息
//			//	vecIndex.push_back(iter->first);
//			//}
//			//else
//			//{
//				vecIndex.push_back(vertNum);
//				mapVert[vertNum] = vert;
//				vertNum++;
//			//}
//
//			//*out << " Vertex: " << vertex.x << " " << vertex.y << " " << vertex.z << endl;
//			//*out << " " << face->vert[v];
//		}
//		//*out << endl;
//	}
//
//	VertexInfo* vertices = new VertexInfo[mapVert.size()];
//
//	for (VertexMap::iterator iter = mapVert.begin(); iter != mapVert.end(); iter++)
//	{
//		unsigned int index = iter->first;
//		vertices[index] = iter->second;
//	}
//
//	//*out << "Face num:";
//	*out << faceNum << endl;
//	
//	for (vector<unsigned int>::iterator iter = vecIndex.begin(); iter != vecIndex.end();)
//	{
//		*out << *(iter++) << " " << *(iter++) << " " << *(iter++) << endl;
//	}
//
//	*out << endl;
//
//	//*out << "Vertex:" << endl;
//	*out << vertNum << endl;
//
//	for (unsigned int i=0; i<vertNum; i++)
//	{
//		VertexInfo* info = &(vertices[i]);
//		*out << info->x << " " << info->y << " " << info->z << " "
//			<< info->nx << " " << info->ny << " " << info->nz << " "
//			<< info->u << " " << info->v << endl;
//	}
//
//	delete [] vertices;
//
///*
//	*out << endl << " ----Vertexes----" << endl;
//
//	int vertNum = mesh->GetNumberOfVerts();
//	*out << "Vert num:" << vertNum << endl;
//
//	for (int i=0; i<vertNum; i++)
//	{
//		Point3 vertex = mesh->GetVertex(i, true);
//		*out << " " << i << ": "<< vertex.x << " " << vertex.y << " " << vertex.z << endl;
//	}
//*/
//}
