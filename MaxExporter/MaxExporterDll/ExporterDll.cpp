#include "Max.h"
//#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

// IGame headers
#include "IGame.h"
#include "IGameModifier.h"

#include "INode.h"
#include "iparamb2.h"
#include "ISkin.h"
#include <modstack.h>

//#include "decomp.h"

#include "VertexInfo.h"

#include <map>
#include <vector>
#include <fstream>
#include <stdio.h>
using namespace std;

#include <Material.h>
#include <EmdMesh.h>

//#define USE_ABSOLUTE_MATRIX

#pragma warning(disable:4996)

// Mesh版本
#define EMDL_VERSION 20


// 记录骨骼的初始矩阵
typedef map<int, GMatrix>	BoneInitMatrix;

// 获取采样关键帧，每4帧进行一次采样
static int sampleRate = 4;

// 记录骨骼动画实际导出的骨骼数量
static int exportedBoneCount;

// 每帧tick数目
int g_TicksPerFrame;

string g_PathName;

void ExportKeyFrame(IGameNode* node);

void ExportNode_r(IGameNode* node);

bool ExportBoneHierarchy_r(IGameNode* bone, ISkin* skin, ofstream& fout, BoneInitMatrix& initMatrixMap, int parentId=-1, GMatrix* parentMatrix=NULL, int level=0);
void ExportBoneHierarchy(IGameNode* node, BoneInitMatrix& initMatrixMap);

void ExportBoneHierarchyAnimation_r(IGameNode* node, IGameSkin* skin, ofstream& fout, BoneInitMatrix& initMatrixMap, int parentId=-1, IGameKeyTab* parentKey=NULL, int level=0);
void ExportBoneHierarchyAnimation(IGameNode* node, IGameSkin* skin, BoneInitMatrix& initMatrixMap);

IGameSkin* GetIGameSkin(IGameNode* node);

void ExportMesh(IGameNode* node);

void ExportMaterial(IGameMaterial* material);

// 对齐用，输出tab
void OutputTab(ofstream& fout, int num)
{
	for (int i=0; i<num; i++)
		fout << "\t";
}

// 导出关键帧
void ExportKeyFrame(IGameNode* node)
{
	IGameKeyTab posKey;
	IGameKeyTab rotKey;
	IGameControl* control = node->GetIGameControl();

	if (!control)
	{
		return;
	}

	if (control->GetFullSampledKeys(posKey, sampleRate, IGAME_POS) &&
		control->GetFullSampledKeys(rotKey, sampleRate, IGAME_ROT))
	{
		//static int index = 0;
		char filename[256];
		sprintf(filename, "%ssample_%s.txt", g_PathName.data(), node->GetName());
		ofstream fout(filename);

		// 关键帧数量
		fout << posKey.Count() << endl;

		for (int i=0; i<posKey.Count(); i++)
		{
			// 写入坐标值
			Point3 k = posKey[i].sampleKey.pval;
			fout << k.x << " " << k.y << " " << k.z << endl;

			// 写入旋转值
			// Note: Max中四元数x y z方向与实际相反，需要取负
			Quat q = rotKey[i].sampleKey.qval;
			fout << -q.x << " " << -q.y << " " << -q.z << " " << q.w << endl;
		}
		fout.close();
	}
}

// 递归导出节点
void ExportNode_r(IGameNode* node)
{
	// 只有在调用了GetIGameObject方法后才能获取正确的IGameControl
	// But why?
	IGameObject* obj = node->GetIGameObject();

	// 忽略目标节点
	// 目标节点是指摄像机和灯光等的控制面对方向用的节点，
	// 通常这个节点不需要导出
	if (node->IsTarget())
		return;

	// 导出结点的关键帧动画
	//ExportKeyFrame(node);

	for (int i=0; i<node->GetChildCount(); i++)
	{
		ExportNode_r(node->GetNodeChild(i));
	}
}
//
//void ExportBoneHierarchy_r(IGameNode* node, ofstream& fout, int parentId=-1, IGameKeyTab* parentKey=NULL, int level=0)
//{
//	IGameObject* obj = node->GetIGameObject();
//
//	IGameKeyTab	matKey;
//	//IGameKeyTab posKey;
//	//IGameKeyTab rotKey;
//
//	//static int boneId = 0;
//	int id = node->GetNodeID();
//
//
//}

ISkin* GetSkinModifier(INode* pINode)
{
	Object* pObject = pINode->GetObjectRef();
	if (!pObject) return NULL;

	while (pObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* pDerivedObject = (IDerivedObject*)pObject;

		for (int stackId=0; stackId<pDerivedObject->NumModifiers(); stackId++)
		{
			Modifier* pModifier = pDerivedObject->GetModifier(stackId);

			// 判断是否为Skin修改器
			if (pModifier->ClassID() == SKIN_CLASSID)
			{
				return (ISkin*)pModifier->GetInterface(I_SKIN);
			}

			pObject = pDerivedObject->GetObjRef();
		}
	}

	return NULL;
}

bool ExportBoneHierarchy_r(IGameNode* bone, ISkin* skin, ofstream& fout, BoneInitMatrix& initMatrixMap, int parentId, GMatrix* parentMatrix, int level)
{
	bone->GetIGameObject()->InitializeData();
	INode* maxNode = bone->GetMaxNode();

	// 获取骨骼初状态矩阵
	Matrix3 boneInitMat;

#if 0
	char buf[256];

	sprintf(buf, "Bone name: %s\nId: %d", bone->GetName(), bone->GetNodeID());
	MessageBox(NULL, buf, "Bone", MB_OK);
#endif

	int id = bone->GetNodeID();

	GMatrix initGMatrix,		// 该骨骼相对于父节点的初始状态矩阵
			boneGMat;			// 该骨骼当前的世界矩阵

	exportedBoneCount++;

	// 如果根节点是一个不属于蒙皮的结点，忽略之
	// 注：如果骨骼继承关系中存在不属于蒙皮的结点，这样方法会造成潜在错误
	if (skin->GetBoneInitTM(maxNode, boneInitMat, true))
	{
		//skin->GetInitSkinTM(boneInitMat);

		// 标准化
		for (int i=0; i<3; i++)
		{
			Point3 p = boneInitMat.GetRow(i).Normalize();
			boneInitMat.SetRow(i, p);
		}

	}
	else
	{
		// TODO: 没有蒙皮的结点使用结点世界变换作为初状态矩阵
		boneInitMat = bone->GetWorldTM().ExtractMatrix3();
	}

	boneGMat = boneInitMat;

	// 将矩阵转换为引擎的坐标系
	// Note: 这里只是将坐标系变换成与动画关键帧一致
	Quat q = boneGMat.Rotation();
	Quat r;
	r.SetEuler(-PI/2.0f, 0.0f, 0.0f);
	q *= r;

	Point3 v = boneInitMat.GetTrans();
	v = Point3(v.x, v.z, -v.y);
	boneInitMat.SetRotate(q);
	//Point3 px = boneInitMat.GetRow(0);
	Point3 py = boneInitMat.GetRow(1);
	Point3 pz = boneInitMat.GetRow(2);
	//boneInitMat.SetRow(0, -px);
	boneInitMat.SetRow(1, pz);
	boneInitMat.SetRow(2, -py);
	boneInitMat.SetRow(3, v);
	boneGMat = boneInitMat;

#ifndef USE_ABSOLUTE_MATRIX
	// 根节点取其自身坐标系
	// 其他节点取相对于父结点的坐标系
	if (parentMatrix)
	{
		initGMatrix = boneGMat * parentMatrix->Inverse();
	}
	else
#endif
	{
		initGMatrix = boneGMat;
	}

	// 保存矩阵到map中去
	initMatrixMap[id] = initGMatrix;

	// 导出骨骼编号及父节点编号
	OutputTab(fout, level);
	fout << id << endl;
	OutputTab(fout, level);
	fout << parentId << endl;

	// 骨骼名称
	//OutputTab(fout, level);
	//fout << bone->GetName() << endl;

	// 输出骨骼矩阵

	//for (int i=0; i<4; i++)
	//{
	//	Point4 p = initGMatrix.GetColumn(i);
	//	OutputTab(fout, level);
	//	fout << p.x << " " << p.y << " " << p.z << " " << p.w << endl;
	//}

	Point3 p = initGMatrix.Translation();
	OutputTab(fout, level);
	fout << p.x << " " << p.y << " " << p.z << endl;
	//fout << p.x << " " << p.z << " " << -p.y << endl;

	q = initGMatrix.Rotation();

	OutputTab(fout, level);
	fout << -q.x << " " << -q.y << " " << -q.z << " " << q.w << endl;


	for (int i=0; i<bone->GetChildCount(); i++)
	{
		IGameNode* child = bone->GetNodeChild(i);
		if (child->GetIGameObject()->GetIGameType()==IGameObject::IGAME_BONE)
		{
			//if (!ExportBoneHierarchy_r(bone->GetNodeChild(i), skin, fout, id, &boneGMat, level+1))
			//	return false;
			ExportBoneHierarchy_r(bone->GetNodeChild(i), skin, fout, initMatrixMap, id, &boneGMat, level+1);
		}
	}
	return true;
}

IGameNode* GetRootBone(IGameSkin* skin)
{
	for (int i=0; i<skin->GetTotalSkinBoneCount(); i++)
	{
		IGameNode* bone = skin->GetIGameBone(i);

		if (!bone) break;

		// 向上找到这组骨骼的根节点
		IGameNode* parent = bone->GetNodeParent();
		IGameObject::ObjectTypes type = parent->GetIGameObject()->GetIGameType();

		// 如果存在父节点，且父节点是个骨骼结点
		while(parent && parent->GetIGameObject()->GetIGameType()==IGameObject::IGAME_BONE)
		{
			bone = bone->GetNodeParent();
			parent = bone->GetNodeParent();
			
			int w = 0;
		}

		const char* n = bone->GetName();

		return bone;
	}

	return NULL;
}

void ExportBoneHierarchy(IGameNode* node, BoneInitMatrix& initMatrixMap)
{
	INode* maxNode = node->GetMaxNode();
	ISkin* skin = GetSkinModifier(maxNode);
	//skin->Get
	IGameNode* bone = GetRootBone(GetIGameSkin(node));

	if (bone)
	{
		string filename = g_PathName;
		filename += node->GetName();
		filename += ".ebh";
		ofstream fout(filename.data());
		ExportBoneHierarchy_r(bone, skin, fout, initMatrixMap);
		fout.close();
	}
}


// 导出骨骼层级动画，不指定父节点表示这是个根节点
void ExportBoneHierarchyAnimation_r(IGameNode* node, IGameSkin* skin, ofstream& fout, BoneInitMatrix& initMatrixMap, int parentId, IGameKeyTab* parentKey, int level)
{
	IGameObject* obj = node->GetIGameObject();

	IGameKeyTab	matKey;
	//IGameKeyTab posKey;
	//IGameKeyTab rotKey;

	//static int boneId = 0;
	int id = node->GetNodeID();

	// 如果这个节点没有保存在map中，退出
	if (initMatrixMap.find(id)!=initMatrixMap.end())
	{

		IGameControl* control = node->GetIGameControl();

		// 对于每个骨骼，获取其位置和旋转量信息
		// 子节点需要计算基于父节点的相对坐标

		// 骨骼编号
		fout << id << endl;

		if (control->GetFullSampledKeys(matKey, sampleRate, IGAME_TM/*, false*/))
		{
			// 关键帧数量
			fout << matKey.Count() << endl;

			for (int i=0; i<matKey.Count(); i++)
			{
				GMatrix mat = matKey[i].sampleKey.gval;
				Point3 k;

				Point4 rx = mat.GetRow(0);
				Point4 ry = mat.GetRow(1);
				mat.SetRow(0, -rx);
				mat.SetRow(1, -ry);

				k = mat.Translation();

				if (parentKey)
				{
					// 如果不是根节点
					// 使用父节点的逆矩阵求出相对变换
					GMatrix parentInv = (*parentKey)[i].sampleKey.gval;
					//rx = parentInv.GetRow(0);
					//ry = parentInv.GetRow(1);
					//parentInv.SetRow(0, -rx);
					//parentInv.SetRow(1, -ry);
					parentInv = parentInv.Inverse();

					GMatrix initInv = initMatrixMap[id].Inverse();

					//mat = mat/* * parentInv*/ * initInv;

					// This hacks: 使用了逆矩阵以后坐标受到了影响，暂时这样修正
					//k.x = -k.x;
					//k.y = -k.y;
					//k.z = -k.z;
				}
				else
				{
					// 根节点矩阵使用自身的变换
					GMatrix initInv = initMatrixMap[id].Inverse();

					//mat = mat * initInv;
				}

				//AffineParts ap;
				//decomp_affine(mat.ExtractMatrix3(), &ap);

				// 写入坐标值
				//Point3 k = ap.t;
				//Point3 k = posKey[i].sampleKey.pval;
				fout << k.x << " " << k.y << " " << k.z << endl;

				// 写入旋转值
				// Note: Max中四元数x y z方向与实际相反，需要取负
				Quat q = mat.Rotation();
				//Quat q = ap.q;
				//Quat q = rotKey[i].sampleKey.qval;
				fout << -q.x << " " << -q.y << " " << -q.z << " " << q.w << endl;
			}

			//int childCount = node->GetChildCount();
			//char msg[256];
			//sprintf(msg, "Node name: %s\nChild count: %d", node->GetName(), childCount);
			//MessageBox(NULL, msg, "Tips", MB_OK);

			for (int i=0; i<node->GetChildCount(); i++)
			{
				IGameNode* child = node->GetNodeChild(i);
				if (child->GetIGameObject()->GetIGameType()==IGameObject::IGAME_BONE)
					ExportBoneHierarchyAnimation_r(node->GetNodeChild(i), skin, fout, initMatrixMap, id, &matKey, level+1);
			}
		}
	}
	else
	{
		for (int i=0; i<node->GetChildCount(); i++)
		{
			IGameNode* child = node->GetNodeChild(i);
			if (child->GetIGameObject()->GetIGameType()==IGameObject::IGAME_BONE)
				ExportBoneHierarchyAnimation_r(node->GetNodeChild(i), skin, fout, initMatrixMap, -1, NULL, level+1);
		}
	}

}

void ExportBoneHierarchyAnimation(IGameNode* node, IGameSkin* skin, BoneInitMatrix& initMatrixMap)
{
	IGameNode* rootBone = GetRootBone(skin);

	if (rootBone)
	{
		string filename = g_PathName;
		filename += "BoneAnim.txt";
		ofstream fout(filename.data());

		// 记录动画播放速度
		// 1s = 4800 ticks
		// 1ms = 4.8 ticks
		int ticksPerSample = g_TicksPerFrame * sampleRate;
		fout << ticksPerSample << endl;

		// 输出导出骨骼的数量
		fout << exportedBoneCount << endl;

		ExportBoneHierarchyAnimation_r(rootBone, skin, fout, initMatrixMap);
		fout.close();
	}
}


IGameSkin* GetIGameSkin(IGameNode* node)
{
	IGameObject* obj = node->GetIGameObject();

	for (int i=0; i<obj->GetNumModifiers(); i++)
	{
		IGameModifier* modifier = obj->GetIGameModifier(i);

		// 找到第一个蒙皮修改器
		// 注：此处在max中修改器的顺序为从上至下
		if (modifier->IsSkin())
		{
			return (IGameSkin*)modifier;
		}
	}

	return NULL;
}

// 导出模型
void ExportMesh(IGameNode* node)
{
	IGameObject* obj = node->GetIGameObject();
	obj->InitializeData();

	IGameMesh* mesh = (IGameMesh*)obj;

	IGameSkin* skin = GetIGameSkin(node);

	for (int i=0; i<obj->GetNumModifiers(); i++)
	{
		IGameModifier* modifier = obj->GetIGameModifier(i);

		// 找到第一个蒙皮修改器
		// 注：此处在max中修改器的顺序为从上至下
		if (modifier->IsSkin())
		{
			skin = (IGameSkin*)modifier;
			break;
		}
	}

	// 如果模型使用了蒙皮修改器，导出蒙皮前的模型状态
	if (skin)
		mesh = skin->GetInitialPose();

	string filename = g_PathName;
	filename += string(node->GetName());
	filename += ".emd";
	ofstream fout(filename.data(), ios_base::binary);

	// 写入文件头
	fout.write("EMDL", 4);

	// 写入版本号
	unsigned int ver = EMDL_VERSION;
	fout.write((char*)&ver, sizeof(ver));

	// 数据区域头
	unsigned int header;

	// 导出材质
	IGameMaterial* material = node->GetNodeMaterial();

	// 模型分组多边形信息
	typedef vector<Tab<FaceEx*>>	MeshElemGroup;
	MeshElemGroup	meshElementGroup;

	if (material)
	{
		int materialCount = 0;

		// 材质id列表
		map<int, int> matIdList;

		// 子材质数目
		int submatCount = material->GetSubMaterialCount();

		if (submatCount)
		{
			// 修正材质编号
			for (int i=0; i<mesh->GetNumberOfFaces(); i++)
			{
				FaceEx* face = mesh->GetFace(i);
				face->matID %= submatCount;
			}

			// 遍历所有表面，确定材质数目
			for (int i=0; i<mesh->GetNumberOfFaces(); i++)
			{
				int matId = mesh->GetFaceMaterialID(i) % submatCount;

				// 这个材质id还没有添加到列表中
				if (matIdList.find(matId)==matIdList.end())
				{
					matIdList[matId] = materialCount++;

					ExportMaterial(material->GetSubMaterial(matId));
				}
			}
		}
		else
		{
			// 具有单一材质的模型
			materialCount = 1;

			// 修整材质id
			for (int i=0; i<mesh->GetNumberOfFaces(); i++)
			{
				FaceEx* face = mesh->GetFace(i);
				face->matID = 0;
			}

			ExportMaterial(material);
		}

		for (int m=0; m<materialCount; m++)
		{
			meshElementGroup.push_back(mesh->GetFacesFromMatID(m));
		}


		// TODO: 保存所有材质
		unsigned int mat_header = Gen::EMDL_LUMP_MATERIALS;
		fout.write((char*)&mat_header, sizeof(mat_header));

		// 输出材质数(即子模型数目)
		fout.write((char*)&materialCount, sizeof(materialCount));

		for (int m=0; m<materialCount; m++)
			//for (map<int, int>::iterator iter=matIdList.begin(); iter!=matIdList.end(); iter++)
		{
			//int count = material->GetSubMaterialCount();

			string matName;
			if (materialCount==1 && !material->GetSubMaterial(m))
				matName = material->GetMaterialName();
			else
				matName= material->GetSubMaterial(m)->GetMaterialName();
			unsigned int len = matName.size();
			fout.write((char*)&len, sizeof(len));

			fout.write(matName.c_str(),  sizeof(char) * len);
		}
	}
	else
	{
		// 没有材质的对象，将所有多边形分在同一个组里
		Tab<FaceEx*> faces;

		for (int i=0; i<mesh->GetNumberOfFaces(); i++)
		{
			FaceEx* f = mesh->GetFace(i);
			faces.Append(1, &f);
		}

		meshElementGroup.push_back(faces);
	}

	// 导出子模型列表

	header = Gen::EMDL_LUMP_MESH_ELEMENTS;
	fout.write((char*)&header, sizeof(header));

	map<VertexInfo, int>		vertexIndexMap;
	map<int, VertexInfo>		indexVertexMap;
	int vertIndex = 0;

	for (MeshElemGroup::iterator iter=meshElementGroup.begin();
		 iter!=meshElementGroup.end();
		 iter++)
	{
		vector<unsigned int>					indices;


		// 面数目
		int faceCount = iter->Count();

		//fout << "Face:" << endl;
		//fout << "Count: " << faceCount << endl;
		for (int i=0; i<faceCount; i++)
		{
			FaceEx* face =(*iter)[i];

			for (int j=0; j<3; j++)
			{
				Point3 v = mesh->GetVertex(face->vert[j], true);
				Point3 n = mesh->GetNormal(face->norm[j], true);
				Point2 uv = mesh->GetTexVertex(face->texCoord[j]);

				VertexInfo vi;

				vi.x = v.x;
				vi.y = v.y;
				vi.z = v.z;
				vi.nx = n.x;
				vi.ny = n.y;
				vi.nz = n.z;
				vi.u = uv.x;
				vi.v = uv.y;
				vi.index = face->vert[j];

				map<VertexInfo, int>::iterator iter;

				// 如果相同顶点不存在存在，添加这个顶点，索引号递增
				if ((iter=vertexIndexMap.find(vi))==vertexIndexMap.end())
				{
					indexVertexMap[vertIndex] = vi;
					vertexIndexMap[vi] = vertIndex;
					indices.push_back(vertIndex++);
				}
				else
				{
					// 否则该平面使用已有顶点的索引
					indices.push_back(iter->second);
				}

			}
		}

		//fout << "------------------- Indices" << endl;

		int indexSize = (int)indices.size() / 3;

		fout.write((char*)&indexSize, sizeof(indexSize));

		// 输出索引数目
		//fout << indexSize << endl;

		// 导出多边形信息
		for (int i=0; i<indexSize; i++)
		{
			//fout << indices[i * 3] << " " << indices[i * 3 + 1] << " " << indices[i * 3 + 2] << endl;
			fout.write((char*)&indices[i * 3], sizeof(unsigned int));
			fout.write((char*)&indices[i * 3 + 1], sizeof(unsigned int));
			fout.write((char*)&indices[i * 3 + 2], sizeof(unsigned int));
		}
	}
	// ------------------- Vertices

	// 输出顶点数目
	header = Gen::EMDL_LUMP_VERTICES;
	fout.write((char*)&header, sizeof(header));
	fout.write((char*)&vertIndex, sizeof(vertIndex));
	//fout << "------------------- Vertices" << endl;
	//fout << vertIndex << endl;

	// 导出所有顶点
	for (int i=0; i<vertIndex; i++)
	{
		//fout << indexVertexMap[i].x << " " << indexVertexMap[i].y << " " << indexVertexMap[i].z << endl;
		//fout << indexVertexMap[i].nx << " " << indexVertexMap[i].ny << " " << indexVertexMap[i].nz << endl;
		//fout << indexVertexMap[i].u << " " << indexVertexMap[i].v << endl;

		fout.write((char*)&indexVertexMap[i].x, sizeof(float) * 8);
	}

	fout.close();


	// 导出蒙皮信息
	if (skin)
	{
		string skin_filename = g_PathName;
		skin_filename += string(node->GetName());
		skin_filename += ".esk";
		ofstream foutSkin(skin_filename.data());

		// 输出顶点总数
		foutSkin << vertIndex << endl;

		for (int i=0; i<vertIndex; i++)
		{
			int index = indexVertexMap[i].index;
			// NOTE: 这里假设蒙皮修改器的顶点与多边形是一致的
			IGameSkin::VertexType type = skin->GetVertexType(index);
			switch(type)
			{
			case IGameSkin::IGAME_RIGID:
			case IGameSkin::IGAME_RIGID_BLENDED:
				{
					int boneCount = skin->GetNumberOfBones(index);

					// 顶点最大骨骼数超过4，退出
					if (boneCount>4) boneCount = 4;
					float sumWeight = 0.0f;

					// 影响顶点的骨骼数量
					foutSkin << boneCount << endl;
					for (int b=0; b<boneCount; b++)
					{
						float weight = skin->GetWeight(index, b);
						int boneId = skin->GetBoneID(index, b);
						// 最后一个顶点，将剩余权重全部分配
						if (b==boneCount-1)
							weight = 1.0f - sumWeight;
						else
							sumWeight += weight;

						// 骨骼编号和权重
						foutSkin << boneId << " " << weight << endl;
					}
				}
				break;
			default:
				break;
			}
		}
		foutSkin.close();
		
		BoneInitMatrix boneInitMatrix;

		exportedBoneCount = 0;

		// 导出模型的骨骼层次关系以及默认状态
		ExportBoneHierarchy(node, boneInitMatrix);

		// 导出骨骼动画
		ExportBoneHierarchyAnimation(node, skin, boneInitMatrix);
	}

}

// 导出材质
void ExportMaterial(IGameMaterial* material)
{
	Gen::String matName(material->GetMaterialName());

	// 如果相同材质已经导出过则不重复导出
	if (!Gen::MaterialManager::Instance().GetByName(matName))
	{
		Gen::Material* mat	= Gen::MaterialManager::Instance().Create(matName);

		// 导出基本材质属性
		Point3 color;
		IGameProperty* p;

		if (p = material->GetAmbientData())
		{
			p->GetPropertyValue(color);
			mat->SetAmbient(Gen::Color4f(color.x, color.y, color.z));
		}

		if (p = material->GetDiffuseData())
		{
			p->GetPropertyValue(color);
			mat->SetDiffuse(Gen::Color4f(color.x, color.y, color.z));
		}

		if (p = material->GetEmissiveData())
		{
			p->GetPropertyValue(color);
			mat->SetEmissive(Gen::Color4f(color.x, color.y, color.z));
		}

		if (p = material->GetSpecularData())
		{
			p->GetPropertyValue(color);
			mat->SetSpecular(Gen::Color4f(color.x, color.y, color.z));
		}

		if (p = material->GetSpecularLevelData())
		{
			float f;
			p->GetPropertyValue(f);
			mat->SetSpecularLevel(f);
		}

		//if (ambient->GetType()==IGAME_POINT3_PROP)
		//{
		//	Point3 c;
		//	ambient->GetPropertyValue(c);
		//	fout << "Ambient:" << c.x << " " << c.y << " " << c.z << endl;
		//}

		IGameTextureMap* texture = material->GetIGameTextureMap(0);
		if (texture)
		{
			string filename = texture->GetBitmapFileName();
			size_t pos = filename.find_last_of("\\");
			if (pos!=string::npos)
			{
				filename = filename.substr(pos + 1);
			}
			mat->GetTextureRenderState()->textureName = filename;
			string texFilename = g_PathName;
			texFilename += filename;
			CopyFile(texture->GetBitmapFileName(), texFilename.data(), FALSE);

		}

		Gen::String matFullName = Gen::String(g_PathName) + matName + ".emt";
		mat->SaveToFile(matFullName);
	}
}

// 导出数据
extern "C" __declspec(dllexport) int DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	try
	{
		// 显示导出对话框
		// 注：暂时无法在dll中调用这个对话框

		//if(!suppressPrompts)
		//	DialogBoxParam(hInstance, 
		//			MAKEINTRESOURCE(IDD_PANEL), 
		//			GetActiveWindow(), 
		//			MeshExporterOptionsDlgProc, (LPARAM)this);

		locale   langLocale("");
		setlocale(LC_ALL, langLocale.name().data());

		// 记录文件的保存路径
		g_PathName = name;
		size_t pos = g_PathName.find_last_of("\\");
		if (pos!=string::npos)
		{
			g_PathName = g_PathName.substr(0, pos + 1);
		}

		// 使用IGame导出

		// 指定坐标系
		UserCoord coord = {
			1,	// 右手系
			1,	// X轴向右
			2,	// Y轴向上
			5,	// Z轴向外
			1,	// U坐标向右
			0,	// V坐标向上
		};

		IGameScene* pIgame = GetIGameInterface();

		IGameConversionManager*	cm = GetConversionManager();
		cm->SetUserCoordSystem(coord);

		// 初始化IGame，这里可以设置是否仅导出选中对象
		pIgame->InitialiseIGame();

		g_TicksPerFrame = pIgame->GetSceneTicks();

		// 指定导出帧为初始
		pIgame->SetStaticFrame(0);

		// 遍历所有根节点
		for (int loop=0; loop<pIgame->GetTopLevelNodeCount(); loop++)
		{
			// 获取一个节点
			IGameNode* pGameNode = pIgame->GetTopLevelNode(loop);

			switch (pGameNode->GetIGameObject()->GetIGameType())
			{
			case IGameObject::IGAME_BONE:
				//ExportBoneHierarchyAnimation(pGameNode);
				break;

			case IGameObject::IGAME_MESH:
				ExportMesh(pGameNode);
				//ExportSkinModifier(pGameNode);
			default:
				ExportNode_r(pGameNode);
				break;
			}
		}

		// 释放IGame
		pIgame->ReleaseIGame();
	}	
	catch(...)
	{
		MessageBox(NULL, "导出插件发生异常", "Exception", MB_OK|MB_ICONHAND);
	}

	return TRUE;
}
