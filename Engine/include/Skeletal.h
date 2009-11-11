#ifndef _SKELETAL_H
#define _SKELETAL_H

#include "Matrix4.h"

#include <vector>
#include <map>
#include "EString.h"

using namespace std;

namespace Gen
{
#define MAX_BONE_NUM_PER_VERTEX 4

	// ÿ�������Ƥ��Ϣ����¼��Ӱ���������ľ����Ȩ��
	typedef struct skinVertex
	{
		unsigned int boneNum;								///< �������������4
		unsigned int boneID[MAX_BONE_NUM_PER_VERTEX];		///< �������
		float weight[MAX_BONE_NUM_PER_VERTEX];				///< ��Ӧÿ��������Ȩ��

		skinVertex& operator=(const skinVertex& rhs)
		{
			if (this!=&rhs)
				memcpy(this, &rhs, sizeof(skinVertex));

			return *this;
		}

	} skinVertex_t;

	// ��������¼�ؽ�֮��Ĺ�ϵ
	class Bone
	{
	public:
		Bone();

		void Update();

	//private:
		Bone*	m_Parent;			///< ���ڵ�

		// ������ر���
		bool	m_IsUpdated;		///< ��¼�ýڵ�
		Matrix4	m_Transform;		///< �任����
		Matrix4	m_RootTransform;	///< ������SceneObject������任�������ڹ���ϵͳ������任������ڸ��ڵ���Ե�
	};

	class Skeletal
	{
	public:
		Skeletal();
		~Skeletal();

		Bone* ReadBones(ifstream& fin, unsigned int index);
		bool LoadFromFile(const String& filename);

		void Update(unsigned int time);

	//protected:
		// ������Ϣ
		// ÿ�������������Ӧ����Ϣ

		Bone*								m_BoneRoot[100];			///< ������
		//map<unsigned int, Bone*>			m_Bones;			///< ��������id
		vector<Bone*>						m_Bones[100];

		unsigned int						m_Frames;
		map<unsigned int, skinVertex_t>		m_SkinVertexInfo;	///< ������Ƥ��Ϣ
	};
}

#endif
