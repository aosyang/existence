//-----------------------------------------------------------------------------------
/// Skeleton.cpp 骨骼类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "Skeleton.h"
#include "Engine.h"
#include "FileSystem.h"

#include <fstream>



namespace Gen
{
	SkeletalJoint::SkeletalJoint(int id, int parentId)
		: m_JointId(id),
		  m_ParentId(parentId)
	{
	}

	SkeletalJoint::~SkeletalJoint()
	{
	}

	// ----------------------------------- JointPoseAnimation

	JointPoseAnimation::JointPoseAnimation(int keyFrameCount)
		//: m_KeyFrameCount(keyFrameCount)
	{
		AssertFatal(keyFrameCount>0, "JointPoseAnimation::JointPoseAnimation(): Invalid animation keyframe count.");

		m_PosArray = new Vector3f[keyFrameCount];
		m_RotArray = new Quaternion[keyFrameCount];
	}


	JointPoseAnimation::~JointPoseAnimation()
	{
		SAFE_DELETE(m_PosArray);
		SAFE_DELETE(m_RotArray);
	}

	Vector3f& JointPoseAnimation::PosArray(int keyFrame)
	{
		return m_PosArray[keyFrame];
	}

	Quaternion& JointPoseAnimation::QuatArray(int keyFrame)
	{
		return m_RotArray[keyFrame];
	}

	// ----------------------------------- SkeletalAnimation
	SkeletalAnimation::SkeletalAnimation()
	{
	}

	SkeletalAnimation::~SkeletalAnimation()
	{
		JointSet::iterator iter;
		for (iter=m_JointAnimationSet.begin();
			 iter!=m_JointAnimationSet.end();
			 iter++)
		{
			delete iter->second;
		}
	}

	bool SkeletalAnimation::LoadFromFile(const String& filename)
	{
		std::ifstream fin(filename.Data());
		if (!fin.is_open()) return false;

		// 采样律
		fin >> m_TicksPerSample;

		// 骨骼数目
		int boneCount;
		fin >> boneCount;

		for (int i=0; i<boneCount; i++)
		{
			int boneId;
			fin >> boneId;

			fin >> m_KeyFrameCount;

			JointPoseAnimation* anim = new JointPoseAnimation(m_KeyFrameCount);

			// 检测是否该id已存在
			AssertFatal(m_JointAnimationSet.find(boneId)==m_JointAnimationSet.end(), "SkeletalAnimation::LoadFromFile(): Duplicated bone id found.");

			m_JointAnimationSet[boneId] = anim;

			for (int k=0; k<m_KeyFrameCount; k++)
			{
				Vector3f& pos = anim->PosArray(k);
				Quaternion& quat = anim->QuatArray(k);

				fin >> pos.x >> pos.y >> pos.z;
				fin >> quat.x >> quat.y >> quat.z >> quat.w;
			}
		}

		fin.close();
		return true;
	}

	// ----------------------------------- Skeleton

	Skeleton::Skeleton(const String& filename)
		: Resource(filename),
		  m_RootJoint(NULL)
	{
	}

	Skeleton::~Skeleton()
	{
	}

	bool Skeleton::LoadImpl()
	{
		std::ifstream fin(m_Filename.Data());
		if (!fin.is_open()) return false;

		int jointId, parentId;
		fin >> jointId >> parentId;

		while (!fin.eof())
		{
			SkeletalJoint* joint;
			if (parentId==-1)
			{
				// 如果没有父节点，指定为根结点
				joint = m_RootJoint = new SkeletalJoint(jointId);
			}
			else
			{
				// 如果拥有父节点，创建骨骼后将之添加到对应的父结点下
				SkeletalJoint* parent = m_JointList[parentId];
				joint = new SkeletalJoint(jointId, parentId);
				//parent->AddChildJoint(joint);
			}

			// 获得该关节的初始变换
			Vector3f pos;
			Quaternion quat;

			fin >> pos.x >> pos.y >> pos.z;
			fin >> quat.x >> quat.y >> quat.z >> quat.w;

			joint->m_InitTransform.MakeIdentity();
			joint->m_InitTransform.SetPosition(pos);
			joint->m_InitTransform.SetRotation(quat);

			// 求出相对于父节点的逆变换
			if (joint->m_ParentId != -1)
				joint->m_OriginTransformInv = joint->m_InitTransform.GetInverseMatrix() * m_JointList[joint->m_ParentId]->m_OriginTransformInv;
			else
				joint->m_OriginTransformInv = joint->m_InitTransform.GetInverseMatrix();

			m_JointList[jointId] = joint;

			fin >> jointId >> parentId;
		}

		fin.close();

		// TODO: 载入骨骼动画列表
		String animListName = m_Filename;
		String lcName = m_Filename;
		lcName.ToLowerCase();
		size_t pos = lcName.FindLastOf("ebh");
		animListName.Replace(pos - 2, 3, "eal");

		// 打开动画列表文件
		std::ifstream flist(animListName.Data());
		if (flist.is_open())
		{
			while(!flist.eof())
			{
				char name[256];
				flist.getline(name, sizeof(name));
				if (flist.eof()) break;

				String animName = name;
				// Linux需要用这种方式防止换行符被读入字串
				animName.Trim();
				AssertFatal(m_AnimationSet.find(animName)==m_AnimationSet.end(),
							"Skeleton::Load(): A same name for the animation already exists.");

				flist.getline(name, sizeof(name));
				String filename = name;
				filename.Trim();
				SkeletalAnimation* anim = new SkeletalAnimation;
				if (!anim->LoadFromFile(filename))
				{
					String fullName = GetPathName(m_Filename) + filename;

					if (!anim->LoadFromFile(fullName))
					{
						// 仍然无法成功读取，放弃
						SAFE_DELETE(anim);
					}
				}

				if (anim)
				{
					m_AnimationSet[animName] = anim;
				}
			}


			flist.close();
		}

		return true;
	}

	void Skeleton::UnloadImpl()
	{
		// 删除骨骼信息
		for (JointList::iterator iter=m_JointList.begin();
			 iter!=m_JointList.end();
			 iter++)
		{
			delete iter->second;
		}
		m_JointList.clear();
		m_RootJoint = NULL;

		// 删除动画信息
		for (SkelAnimSet::iterator iter=m_AnimationSet.begin();
			 iter!=m_AnimationSet.end();
			 iter++)
		{
			delete iter->second;
		}
		m_AnimationSet.clear();
	}

}
