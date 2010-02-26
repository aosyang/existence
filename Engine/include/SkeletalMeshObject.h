//-----------------------------------------------------------------------------------
/// SkeletalMeshObject.h 骨骼模型对象，骨骼模型的实例，控制动画及更新顶点信息
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _SKELETALMESHOBJECT_H
#define _SKELETALMESHOBJECT_H

#include "MeshObject.h"

namespace Gen
{
	class SkeletalJointInstance;
	class SkeletalAnimation;
	class JointPoseAnimation;

	// 骨骼模型对象
	// 场景中骨骼模型的实例化对象
	class SkeletalMeshObject : public MeshObject
	{
		DECLARE_FACTORY_OBJECT(SkeletalMeshObject);
	public:
		// ----- Overwrite IObject
		void Update(unsigned long deltaTime);

		// ----- Overwrite MeshObject
		void SetMesh(BaseMesh* mesh);

		// ----- Overwrite SceneObject
		void DebugRender();

		// ----- Overwrite IRenderableObject
		void RenderSingleObject();

		// ----- SkeletalMeshObject
		void SetSkeleton(Skeleton* skeleton);

		// 播放动画
		void PlayAnimation(const String& animName, bool resetTargetAnim=true);

		// 指定动画播放位置
		void SetAnimationTime(float time);

	protected:
		// 使用骨骼信息创建骨骼实例
		void CreateJointInstance();

		void AdvanceTime(unsigned long deltaTime);

		// 使用当前骨骼状态更新模型
		void UpdateMesh();

	protected:
		SkeletalJointInstance*		m_RootJoint;
		Skeleton*					m_Skeleton;
		IVertexBuffer*				m_VertexBuffer;

		float*						m_VertexArray;
		float*						m_NormalArray;

		map<int, SkeletalJointInstance*>	m_JointInstanceMap;

		SkeletalAnimation*			m_CurrentAnimation;		///< 当前播放的动画
		float						m_CurAnimationTime;		///< 当前动画时间
	};


	// 骨骼关节实例，记录关节之间某时刻的关系
	class SkeletalJointInstance
	{
		friend class Skeleton;
	public:
		SkeletalJointInstance(int id, int parentId, Skeleton* skeleton);
		~SkeletalJointInstance();

		int GetJointId() const { return m_JointId; }

		// 获取父节点id
		int GetParentId() const { return m_ParentId; }

		inline const Matrix4& GetBoneTransform() const { return m_BoneTransform; }

		// 添加子关节
		void AddChildJoint(SkeletalJointInstance* child);

		// debug渲染
		void DebugRender(const Matrix4& world);

		// 更新骨骼关系
		void Update(float animTime, const Matrix4& parentBoneTransform=Matrix4::IDENTITY);

		// 指定动画
		void SetAnimation(SkeletalAnimation* anim);
	private:
		int				m_JointId;			///< 关节id
		int				m_ParentId;			///< 父节点id

		typedef set<SkeletalJointInstance*>	ChildrenJoints;
		ChildrenJoints	m_ChildrenJoints;	///< 子关节

		JointPoseAnimation*	m_JointAnim;	///< 当前使用的关节动画

		Matrix4			m_Transform;			///< 局部变换
		Matrix4			m_BoneTransform;		///< 骨骼空间变换

		Skeleton*		m_Skeleton;				///< 关键使用的骨骼资源
	};

}

#endif
