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

// Mesh�汾
#define EMDL_VERSION 20


// ��¼�����ĳ�ʼ����
typedef map<int, GMatrix>	BoneInitMatrix;

// ��ȡ�����ؼ�֡��ÿ4֡����һ�β���
static int sampleRate = 4;

// ��¼��������ʵ�ʵ����Ĺ�������
static int exportedBoneCount;

// ÿ֡tick��Ŀ
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

// �����ã����tab
void OutputTab(ofstream& fout, int num)
{
	for (int i=0; i<num; i++)
		fout << "\t";
}

// �����ؼ�֡
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

		// �ؼ�֡����
		fout << posKey.Count() << endl;

		for (int i=0; i<posKey.Count(); i++)
		{
			// д������ֵ
			Point3 k = posKey[i].sampleKey.pval;
			fout << k.x << " " << k.y << " " << k.z << endl;

			// д����תֵ
			// Note: Max����Ԫ��x y z������ʵ���෴����Ҫȡ��
			Quat q = rotKey[i].sampleKey.qval;
			fout << -q.x << " " << -q.y << " " << -q.z << " " << q.w << endl;
		}
		fout.close();
	}
}

// �ݹ鵼���ڵ�
void ExportNode_r(IGameNode* node)
{
	// ֻ���ڵ�����GetIGameObject��������ܻ�ȡ��ȷ��IGameControl
	// But why?
	IGameObject* obj = node->GetIGameObject();

	// ����Ŀ��ڵ�
	// Ŀ��ڵ���ָ������͵ƹ�ȵĿ�����Է����õĽڵ㣬
	// ͨ������ڵ㲻��Ҫ����
	if (node->IsTarget())
		return;

	// �������Ĺؼ�֡����
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

			// �ж��Ƿ�ΪSkin�޸���
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

	// ��ȡ������״̬����
	Matrix3 boneInitMat;

#if 0
	char buf[256];

	sprintf(buf, "Bone name: %s\nId: %d", bone->GetName(), bone->GetNodeID());
	MessageBox(NULL, buf, "Bone", MB_OK);
#endif

	int id = bone->GetNodeID();

	GMatrix initGMatrix,		// �ù�������ڸ��ڵ�ĳ�ʼ״̬����
			boneGMat;			// �ù�����ǰ���������

	exportedBoneCount++;

	// ������ڵ���һ����������Ƥ�Ľ�㣬����֮
	// ע����������̳й�ϵ�д��ڲ�������Ƥ�Ľ�㣬�������������Ǳ�ڴ���
	if (skin->GetBoneInitTM(maxNode, boneInitMat, true))
	{
		//skin->GetInitSkinTM(boneInitMat);

		// ��׼��
		for (int i=0; i<3; i++)
		{
			Point3 p = boneInitMat.GetRow(i).Normalize();
			boneInitMat.SetRow(i, p);
		}

	}
	else
	{
		// TODO: û����Ƥ�Ľ��ʹ�ý������任��Ϊ��״̬����
		boneInitMat = bone->GetWorldTM().ExtractMatrix3();
	}

	boneGMat = boneInitMat;

	// ������ת��Ϊ���������ϵ
	// Note: ����ֻ�ǽ�����ϵ�任���붯���ؼ�֡һ��
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
	// ���ڵ�ȡ����������ϵ
	// �����ڵ�ȡ����ڸ���������ϵ
	if (parentMatrix)
	{
		initGMatrix = boneGMat * parentMatrix->Inverse();
	}
	else
#endif
	{
		initGMatrix = boneGMat;
	}

	// �������map��ȥ
	initMatrixMap[id] = initGMatrix;

	// ����������ż����ڵ���
	OutputTab(fout, level);
	fout << id << endl;
	OutputTab(fout, level);
	fout << parentId << endl;

	// ��������
	//OutputTab(fout, level);
	//fout << bone->GetName() << endl;

	// �����������

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

		// �����ҵ���������ĸ��ڵ�
		IGameNode* parent = bone->GetNodeParent();
		IGameObject::ObjectTypes type = parent->GetIGameObject()->GetIGameType();

		// ������ڸ��ڵ㣬�Ҹ��ڵ��Ǹ��������
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


// ���������㼶��������ָ�����ڵ��ʾ���Ǹ����ڵ�
void ExportBoneHierarchyAnimation_r(IGameNode* node, IGameSkin* skin, ofstream& fout, BoneInitMatrix& initMatrixMap, int parentId, IGameKeyTab* parentKey, int level)
{
	IGameObject* obj = node->GetIGameObject();

	IGameKeyTab	matKey;
	//IGameKeyTab posKey;
	//IGameKeyTab rotKey;

	//static int boneId = 0;
	int id = node->GetNodeID();

	// �������ڵ�û�б�����map�У��˳�
	if (initMatrixMap.find(id)!=initMatrixMap.end())
	{

		IGameControl* control = node->GetIGameControl();

		// ����ÿ����������ȡ��λ�ú���ת����Ϣ
		// �ӽڵ���Ҫ������ڸ��ڵ���������

		// �������
		fout << id << endl;

		if (control->GetFullSampledKeys(matKey, sampleRate, IGAME_TM/*, false*/))
		{
			// �ؼ�֡����
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
					// ������Ǹ��ڵ�
					// ʹ�ø��ڵ������������Ա任
					GMatrix parentInv = (*parentKey)[i].sampleKey.gval;
					//rx = parentInv.GetRow(0);
					//ry = parentInv.GetRow(1);
					//parentInv.SetRow(0, -rx);
					//parentInv.SetRow(1, -ry);
					parentInv = parentInv.Inverse();

					GMatrix initInv = initMatrixMap[id].Inverse();

					//mat = mat/* * parentInv*/ * initInv;

					// This hacks: ʹ����������Ժ������ܵ���Ӱ�죬��ʱ��������
					//k.x = -k.x;
					//k.y = -k.y;
					//k.z = -k.z;
				}
				else
				{
					// ���ڵ����ʹ������ı任
					GMatrix initInv = initMatrixMap[id].Inverse();

					//mat = mat * initInv;
				}

				//AffineParts ap;
				//decomp_affine(mat.ExtractMatrix3(), &ap);

				// д������ֵ
				//Point3 k = ap.t;
				//Point3 k = posKey[i].sampleKey.pval;
				fout << k.x << " " << k.y << " " << k.z << endl;

				// д����תֵ
				// Note: Max����Ԫ��x y z������ʵ���෴����Ҫȡ��
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

		// ��¼���������ٶ�
		// 1s = 4800 ticks
		// 1ms = 4.8 ticks
		int ticksPerSample = g_TicksPerFrame * sampleRate;
		fout << ticksPerSample << endl;

		// �����������������
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

		// �ҵ���һ����Ƥ�޸���
		// ע���˴���max���޸�����˳��Ϊ��������
		if (modifier->IsSkin())
		{
			return (IGameSkin*)modifier;
		}
	}

	return NULL;
}

// ����ģ��
void ExportMesh(IGameNode* node)
{
	IGameObject* obj = node->GetIGameObject();
	obj->InitializeData();

	IGameMesh* mesh = (IGameMesh*)obj;

	IGameSkin* skin = GetIGameSkin(node);

	for (int i=0; i<obj->GetNumModifiers(); i++)
	{
		IGameModifier* modifier = obj->GetIGameModifier(i);

		// �ҵ���һ����Ƥ�޸���
		// ע���˴���max���޸�����˳��Ϊ��������
		if (modifier->IsSkin())
		{
			skin = (IGameSkin*)modifier;
			break;
		}
	}

	// ���ģ��ʹ������Ƥ�޸�����������Ƥǰ��ģ��״̬
	if (skin)
		mesh = skin->GetInitialPose();

	string filename = g_PathName;
	filename += string(node->GetName());
	filename += ".emd";
	ofstream fout(filename.data(), ios_base::binary);

	// д���ļ�ͷ
	fout.write("EMDL", 4);

	// д��汾��
	unsigned int ver = EMDL_VERSION;
	fout.write((char*)&ver, sizeof(ver));

	// ��������ͷ
	unsigned int header;

	// ��������
	IGameMaterial* material = node->GetNodeMaterial();

	// ģ�ͷ���������Ϣ
	typedef vector<Tab<FaceEx*>>	MeshElemGroup;
	MeshElemGroup	meshElementGroup;

	if (material)
	{
		int materialCount = 0;

		// ����id�б�
		map<int, int> matIdList;

		// �Ӳ�����Ŀ
		int submatCount = material->GetSubMaterialCount();

		if (submatCount)
		{
			// �������ʱ��
			for (int i=0; i<mesh->GetNumberOfFaces(); i++)
			{
				FaceEx* face = mesh->GetFace(i);
				face->matID %= submatCount;
			}

			// �������б��棬ȷ��������Ŀ
			for (int i=0; i<mesh->GetNumberOfFaces(); i++)
			{
				int matId = mesh->GetFaceMaterialID(i) % submatCount;

				// �������id��û����ӵ��б���
				if (matIdList.find(matId)==matIdList.end())
				{
					matIdList[matId] = materialCount++;

					ExportMaterial(material->GetSubMaterial(matId));
				}
			}
		}
		else
		{
			// ���е�һ���ʵ�ģ��
			materialCount = 1;

			// ��������id
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


		// TODO: �������в���
		unsigned int mat_header = Gen::EMDL_LUMP_MATERIALS;
		fout.write((char*)&mat_header, sizeof(mat_header));

		// ���������(����ģ����Ŀ)
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
		// û�в��ʵĶ��󣬽����ж���η���ͬһ������
		Tab<FaceEx*> faces;

		for (int i=0; i<mesh->GetNumberOfFaces(); i++)
		{
			FaceEx* f = mesh->GetFace(i);
			faces.Append(1, &f);
		}

		meshElementGroup.push_back(faces);
	}

	// ������ģ���б�

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


		// ����Ŀ
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

				// �����ͬ���㲻���ڴ��ڣ����������㣬�����ŵ���
				if ((iter=vertexIndexMap.find(vi))==vertexIndexMap.end())
				{
					indexVertexMap[vertIndex] = vi;
					vertexIndexMap[vi] = vertIndex;
					indices.push_back(vertIndex++);
				}
				else
				{
					// �����ƽ��ʹ�����ж��������
					indices.push_back(iter->second);
				}

			}
		}

		//fout << "------------------- Indices" << endl;

		int indexSize = (int)indices.size() / 3;

		fout.write((char*)&indexSize, sizeof(indexSize));

		// ���������Ŀ
		//fout << indexSize << endl;

		// �����������Ϣ
		for (int i=0; i<indexSize; i++)
		{
			//fout << indices[i * 3] << " " << indices[i * 3 + 1] << " " << indices[i * 3 + 2] << endl;
			fout.write((char*)&indices[i * 3], sizeof(unsigned int));
			fout.write((char*)&indices[i * 3 + 1], sizeof(unsigned int));
			fout.write((char*)&indices[i * 3 + 2], sizeof(unsigned int));
		}
	}
	// ------------------- Vertices

	// ���������Ŀ
	header = Gen::EMDL_LUMP_VERTICES;
	fout.write((char*)&header, sizeof(header));
	fout.write((char*)&vertIndex, sizeof(vertIndex));
	//fout << "------------------- Vertices" << endl;
	//fout << vertIndex << endl;

	// �������ж���
	for (int i=0; i<vertIndex; i++)
	{
		//fout << indexVertexMap[i].x << " " << indexVertexMap[i].y << " " << indexVertexMap[i].z << endl;
		//fout << indexVertexMap[i].nx << " " << indexVertexMap[i].ny << " " << indexVertexMap[i].nz << endl;
		//fout << indexVertexMap[i].u << " " << indexVertexMap[i].v << endl;

		fout.write((char*)&indexVertexMap[i].x, sizeof(float) * 8);
	}

	fout.close();


	// ������Ƥ��Ϣ
	if (skin)
	{
		string skin_filename = g_PathName;
		skin_filename += string(node->GetName());
		skin_filename += ".esk";
		ofstream foutSkin(skin_filename.data());

		// �����������
		foutSkin << vertIndex << endl;

		for (int i=0; i<vertIndex; i++)
		{
			int index = indexVertexMap[i].index;
			// NOTE: ���������Ƥ�޸����Ķ�����������һ�µ�
			IGameSkin::VertexType type = skin->GetVertexType(index);
			switch(type)
			{
			case IGameSkin::IGAME_RIGID:
			case IGameSkin::IGAME_RIGID_BLENDED:
				{
					int boneCount = skin->GetNumberOfBones(index);

					// ����������������4���˳�
					if (boneCount>4) boneCount = 4;
					float sumWeight = 0.0f;

					// Ӱ�춥��Ĺ�������
					foutSkin << boneCount << endl;
					for (int b=0; b<boneCount; b++)
					{
						float weight = skin->GetWeight(index, b);
						int boneId = skin->GetBoneID(index, b);
						// ���һ�����㣬��ʣ��Ȩ��ȫ������
						if (b==boneCount-1)
							weight = 1.0f - sumWeight;
						else
							sumWeight += weight;

						// ������ź�Ȩ��
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

		// ����ģ�͵Ĺ�����ι�ϵ�Լ�Ĭ��״̬
		ExportBoneHierarchy(node, boneInitMatrix);

		// ������������
		ExportBoneHierarchyAnimation(node, skin, boneInitMatrix);
	}

}

// ��������
void ExportMaterial(IGameMaterial* material)
{
	Gen::String matName(material->GetMaterialName());

	// �����ͬ�����Ѿ����������ظ�����
	if (!Gen::MaterialManager::Instance().GetByName(matName))
	{
		Gen::Material* mat	= Gen::MaterialManager::Instance().Create(matName);

		// ����������������
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

// ��������
extern "C" __declspec(dllexport) int DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	try
	{
		// ��ʾ�����Ի���
		// ע����ʱ�޷���dll�е�������Ի���

		//if(!suppressPrompts)
		//	DialogBoxParam(hInstance, 
		//			MAKEINTRESOURCE(IDD_PANEL), 
		//			GetActiveWindow(), 
		//			MeshExporterOptionsDlgProc, (LPARAM)this);

		locale   langLocale("");
		setlocale(LC_ALL, langLocale.name().data());

		// ��¼�ļ��ı���·��
		g_PathName = name;
		size_t pos = g_PathName.find_last_of("\\");
		if (pos!=string::npos)
		{
			g_PathName = g_PathName.substr(0, pos + 1);
		}

		// ʹ��IGame����

		// ָ������ϵ
		UserCoord coord = {
			1,	// ����ϵ
			1,	// X������
			2,	// Y������
			5,	// Z������
			1,	// U��������
			0,	// V��������
		};

		IGameScene* pIgame = GetIGameInterface();

		IGameConversionManager*	cm = GetConversionManager();
		cm->SetUserCoordSystem(coord);

		// ��ʼ��IGame��������������Ƿ������ѡ�ж���
		pIgame->InitialiseIGame();

		g_TicksPerFrame = pIgame->GetSceneTicks();

		// ָ������֡Ϊ��ʼ
		pIgame->SetStaticFrame(0);

		// �������и��ڵ�
		for (int loop=0; loop<pIgame->GetTopLevelNodeCount(); loop++)
		{
			// ��ȡһ���ڵ�
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

		// �ͷ�IGame
		pIgame->ReleaseIGame();
	}	
	catch(...)
	{
		MessageBox(NULL, "������������쳣", "Exception", MB_OK|MB_ICONHAND);
	}

	return TRUE;
}
