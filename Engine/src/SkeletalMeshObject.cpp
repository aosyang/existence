//-----------------------------------------------------------------------------------
/// SkeletalMeshObject.cpp 骨骼模型对象实现代码
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "SkeletalMeshObject.h"
#include "Renderer.h"
#include "Log.h"
#include "MeshElement.h"
#include "MeshManager.h"
#include "SkeletonManager.h"
#include "Skeleton.h"
#include "DebugRenderer.h"

namespace Gen
{
	SkeletalMeshObject::SkeletalMeshObject(SceneGraph* scene)
		: MeshObject(scene),
		  m_RootJoint(NULL),
		  m_Skeleton(NULL),
		  m_VertexBuffer(NULL),
		  m_VertexArray(NULL),
		  m_NormalArray(NULL),
		  m_CurrentAnimation(NULL),
		  m_CurAnimationTime(0.0f),
		  m_NeedUpdateMesh(true)
	{
	}

	SkeletalMeshObject::~SkeletalMeshObject()
	{
		SAFE_DELETE(m_VertexBuffer);
		//SAFE_DELETE(m_RootJoint);
		SAFE_DELETE_ARRAY(m_VertexArray);
		SAFE_DELETE_ARRAY(m_NormalArray);
	}

	void SkeletalMeshObject::Update(unsigned long deltaTime)
	{
		if (m_CurrentAnimation)
		{
			// 递进动画时间
			AdvanceTime(deltaTime);

			m_RootJoint->Update(m_CurAnimationTime);

			m_NeedUpdateMesh = true;
		}

		// 更新顶点
		//UpdateMesh();

		BaseClass::Update(deltaTime);
	}

	void SkeletalMeshObject::SetMesh(const String& meshName)
	{
		BaseMesh* mesh = MeshManager::Instance().GetByName(meshName);
		SetMesh(mesh);
	}

	void SkeletalMeshObject::SetMesh(BaseMesh* mesh)
	{
		MeshObject::SetMesh(mesh);

		if (mesh)
		{
			// 为模型创建动画用的顶点缓冲
			m_VertexBuffer = mesh->BuildVertexBufferCopy();

			// 分配顶点及法线数组
			SAFE_DELETE_ARRAY(m_VertexArray);
			SAFE_DELETE_ARRAY(m_NormalArray);

			int vertCount = mesh->GetVertexCount();

			m_VertexArray = new float[vertCount * 3];
			m_NormalArray = new float[vertCount * 3];
		}
	}
	
	void SkeletalMeshObject::DebugRender()
	{
		MeshObject::DebugRender();

		//if (m_RootJoint)
		//	m_RootJoint->DebugRender(m_WorldTransform);
	}
	
	void SkeletalMeshObject::RenderSingleObject()
	{
		// 不要调用MeshObject的渲染方法
		BaseClass::BaseClass::RenderSingleObject();

		// 更新顶点
		if (m_NeedUpdateMesh)
			UpdateMesh();

		if (m_Mesh)
		{
			// TODO: 以后使用这种方式渲染
			//m_Mesh->RenderMesh(&m_WorldTransform);

			for (int i=0; i<m_Mesh->GetElementCount(); i++)
			{
				MeshElement* elem = m_Mesh->GetElement(i);

				RenderCommand cmd;
				cmd.indexBuffer = elem->GetIndexBuffer();
				cmd.vertexBuffer = m_VertexBuffer;
				cmd.primType = PRIM_TRIANGLES;
				cmd.transform = m_WorldTransform;
				cmd.material = m_Materials[i];
				cmd.renderOrder = m_RenderOrder;
				cmd.sceneObj = this;

				Renderer::Instance().SubmitRenderCommand(cmd);


				//Renderer::Instance().SetupMaterial(m_Materials[i]);
				//Renderer::Instance().RenderPrimitives(m_VertexBuffer,
				//									  elem->GetIndexBuffer(),
				//									  m_WorldTransform);
			}
		}
	}

	void SkeletalMeshObject::SetSkeleton(const String& skelName)
	{
		Skeleton* skel = SkeletonManager::Instance().GetByName(skelName);
		SetSkeleton(skel);
	}

	void SkeletalMeshObject::SetSkeleton(Skeleton* skeleton)
	{
		m_Skeleton = skeleton;

		CreateJointInstance();
	}

	void SkeletalMeshObject::PlayAnimation(const String& animName, bool resetTargetAnim)
	{
		m_CurrentAnimation = m_Skeleton->GetAnimation(animName);
		if (!m_CurrentAnimation)
		{
			String msg;
			msg.Format("Warning: Unable to find animation %s for object", animName.Data());
			Log.MsgLn(msg);
		}

		if (resetTargetAnim)
		{
			m_CurAnimationTime = 0.0f;
		}

		m_RootJoint->SetAnimation(m_CurrentAnimation);
	}

	void SkeletalMeshObject::SetAnimationTime(float time)
	{
		m_CurAnimationTime = time;
	}

	void SkeletalMeshObject::AddJointAnchor(const String& anchorName, int jointId)
	{
		AddAnchor(anchorName, m_JointInstanceMap[jointId]);
	}

	void SkeletalMeshObject::CreateJointInstance()
	{
		if (!m_Skeleton) return;

		// 激活资源
		m_Skeleton->Trigger();


		// 从骨骼资源创建骨骼关系实例
		Skeleton::JointList::iterator iter;
		for (iter=m_Skeleton->m_JointList.begin();
			 iter!=m_Skeleton->m_JointList.end();
			 iter++)
		{
			int parent = iter->second->GetParentId();
			int id = iter->second->GetJointId();
			SkeletalJointInstance* inst = new SkeletalJointInstance(id, parent, m_Skeleton);

			// 没有父结点的关节作为根节点
			if (parent==-1)
			{
				m_RootJoint = inst;
			}

			m_JointInstanceMap[id] = inst;
		}

		// 确立父子关系
		for (std::map<int, RefPtr<SkeletalJointInstance> >::iterator iter=m_JointInstanceMap.begin();
			 iter!=m_JointInstanceMap.end();
			 iter++)
		{
			if (iter->second->GetParentId()!=-1)
			{
				int id = iter->second->GetJointId();
				int parent = iter->second->GetParentId();
				m_JointInstanceMap[parent]->AddChildJoint(iter->second);
			}
		}

		// 更新骨骼默认位置
		m_RootJoint->Update(0.0f);

		//skel->m_JointList
	}

	void SkeletalMeshObject::AdvanceTime(unsigned long deltaTime)
	{
		m_CurAnimationTime += (float)deltaTime * 4.8f / m_CurrentAnimation->GetTicksPerSample()/* / 50.0f*/;

		while (m_CurAnimationTime>=m_CurrentAnimation->GetKeyFrameCount() - 1)
			m_CurAnimationTime -= m_CurrentAnimation->GetKeyFrameCount() - 1;
		while (m_CurAnimationTime<0)
			m_CurAnimationTime += m_CurrentAnimation->GetKeyFrameCount() - 1;
	}
		
	void SkeletalMeshObject::UpdateMesh()
	{
		// 没有赋予骨骼，不进行更新
		if (!m_Skeleton) return;

		int vertCount = m_Mesh->GetVertexCount();

		//m_OBB.Reset();

		SkinnedVertexInfo* skin = m_Mesh->GetSkin();

		for (int i=0; i<vertCount; i++)
		{
			// 顶点的坐标和法线方向从原始模型的数组中获取
			Vector3f p = Vector3f(&m_Mesh->GetVertexArray()[i * 3]);
			Vector3f n = Vector3f(&m_Mesh->GetNormalArray()[i * 3]);

			Vector3f p2 = Vector3f(0.0f, 0.0f, 0.0f),
					 n2 = Vector3f(0.0f, 0.0f, 0.0f);

			// hack:
			//static const Matrix4 boneTrans = m_JointInstanceMap[2461]->GetBoneTransform();

			for (unsigned int j=0; j<skin[i].boneCount; j++)
			{
				int boneId = skin[i].boneId[j];
				float weight = skin[i].weight[j];

				SkeletalJoint* joint = m_Skeleton->GetJoint(boneId);

				// 将顶点变换到初始骨骼节点的空间中
				// p in object space
				Vector3f pInv = joint->GetOriginTransformInv() * p;
				Vector3f nInv = joint->GetOriginTransformInv().GetRotationMatrix() * n;
				//Vector3f pInv = Vector3f(0.0f, 0.0f, 0.0f);
				//Vector3f nInv = Vector3f(0.0f, 0.0f, 0.0f);
				//Vector3f nInv = joint->GetOriginTransform().GetInverseMatrix().GetRotationMatrix() * n;

				// 为每个顶点累积权重与变换
				const Matrix4& boneTransform = m_JointInstanceMap[boneId]->GetTransformToParentObject();
				//const Matrix4& boneTransform = boneTrans;
				p2 += boneTransform * pInv * weight;
				//p2 += pInv * weight;

				// 法线的混合
				// 法线仅仅需要计算旋转量
				n2 += boneTransform.GetRotationMatrix() * nInv * weight;

			}

			//n2.normalize();

			// 向顶点数组中回写修改后的坐标，重新生成缓冲
			memcpy(&m_VertexArray[i * 3], p2.GetArray(), sizeof(float)*3);
			memcpy(&m_NormalArray[i * 3], n2.GetArray(), sizeof(float)*3);
			//m_OBB.Expand(p2);
		}

		// 重新构造顶点缓冲
		m_VertexBuffer->Clear();
		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0,
									 m_VertexArray,
									 m_NormalArray,
									 m_Mesh->GetColorArray(),
									 m_Mesh->GetTexCoordArray(),
									 vertCount);

		m_NeedUpdateMesh = false;
	}

	// ----------------------------------- SkeletalJointInstance

	SkeletalJointInstance::SkeletalJointInstance(int id, int parentId, Skeleton* skeleton)
		: m_JointId(id),
		  m_ParentId(parentId),
		  m_JointAnim(NULL),
		  m_Skeleton(skeleton)
	{
	}

	SkeletalJointInstance::~SkeletalJointInstance()
	{
		// NOTE: 使用了RefPtr，子关节无须删除
		// 删除所有子节点
		//ChildrenJoints::iterator iter;
		//for (iter=m_ChildrenJoints.begin();
		//	 iter!=m_ChildrenJoints.end();
		//	 iter++)
		//{
		//	delete (*iter);
		//}
	}

	void SkeletalJointInstance::AddChildJoint(RefPtr<SkeletalJointInstance> child)
	{
		m_ChildrenJoints.insert(child);
	}

	void SkeletalJointInstance::DebugRender(const Matrix4& world)
	{
		Matrix4 jointWorld = world * m_BoneTransform;
		//DebugRender::Instance().DrawBox(Vector3f(-1.0f, -1.0f, -1.0f), Vector3f(1.0f, 1.0f, 1.0f), Color4f(0.0f, 1.0f, 1.0f), jointWorld);

		//Vector3f pos = jointWorld.GetPosition();
		//Vector3f right = jointWorld.GetRightVector();
		//Vector3f up = jointWorld.GetUpVector();
		//Vector3f forward = jointWorld.GetForwardVector();
		//DebugRenderer::Instance().DrawLine(pos, pos + right, Color4f(1.0f, 0.0f, 0.0f));
		//DebugRenderer::Instance().DrawLine(pos, pos + up, Color4f(0.0f, 1.0f, 0.0f));
		//DebugRenderer::Instance().DrawLine(pos, pos - forward, Color4f(0.0f, 0.0f, 1.0f));

		ChildrenJoints::iterator iter;
		for (iter=m_ChildrenJoints.begin();
			 iter!=m_ChildrenJoints.end();
			 iter++)
		{
			DebugRenderer::Instance().DrawLine(jointWorld.GetPosition(), (world * (*iter)->m_BoneTransform).GetPosition());
			(*iter)->DebugRender(world);
		}
	}

	void SkeletalJointInstance::Update(float animTime, const Matrix4& parentBoneTransform)
	{
		if (m_JointAnim)
		{
			// 取得这个时间点前后的关键帧编号
			float floorVal = floor(animTime);
			float ceilVal = ceil(animTime);

#define USE_INTERPOLATION

#if defined USE_INTERPOLATION
			// 对位置和旋转量分别进行插值
			Vector3f pos = (m_JointAnim->PosArray((int)ceilVal) - m_JointAnim->PosArray((int)floorVal)) * (animTime - floorVal) + m_JointAnim->PosArray((int)floorVal);
			Quaternion quat = Quaternion::Slerp(m_JointAnim->QuatArray((int)floorVal), m_JointAnim->QuatArray((int)ceilVal), animTime - floorVal);
#else
			Vector3f pos = m_JointAnim->PosArray((int)floorVal);
			Quaternion quat = m_JointAnim->QuatArray((int)floorVal);
#endif
			quat.Normalize();

			m_Transform.MakeIdentity();
			m_Transform.SetPosition(pos);
			m_Transform.SetRotation(quat);
			//m_Transform *= m_InitTransform.GetInverseMatrix();

			// 骨骼变换记录骨骼在层次动画中相对于根节点的变换
			m_BoneTransform = parentBoneTransform * m_Transform;
			////m_OriginTransformInve = m_BoneTransform.GetInverseMatrix();

			//m_WorldTransform = parentWorldTransform/* * m_InitTransform*/ * m_Transform;
		}
		else
		{
			m_BoneTransform = parentBoneTransform * m_Skeleton->GetJoint(m_JointId)->GetInitTransform();
			//m_WorldTransform = parentWorldTransform * m_InitTransform;
		}

		ChildrenJoints::iterator iter;
		for (iter=m_ChildrenJoints.begin();
			iter!=m_ChildrenJoints.end();
			iter++)
		{
			(*iter)->Update(animTime, m_BoneTransform);
		}
	}

	void SkeletalJointInstance::SetAnimation(SkeletalAnimation* anim)
	{
		if (anim)
			m_JointAnim = anim->GetJointPoseAnim(m_JointId);
		else
			m_JointAnim = NULL;

		// 递归更新子节点的骨骼动画
		ChildrenJoints::iterator iter;
		for (iter=m_ChildrenJoints.begin();
			iter!=m_ChildrenJoints.end();
			iter++)
		{
			(*iter)->SetAnimation(anim);
		}
	}
}
