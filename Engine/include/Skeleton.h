//-----------------------------------------------------------------------------------
/// Skeleton.h 骨骼类，用于实现骨骼动画
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
	/// 蒙皮顶点信息
	/// @par
	///		记录了影响这个顶点的矩阵和权重
	struct SkinnedVertexInfo
	{
		unsigned int boneCount;								///< 影响该顶点的骨骼数量，最大4
		unsigned int boneId[MAX_BONE_NUM_PER_VERTEX];		///< 每个骨骼的编号
		float weight[MAX_BONE_NUM_PER_VERTEX];				///< 对应每个骨骼的权重(和应该为1)

	};

	// 骨骼关节，记录关节之间的默认关系
	class SkeletalJoint
	{
		friend class Skeleton;
	public:
		SkeletalJoint(int id, int parentId = -1);
		~SkeletalJoint();

		// 获取关节id
		inline int GetJointId() const { return m_JointId; }

		// 获取父节点id
		inline int GetParentId() const { return m_ParentId; }

		inline const Matrix4& GetOriginTransformInv() const { return m_OriginTransformInv; }
		inline const Matrix4& GetInitTransform() const { return m_InitTransform; }

	private:
		int				m_JointId;			///< 关节id
		int				m_ParentId;			///< 父节点id

		Matrix4			m_OriginTransformInv;	///< 原始变换逆变换
		Matrix4			m_InitTransform;		///< 原始变换

	};

	// 关节姿态动画，每个关键帧记录了一个关节在这一时刻的姿态
	class JointPoseAnimation
	{
	public:
		JointPoseAnimation(int keyFrameCount);
		~JointPoseAnimation();

		Vector3f& PosArray(int keyFrame);
		Quaternion& QuatArray(int keyFrame);

	private:
		//int				m_KeyFrameCount;

		Vector3f*		m_PosArray;				///< 位移坐标数组
		Quaternion*		m_RotArray;				///< 旋转坐标数组
	};


	// 骨骼动画
	class SkeletalAnimation
	{
	public:
		SkeletalAnimation();
		~SkeletalAnimation();

		bool LoadFromFile(const String& filename);

		inline int GetKeyFrameCount() const { return m_KeyFrameCount; }
		inline int GetTicksPerSample() const { return m_TicksPerSample; }

		// 获取指定关节的动画
		JointPoseAnimation* GetJointPoseAnim(int jointId)
		{
			return m_JointAnimationSet[jointId];
		}
	private:
		int				m_KeyFrameCount;		///< 关键帧数量
		int				m_TicksPerSample;		///< 每采样tick数

		typedef map<int, JointPoseAnimation*>	JointSet;
		JointSet			m_JointAnimationSet;

	};

	// 骨骼类，记录了骨骼的继承关系
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
		// 顶点信息
		// 每个顶点与骨骼对应的信息

		SkeletalJoint*						m_RootJoint;		///< 根关节
		typedef map<int, SkeletalJoint*>	JointList;
		JointList							m_JointList;		///< 关节列表，以标号为索引记录该骨骼拥有的全部关节

		typedef map<const String, SkeletalAnimation*>	SkelAnimSet;
		SkelAnimSet							m_AnimationSet;		///< 动画集合
	};
}

#endif
