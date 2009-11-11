//-----------------------------------------------------------------------------------
/// SceneObject.cpp �����������ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "SceneObject.h"

#include "Camera.h"
#include "Light.h"
#include "ParticleEmitter.h"
#include "ParticlePool.h"
#include "AudioListener.h"
#include "AudioSourceObject.h"
#include "Billboard.h"
#include "BspObject.h"
#include "Decal.h"
#include "DistantViewObject.h"
#include "MeshObject.h"
#include "SceneGraph.h"
#include "StringConverter.h"

namespace Gen
{
	SceneObjectFactoryCreators SceneObjectFactory::m_FactoryCreators;

	void SceneObjectFactory::RegisterFactoryCreator(const String& className, FactoryCreateFuncPtr ptrFunc)
	{
		AssertFatal(ptrFunc, "SceneObjectFactory::RegisterFactoryCreator(): Function pointer cannot be NULL!");
		AssertFatal(m_FactoryCreators.find(className)==m_FactoryCreators.end(),
			"SceneObjectFactory::RegisterFactoryCreator(): A function with same class name has been registered already.");

		m_FactoryCreators[className] = ptrFunc;
	}

	void SceneObjectFactory::RegisterDefaultFactories()
	{
		REGISTER_FACTORY(Camera);
		REGISTER_FACTORY(Light);
		REGISTER_FACTORY(ParticleEmitter);
		REGISTER_FACTORY(ParticlePool);
		REGISTER_FACTORY(AudioListener);
		REGISTER_FACTORY(AudioSourceObject);
		REGISTER_FACTORY(Billboard);
		REGISTER_FACTORY(BspObject);
		REGISTER_FACTORY(Decal);
		REGISTER_FACTORY(DistantViewObject);
		REGISTER_FACTORY(MeshObject);
	}

	SceneObject* SceneObjectFactory::CreateSceneObject(const String& className)
	{
		AssertFatal(m_FactoryCreators.find(className)!=m_FactoryCreators.end(),
			"SceneObjectFactory::CreateSceneObject(): Unable to create scene object, not a registered class name.");
		return (*m_FactoryCreators[className])();
	}


	SceneObject::SceneObject()
		: m_Visible(true),
		m_Scene(NULL),
		m_ParentObject(NULL),
		m_KeepRotation(false),
		m_CollisionGroupMask(0),		// Ĭ�ϲ������κ���ײ
		m_Serializable(false)
	{
	}

	SceneObject::~SceneObject()
	{
		// ɾ�����еĿ���Ⱦ�Ӷ���

		//RenderableObjectSet::iterator iter;
		//for (iter=m_RenderableObjects.begin(); iter!=m_RenderableObjects.end(); iter++)
		//{
		//	delete *iter;
		//}
	}

	void SceneObject::Update(unsigned long deltaTime)
	{
		// ���û��ຯ��������AABB
		MovableObjectBase::Update(deltaTime);

		if (m_ParentObject)
		{
			// ���������û�б����£�����и���
			if (!m_ParentObject->IsUpdated())
				m_ParentObject->Update(deltaTime);

			if (m_KeepRotation)
				m_WorldTransform.SetPosition((m_ParentObject->WorldTransform() * m_Transform).GetPosition());
			else
				m_WorldTransform = m_ParentObject->WorldTransform() * m_Transform;
		}
		else
		{
			m_WorldTransform = m_Transform;
		}

		// ���¸���obb����aabb
		m_AABB.Expand(m_OBB, m_WorldTransform);


		SceneObjectSet::iterator iter;
		for (iter = m_ChildrenObjects.begin();
			iter != m_ChildrenObjects.end();
			iter++)
		{
			(*iter)->Update(deltaTime);

			// ���Ӷ����aabbҲ������ȥ
			m_AABB.Expand((*iter)->m_AABB);
		}
	}
	//
	//void SceneObject::AddRenderableObject(RenderableObjectBase* renderable)
	//{
	//	// �Ϸ����ж�
	//	AssertFatal(m_RenderableObjects.find(renderable)==m_RenderableObjects.end(), "SceneObject::AddRenderableObject(): Scene object owns this renderable already.");
	//
	//	m_RenderableObjects.insert(renderable);
	//}
	//
	//void SceneObject::RemoveRenderableObject(RenderableObjectBase* renderable)
	//{
	//	// �Ϸ����ж�
	//	AssertFatal(m_RenderableObjects.find(renderable)!=m_RenderableObjects.end(), "SceneObject::RemoveRenderableObject(): Scene object doesn't own this renderable.");
	//
	//	m_RenderableObjects.erase(renderable);
	//	delete renderable;
	//}

	bool SceneObject::IsCulled(Frustum* frustum)
	{
		if (m_Visible)
			return MovableObjectBase::IsCulled(frustum);

		return true;
	}

	void SceneObject::SetSceneGraphRecursively(SceneGraph* scene)
	{
		m_Scene = scene;

		SceneObjectSet::iterator iter;
		for (iter=m_ChildrenObjects.begin(); iter!=m_ChildrenObjects.end(); iter++)
		{
			(*iter)->SetSceneGraphRecursively(scene);
		}
	}

	void SceneObject::CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum)
	{
		SceneObjectSet::iterator iter;
		for (iter=m_ChildrenObjects.begin(); iter!=m_ChildrenObjects.end(); iter++)
		{
			(*iter)->CollectRenderableObject(renderableObjs, frustum);
		}
	}

	void SceneObject::RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup)
	{
		CollisionResult result;

		if ((collisionGroup & m_CollisionGroupMask) &&
			//(type)
			IntersectsRay(ray, result))
		{
			sceneObjects.push_back(result);
		}

		SceneObjectSet::iterator iter;
		for (iter = m_ChildrenObjects.begin();
			iter != m_ChildrenObjects.end();
			iter++)
		{
			(*iter)->RayPickingSceneObject(ray, sceneObjects, type, collisionGroup);
		}
	}

	bool SceneObject::IntersectsRay(const Ray& ray, CollisionResult& info)
	{
		// AABB���
		Vector3f p;
		float d;
		bool result = ray.IntersectsBox(m_AABB.worldMin, m_AABB.worldMax, p, d);

		if (result)
		{
			info.point = p;
			info.distance = d;

			// TODO: ���߷���Ҫ����д
			info.normal = Vector3f(0.0f, 1.0f, 0.0f);
			info.obj = this;
		}

		return result;
	}

	bool SceneObject::PushSphere(const Vector3f& pos, Vector3f& newpos, float radius)
	{
		bool result = false;
		SceneObjectSet::iterator iter;
		for (iter = m_ChildrenObjects.begin();
			iter != m_ChildrenObjects.end();
			iter++)
		{
			result |= (*iter)->PushSphere(pos, newpos, radius);
		}

		return result;
	}


	void SceneObject::AttachChildObject(SceneObject* child, bool keepRotation, bool keepWorldTransform)
	{
		// �����Ӷ������ȷ����ͬһ��������
		AssertFatal(child->GetSceneGraph() && (child->GetSceneGraph()==GetSceneGraph()),
			"SceneObject::AttachChildObject(): Unable to attach a object that in a different scene graph or without one.");

		// ���child�Ƿ��Ѿ��������б���
		if (m_ChildrenObjects.find(child) != m_ChildrenObjects.end())
		{
			return;
		}

		m_ChildrenObjects.insert(child);

		// ������и����󣬽����ϵ
		if (child->m_ParentObject)
		{
			child->DetachFromParent(keepWorldTransform);
		}
		child->SetParentObject(this, keepRotation);

		// �ݹ�ָ������ĳ���
		child->SetSceneGraphRecursively(m_Scene);

		// ά���Ӷ���������ռ��еı任
		if (keepWorldTransform)
		{
			// TODO: Implement this...
			//child->Transform() = child->WorldTransform() * child->;
		}
		else // ά�ֶ���ǰ�ľֲ��ռ�任
		{
			child->m_WorldTransform = WorldTransform() * child->Transform();
		}

	}

	void SceneObject::DetachFromParent(bool keepWorldTransform)
	{
		AssertFatal(m_ParentObject, "SceneObject::DetachFromParent(): Object has got no parent.");

		if (keepWorldTransform)
		{
			// TODO: �޸�Transform
		}

		// �Ӹ�������Ӷ����б����Ƴ��Լ�
		m_ParentObject->m_ChildrenObjects.erase(this);
		m_ParentObject = NULL;
		//SetSceneGraph(NULL);
	}

	void SceneObject::DeleteAllChildren()
	{
		SceneObjectSet::iterator iter;
		while ((iter = m_ChildrenObjects.begin())  != m_ChildrenObjects.end())
		{
			SceneObject* sceneObj = *iter;
			sceneObj->DetachFromParent(false);
			sceneObj->DeleteAllChildren();
			m_Scene->RemoveObject(sceneObj, true, true);
		}

		m_ChildrenObjects.clear();
	}

	bool SceneObject::OnSave(SceneSerializerNode* node)
	{
		if (!m_Serializable) return false;

		SceneObjectSet::iterator iter;
		for (iter=m_ChildrenObjects.begin(); iter!=m_ChildrenObjects.end(); iter++)
		{
			SceneSerializerNode childNode(node);
			childNode.SaveSceneObject(*iter);
		}

		node->SaveAttribute("type", GetTypeName().Data());
		node->SaveAttribute("position", ToString(Transform().GetPosition()).Data());
		node->SaveAttribute("quaternion", ToString(GetLocalQuaternion()).Data());

		return true;
	}

	void SceneObject::OnRestore(SceneSerializerNode* node)
	{
		this->SetPosition(ToVector3f(node->GetAttribute("position")));
		this->SetRotation(ToQuaternion(node->GetAttribute("quaternion")));
	}

	void SceneObject::DumpToLogRecursively(int level)
	{
		String msg, name;
		for (int i=0; i<level; i++)
			msg += "   ";
		name.Format("+- %s", GetTypeName().Data());
		msg += name;
		Log.MsgLn(msg);

		SceneObjectSet::iterator iter;
		for (iter = m_ChildrenObjects.begin();
			iter != m_ChildrenObjects.end();
			iter++)
		{
			(*iter)->DumpToLogRecursively(level+1);
		}
	}


	void SceneObject::SetParentObject(SceneObject* parent, bool keepRotation)
	{
		m_ParentObject = parent;
		m_KeepRotation = keepRotation;
	}
}
