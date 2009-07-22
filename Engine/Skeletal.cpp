#include "Skeletal.h"
#include "Engine.h"

#include <fstream>

using namespace std;

Bone::Bone()
: m_IsUpdated(false),
  m_Parent(NULL)
{
}

void Bone::Update()
{
	if (!m_Parent)
	{
		// 这是一个根节点
		m_RootTransform = Matrix4::IDENTITY;
	}
	else
	{
		if (!m_Parent->m_IsUpdated)
		{
			m_Parent->Update();
		}

		m_RootTransform = /*m_Parent->m_RootTransform * */m_Transform;
	}
	m_IsUpdated = true;
}

Skeletal::Skeletal()
//: m_BoneRoot(NULL)
{
}

Skeletal::~Skeletal()
{
	for (int i=0; i<100; i++)
		for (vector<Bone*>::iterator iter=m_Bones[i].begin(); iter!=m_Bones[i].end(); iter++)
		{
			delete (*iter);
		}
}

Bone* Skeletal::ReadBones(ifstream& fin, unsigned int index)
{
	unsigned int childNum;
	fin >> childNum;

	Matrix4 local;

	for (int i=0; i<16; i++)
	{
		fin >> local.mArray[i];
	}

	Bone* bone = new Bone();
	bone->m_Transform = local;

	for (int i=0; i<childNum; i++)
	{
		Bone* child = ReadBones(fin, index);
		child->m_Parent = bone;
	}

	m_Bones[index].push_back(bone);

	return bone;
}

bool Skeletal::LoadFromFile(const String& filename)
{
	ifstream fin(filename.Data());

	if (!fin.is_open())
		return false;

	fin >> m_Frames;

	for (int i=0; i<m_Frames; i++)
		m_BoneRoot[i] = ReadBones(fin, i);

	fin.close();

	return true;
}

void Skeletal::Update(unsigned int time)
{
	for (vector<Bone*>::iterator iter=m_Bones[time].begin(); iter!=m_Bones[time].end(); iter++)
	{
		(*iter)->Update();
	}
}
