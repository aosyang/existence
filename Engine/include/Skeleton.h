//-----------------------------------------------------------------------------------
/// Skeleton.h �����࣬����ʵ�ֹ�������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _SKELETON_H
#define _SKELETON_H

#include "Matrix4.h"
#include "EString.h"
#include "Resource.h"

#include <vector>
#include <map>
#include <set>

using namespace std;

namespace Gen
{
#define MAX_BONE_NUM_PER_VERTEX 4

	class Skeleton;
	class SkeletonManager;
	class SkeletalMeshObject;

	/// @brief
	/// ��Ƥ������Ϣ
	/// @par
	///		��¼��Ӱ���������ľ����Ȩ��
	struct SkinnedVertexInfo
	{
		unsigned int boneCount;								///< Ӱ��ö���Ĺ������������4
		unsigned int boneId[MAX_BONE_NUM_PER_VERTEX];		///< ÿ�������ı��
		float weight[MAX_BONE_NUM_PER_VERTEX];				///< ��Ӧÿ��������Ȩ��(��Ӧ��Ϊ1)

	};

	// �����ؽڣ���¼�ؽ�֮���Ĭ�Ϲ�ϵ
	class SkeletalJoint
	{
		friend class Skeleton;
	public:
		SkeletalJoint(int id, int parentId = -1);
		~SkeletalJoint();

		// ��ȡ�ؽ�id
		inline int GetJointId() const { return m_JointId; }

		// ��ȡ���ڵ�id
		inline int GetParentId() const { return m_ParentId; }

		inline const Matrix4& GetOriginTransformInv() const { return m_OriginTransformInv; }
		inline const Matrix4& GetInitTransform() const { return m_InitTransform; }

	private:
		int				m_JointId;			///< �ؽ�id
		int				m_ParentId;			///< ���ڵ�id

		Matrix4			m_OriginTransformInv;	///< ԭʼ�任��任
		Matrix4			m_InitTransform;		///< ԭʼ�任

	};

	// �ؽ���̬������ÿ���ؼ�֡��¼��һ���ؽ�����һʱ�̵���̬
	class JointPoseAnimation
	{
	public:
		JointPoseAnimation(int keyFrameCount);
		~JointPoseAnimation();

		Vector3f& PosArray(int keyFrame);
		Quaternion& QuatArray(int keyFrame);

	private:
		//int				m_KeyFrameCount;

		Vector3f*		m_PosArray;				///< λ����������
		Quaternion*		m_RotArray;				///< ��ת��������
	};


	// ��������
	class SkeletalAnimation
	{
	public:
		SkeletalAnimation();
		~SkeletalAnimation();

		bool LoadFromFile(const String& filename);

		inline int GetKeyFrameCount() const { return m_KeyFrameCount; }
		inline int GetTicksPerSample() const { return m_TicksPerSample; }

		// ��ȡָ���ؽڵĶ���
		JointPoseAnimation* GetJointPoseAnim(int jointId)
		{
			return m_JointAnimationSet[jointId];
		}
	private:
		int				m_KeyFrameCount;		///< �ؼ�֡����
		int				m_TicksPerSample;		///< ÿ����tick��

		typedef map<int, JointPoseAnimation*>	JointSet;
		JointSet			m_JointAnimationSet;

	};

	// �����࣬��¼�˹����ļ̳й�ϵ
	class Skeleton : public Resource
	{
		friend class SkeletonManager;
		friend class SkeletalMeshObject;
	public:
		// ----- Skeleton Methods
		inline SkeletalAnimation* GetAnimation(const String& animName)
		{
			if (m_AnimationSet.find(animName)!=m_AnimationSet.end())
				return m_AnimationSet[animName];

			return NULL;
		}
		inline SkeletalJoint* GetJoint(int jointId) { return m_JointList[jointId]; }
	protected:
		// ----- Overwrite Resource
		bool LoadImpl();
		void UnloadImpl();

		// ----- Skeleton Methods
		Skeleton(const String& filename);
		~Skeleton();

	protected:
		// ������Ϣ
		// ÿ�������������Ӧ����Ϣ

		SkeletalJoint*						m_RootJoint;		///< ���ؽ�
		typedef map<int, SkeletalJoint*>	JointList;
		JointList							m_JointList;		///< �ؽ��б��Ա��Ϊ������¼�ù���ӵ�е�ȫ���ؽ�

		typedef map<const String, SkeletalAnimation*>	SkelAnimSet;
		SkelAnimSet							m_AnimationSet;		///< ��������
	};
}

#endif
