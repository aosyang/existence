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

	/// @brief
	/// 骨骼模型对象
	/// @par
	///		可以添加到场景的骨骼模型实例化对象
	/// @remarks
	///		为骨骼模型对象指定了模型以后，会从模型中复制一份顶点数据用于动画变形
	class SkeletalMeshObject : public MeshObject
	{
		DECLARE_FACTORY_OBJECT(SkeletalMeshObject, MeshObject);
	public:
		// ----- Overwrite IObject
		/// @copydoc MeshObject::Update()
		void Update(unsigned long deltaTime);

		// ----- Overwrite MeshObject
		/// @copydoc MeshObject::SetMesh()
		void SetMesh(BaseMesh* mesh);

		// ----- SkeletalMeshObject

		void SetMesh(const String& meshName);

		void SetSkeleton(const String& skelName);

		/// @brief
		/// 为骨骼模型对象指定骨骼信息
		/// @param skeleton
		///		所要指定的骨骼信息
		void SetSkeleton(Skeleton* skeleton);

		/// @brief
		/// 播放一个骨骼动画
		/// @param animName
		///		动画名称
		/// @param resetTargetAnim
		///		是否重置所播放的动画时间点，为true时将从头开始播放动画，为false时将从上一次动画结束的时间点继续播放
		void PlayAnimation(const String& animName, bool resetTargetAnim=true);

		/// @brief
		///	指定动画播放时间点
		/// @param time
		///		动画时间点(单位：秒)
		void SetAnimationTime(float time);

		// temp
		void AddJointAnchor(const String& anchorName, int jointId);

	protected:

		// ----- Overwrite SceneObject
		/// @copydoc SceneObject::DebugRender()
		void DebugRender();

		void RenderSingleObject();

	protected:
		/// @brief
		/// 使用骨骼信息创建骨骼实例
		void CreateJointInstance();

		/// @brief
		///	递进骨骼动画的时间
		/// @param deltaTime
		///		递进时间(单位：毫秒)
		void AdvanceTime(unsigned long deltaTime);

		/// @brief
		/// 使用当前骨骼状态更新模型的顶点位置及法线方向
		void UpdateMesh();

	protected:
		RefPtr<SkeletalJointInstance>					m_RootJoint;			///< 骨骼根结点
		Skeleton*										m_Skeleton;				///< 骨骼信息
		IVertexBuffer*									m_VertexBuffer;			///< 变形用顶点缓冲副本

		float*											m_VertexArray;			///< 顶点数组
		float*											m_NormalArray;			///< 法线数组

		std::map<int, RefPtr<SkeletalJointInstance> >	m_JointInstanceMap;		///< 骨骼关节实例列表

		SkeletalAnimation*								m_CurrentAnimation;		///< 当前播放的动画
		float											m_CurAnimationTime;		///< 当前动画时间

		bool											m_NeedUpdateMesh;		///< 是否需要更新模型
	};


	/// @brief
	/// 骨骼关节实例
	/// @par
	///		骨骼关节实例是根据骨骼信息为一个骨骼模型对象创建的关节实例对象。<br>
	///		每个对象记录一个关节在某时刻的位置及旋转状态，骨骼模型对象记录一组<br>
	///		骨骼关节实例的信息，并根据蒙皮和骨骼关节实例的状态调整顶点的位置。<br>
	class SkeletalJointInstance : public Anchor
	{
		friend class Skeleton;
	public:
		/// @brief
		///	构造函数
		///	@param id
		///		关节id
		///	@param parentId
		///		父结点关节id
		///	@param skeleton
		///		骨骼信息
		SkeletalJointInstance(int id, int parentId, Skeleton* skeleton);
		~SkeletalJointInstance();

		// ----- Overwrite Anchor
		/// @copydoc Anchor::GetTransformToParentObject()
		const Matrix4& GetTransformToParentObject() const { return m_BoneTransform; }

		// ----- SkeletalJointInstance Methods
		/// @brief
		///	获取关节的id
		int GetJointId() const { return m_JointId; }

		/// @brief
		/// 获取父关节id
		int GetParentId() const { return m_ParentId; }

		/// @brief
		/// 添加一个子关节
		/// @param child
		///		子关节
		void AddChildJoint(RefPtr<SkeletalJointInstance> child);

		/// @brief
		/// 渲染调试信息
		/// @param world
		///		骨骼根结点的世界变换矩阵
		/// @remarks
		///		这个方法递归调用子结点的相同方法进行渲染输出
		void DebugRender(const Matrix4& world);

		/// @brief
		/// 更新骨骼关系
		/// @param animTime
		///		动画当前播放时间(单位：秒)
		/// @param parentBoneTransform
		///		父结点骨骼变换矩阵
		void Update(float animTime, const Matrix4& parentBoneTransform=Matrix4::IDENTITY);

		/// @brief
		/// 为关节指定动画
		/// @param anim
		///		指定的骨骼动画
		void SetAnimation(SkeletalAnimation* anim);
	private:
		int				m_JointId;			///< 关节id
		int				m_ParentId;			///< 父节点id

		typedef std::set< RefPtr<SkeletalJointInstance> >	ChildrenJoints;
		ChildrenJoints	m_ChildrenJoints;	///< 子关节

		JointPoseAnimation*	m_JointAnim;	///< 当前使用的关节动画

		//Matrix4			m_Transform;			///< 局部变换
		Matrix4			m_BoneTransform;		///< 骨骼空间变换

		Skeleton*		m_Skeleton;				///< 关键使用的骨骼资源
	};

}

#endif
