//-----------------------------------------------------------------------------------
/// SkeletalMeshObject.h ����ģ�Ͷ��󣬹���ģ�͵�ʵ�������ƶ��������¶�����Ϣ
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

	// ����ģ�Ͷ���
	// �����й���ģ�͵�ʵ��������
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

		// ���Ŷ���
		void PlayAnimation(const String& animName, bool resetTargetAnim=true);

		// ָ����������λ��
		void SetAnimationTime(float time);

	protected:
		// ʹ�ù�����Ϣ��������ʵ��
		void CreateJointInstance();

		void AdvanceTime(unsigned long deltaTime);

		// ʹ�õ�ǰ����״̬����ģ��
		void UpdateMesh();

	protected:
		SkeletalJointInstance*		m_RootJoint;
		Skeleton*					m_Skeleton;
		IVertexBuffer*				m_VertexBuffer;

		float*						m_VertexArray;
		float*						m_NormalArray;

		map<int, SkeletalJointInstance*>	m_JointInstanceMap;

		SkeletalAnimation*			m_CurrentAnimation;		///< ��ǰ���ŵĶ���
		float						m_CurAnimationTime;		///< ��ǰ����ʱ��
	};


	// �����ؽ�ʵ������¼�ؽ�֮��ĳʱ�̵Ĺ�ϵ
	class SkeletalJointInstance
	{
		friend class Skeleton;
	public:
		SkeletalJointInstance(int id, int parentId, Skeleton* skeleton);
		~SkeletalJointInstance();

		int GetJointId() const { return m_JointId; }

		// ��ȡ���ڵ�id
		int GetParentId() const { return m_ParentId; }

		inline const Matrix4& GetBoneTransform() const { return m_BoneTransform; }

		// ����ӹؽ�
		void AddChildJoint(SkeletalJointInstance* child);

		// debug��Ⱦ
		void DebugRender(const Matrix4& world);

		// ���¹�����ϵ
		void Update(float animTime, const Matrix4& parentBoneTransform=Matrix4::IDENTITY);

		// ָ������
		void SetAnimation(SkeletalAnimation* anim);
	private:
		int				m_JointId;			///< �ؽ�id
		int				m_ParentId;			///< ���ڵ�id

		typedef set<SkeletalJointInstance*>	ChildrenJoints;
		ChildrenJoints	m_ChildrenJoints;	///< �ӹؽ�

		JointPoseAnimation*	m_JointAnim;	///< ��ǰʹ�õĹؽڶ���

		Matrix4			m_Transform;			///< �ֲ��任
		Matrix4			m_BoneTransform;		///< �����ռ�任

		Skeleton*		m_Skeleton;				///< �ؼ�ʹ�õĹ�����Դ
	};

}

#endif
