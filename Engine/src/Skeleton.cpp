//-----------------------------------------------------------------------------------
/// Skeleton.cpp ������ʵ��
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

		// ������
		fin >> m_TicksPerSample;

		// ������Ŀ
		int boneCount;
		fin >> boneCount;

		for (int i=0; i<boneCount; i++)
		{
			int boneId;
			fin >> boneId;

			fin >> m_KeyFrameCount;

			JointPoseAnimation* anim = new JointPoseAnimation(m_KeyFrameCount);

			// ����Ƿ��id�Ѵ���
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
				// ���û�и��ڵ㣬ָ��Ϊ�����
				joint = m_RootJoint = new SkeletalJoint(jointId);
			}
			else
			{
				// ���ӵ�и��ڵ㣬����������֮��ӵ���Ӧ�ĸ������
				SkeletalJoint* parent = m_JointList[parentId];
				joint = new SkeletalJoint(jointId, parentId);
				//parent->AddChildJoint(joint);
			}

			// ��øùؽڵĳ�ʼ�任
			Vector3f pos;
			Quaternion quat;

			fin >> pos.x >> pos.y >> pos.z;
			fin >> quat.x >> quat.y >> quat.z >> quat.w;

			joint->m_InitTransform.MakeIdentity();
			joint->m_InitTransform.SetPosition(pos);
			joint->m_InitTransform.SetRotation(quat);

			// �������ڸ��ڵ����任
			if (joint->m_ParentId != -1)
				joint->m_OriginTransformInv = joint->m_InitTransform.GetInverseMatrix() * m_JointList[joint->m_ParentId]->m_OriginTransformInv;
			else
				joint->m_OriginTransformInv = joint->m_InitTransform.GetInverseMatrix();

			m_JointList[jointId] = joint;

			fin >> jointId >> parentId;
		}

		fin.close();

		// TODO: ������������б�
		String animListName = m_Filename;
		String lcName = m_Filename;
		lcName.ToLowerCase();
		size_t pos = lcName.FindLastOf("ebh");
		animListName.Replace(pos - 2, 3, "eal");

		// �򿪶����б��ļ�
		std::ifstream flist(animListName.Data());
		if (flist.is_open())
		{
			while(!flist.eof())
			{
				char name[256];
				flist.getline(name, sizeof(name));
				if (flist.eof()) break;

				String animName = name;
				// Linux��Ҫ�����ַ�ʽ��ֹ���з��������ִ�
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
						// ��Ȼ�޷��ɹ���ȡ������
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
		// ɾ��������Ϣ
		for (JointList::iterator iter=m_JointList.begin();
			 iter!=m_JointList.end();
			 iter++)
		{
			delete iter->second;
		}
		m_JointList.clear();
		m_RootJoint = NULL;

		// ɾ��������Ϣ
		for (SkelAnimSet::iterator iter=m_AnimationSet.begin();
			 iter!=m_AnimationSet.end();
			 iter++)
		{
			delete iter->second;
		}
		m_AnimationSet.clear();
	}

}
