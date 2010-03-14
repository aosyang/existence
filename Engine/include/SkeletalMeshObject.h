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

	/// @brief
	/// ����ģ�Ͷ���
	/// @par
	///		�������ӵ������Ĺ���ģ��ʵ��������
	/// @remarks
	///		Ϊ����ģ�Ͷ���ָ����ģ���Ժ󣬻��ģ���и���һ�ݶ����������ڶ�������
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

		// ----- Overwrite SceneObject
		/// @copydoc SceneObject::DebugRender()
		void DebugRender();

		// ----- Overwrite IRenderableObject
		void RenderSingleObject();

		// ----- SkeletalMeshObject
		/// @brief
		/// Ϊ����ģ�Ͷ���ָ��������Ϣ
		/// @param skeleton
		///		��Ҫָ���Ĺ�����Ϣ
		void SetSkeleton(Skeleton* skeleton);

		/// @brief
		/// ����һ����������
		/// @param animName
		///		��������
		/// @param resetTargetAnim
		///		�Ƿ����������ŵĶ���ʱ��㣬Ϊtrueʱ����ͷ��ʼ���Ŷ�����Ϊfalseʱ������һ�ζ���������ʱ����������
		void PlayAnimation(const String& animName, bool resetTargetAnim=true);

		/// @brief
		///	ָ����������ʱ���
		/// @param time
		///		����ʱ���(��λ����)
		void SetAnimationTime(float time);

	protected:
		/// @brief
		/// ʹ�ù�����Ϣ��������ʵ��
		void CreateJointInstance();

		/// @brief
		///	�ݽ�����������ʱ��
		/// @param deltaTime
		///		�ݽ�ʱ��(��λ������)
		void AdvanceTime(unsigned long deltaTime);

		/// @brief
		/// ʹ�õ�ǰ����״̬����ģ�͵Ķ���λ�ü����߷���
		void UpdateMesh();

	protected:
		SkeletalJointInstance*		m_RootJoint;			///< ���������
		Skeleton*					m_Skeleton;				///< ������Ϣ
		IVertexBuffer*				m_VertexBuffer;			///< �����ö��㻺�帱��

		float*						m_VertexArray;			///< ��������
		float*						m_NormalArray;			///< ��������

		map<int, SkeletalJointInstance*>	m_JointInstanceMap;	///< �����ؽ�ʵ���б�

		SkeletalAnimation*			m_CurrentAnimation;		///< ��ǰ���ŵĶ���
		float						m_CurAnimationTime;		///< ��ǰ����ʱ��
	};


	/// @brief
	/// �����ؽ�ʵ��
	/// @par
	///		�����ؽ�ʵ���Ǹ��ݹ�����ϢΪһ������ģ�Ͷ��󴴽��Ĺؽ�ʵ������<br>
	///		ÿ�������¼һ���ؽ���ĳʱ�̵�λ�ü���ת״̬������ģ�Ͷ����¼һ��<br>
	///		�����ؽ�ʵ������Ϣ����������Ƥ�͹����ؽ�ʵ����״̬���������λ�á�<br>
	class SkeletalJointInstance
	{
		friend class Skeleton;
	public:
		/// @brief
		///	���캯��
		///	@param id
		///		�ؽ�id
		///	@param parentId
		///		�����ؽ�id
		///	@param skeleton
		///		������Ϣ
		SkeletalJointInstance(int id, int parentId, Skeleton* skeleton);
		~SkeletalJointInstance();

		/// @brief
		///	��ȡ�ؽڵ�id
		int GetJointId() const { return m_JointId; }

		/// @brief
		/// ��ȡ���ؽ�id
		int GetParentId() const { return m_ParentId; }

		/// @brief
		///	��ȡ�ùؽ�����ڹ�����θ����ı任
		inline const Matrix4& GetBoneTransform() const { return m_BoneTransform; }

		/// @brief
		/// ����һ���ӹؽ�
		/// @param child
		///		�ӹؽ�
		void AddChildJoint(SkeletalJointInstance* child);

		/// @brief
		/// ��Ⱦ������Ϣ
		/// @param world
		///		��������������任����
		/// @remarks
		///		��������ݹ�����ӽ�����ͬ����������Ⱦ���
		void DebugRender(const Matrix4& world);

		/// @brief
		/// ���¹�����ϵ
		/// @param animTime
		///		������ǰ����ʱ��(��λ����)
		/// @param parentBoneTransform
		///		���������任����
		void Update(float animTime, const Matrix4& parentBoneTransform=Matrix4::IDENTITY);

		/// @brief
		/// Ϊ�ؽ�ָ������
		/// @param anim
		///		ָ���Ĺ�������
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