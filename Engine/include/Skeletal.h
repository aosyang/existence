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

	// 每顶点的蒙皮信息，记录了影响这个顶点的矩阵和权重
	typedef struct skinVertex
	{
		unsigned int boneNum;								///< 骨骼数量，最大4
		unsigned int boneID[MAX_BONE_NUM_PER_VERTEX];		///< 骨骼编号
		float weight[MAX_BONE_NUM_PER_VERTEX];				///< 对应每个骨骼的权重

		skinVertex& operator=(const skinVertex& rhs)
		{
			if (this!=&rhs)
				memcpy(this, &rhs, sizeof(skinVertex));

			return *this;
		}

	} skinVertex_t;

	// 骨骼，记录关节之间的关系
	class Bone
	{
	public:
		Bone();

		void Update();

	//private:
		Bone*	m_Parent;			///< 父节点

		// 更新相关变量
		bool	m_IsUpdated;		///< 记录该节点
		Matrix4	m_Transform;		///< 变换矩阵
		Matrix4	m_RootTransform;	///< 类似于SceneObject的世界变换，不过在骨骼系统中这个变换是相对于根节点而言的
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
		// 顶点信息
		// 每个顶点与骨骼对应的信息

		Bone*								m_BoneRoot[100];			///< 根骨骼
		//map<unsigned int, Bone*>			m_Bones;			///< 骨骼及其id
		vector<Bone*>						m_Bones[100];

		unsigned int						m_Frames;
		map<unsigned int, skinVertex_t>		m_SkinVertexInfo;	///< 顶点蒙皮信息
	};
}

#endif
