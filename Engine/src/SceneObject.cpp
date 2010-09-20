//-----------------------------------------------------------------------------------
/// SceneObject.cpp 场景对象类的实现
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
#include "SkeletalMeshObject.h"

#include "SceneGraph.h"
#include "Engine.h"
#include "StringConverter.h"
#include "Renderer.h"
#include "DebugRenderer.h"

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
		REGISTER_FACTORY(SkeletalMeshObject);
	}

	SceneObject* SceneObjectFactory::CreateSceneObject(SceneGraph* scene, const String& className)
	{
		AssertFatal(m_FactoryCreators.find(className)!=m_FactoryCreators.end(),
			"SceneObjectFactory::CreateSceneObject(): Unable to create scene object, not a registered class name.");
		return (*m_FactoryCreators[className])(scene);
	}

	bool SceneObject::m_sDebugRender = false;

	SceneObject::SceneObject(SceneGraph* scene)
	: MovableObjectBase(),
	  m_AttachType(ATTACH_TYPE_NONE),
	  m_Visible(true),
	  m_Scene(scene),
	  m_ParentObject(NULL),
	  m_ParentAnchor(NULL),
	  m_KeepRotation(false),
	  m_CollisionGroupMask(0),		// 默认不参与任何碰撞
	  m_RenderOrder(100),
	  m_Serializable(false)
	{
	}

	SceneObject::~SceneObject()
	{
		// 删除所有锚点
		// NOTE: 骨骼动画的锚点应由骨骼动画自身负责删除
		//AnchorMap::iterator iter;
		//for (iter=m_Anchors.begin(); iter!=m_Anchors.end(); iter++)
		//{
		//	SAFE_DELETE(iter->second);
		//}
	}

	void SceneObject::Update(unsigned long deltaTime)
	{
		// 调用基类函数，重置AABB
		MovableObjectBase::Update(deltaTime);

		switch (m_AttachType)
		{
		case ATTACH_TYPE_NONE:
			m_WorldTransform = m_Transform;
			break;
		case ATTACH_TYPE_PARENT_OBJECT:
			if (m_KeepRotation)
				m_WorldTransform.SetPosition((m_ParentObject->WorldTransform() * m_Transform).GetPosition());
			else
				m_WorldTransform = m_ParentObject->WorldTransform() * m_Transform;
			break;
		case ATTACH_TYPE_PARENT_ANCHOR:
			m_WorldTransform = m_ParentObject->WorldTransform() * m_ParentAnchor->GetTransformToParentObject() * m_Transform;
			break;
		}

		// 重新根据obb计算aabb
		m_AABB.Expand(m_OBB, m_WorldTransform);


		SceneObjectSet::iterator iter;
		for (iter = m_ChildrenObjects.begin();
			iter != m_ChildrenObjects.end();
			iter++)
		{
			(*iter)->Update(deltaTime);

			// 将子对象的aabb也包括进去
			m_AABB.Expand((*iter)->m_AABB);
		}
	}


	bool SceneObject::IsCulled(Frustum* frustum)
	{
		if (m_Visible)
		{
			if (!frustum) return false;

			if (frustum->IntersectsAABB(m_AABB)) return false;
		}

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
		// AABB检测
		Vector3f p;
		float d;
		bool result = ray.IntersectsBox(m_AABB.worldMin, m_AABB.worldMax, p, d);

		if (result)
		{
			info.point = p;
			info.distance = d;

			// TODO: 法线方向不要这样写
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
		// 所附加对象必须确保在同一个场景中
		AssertFatal(child->GetSceneGraph() && (child->GetSceneGraph()==GetSceneGraph()),
			"SceneObject::AttachChildObject(): Unable to attach a object that in a different scene graph or without one.");

		// 检查child是否已经存在于列表中
		if (m_ChildrenObjects.find(child) != m_ChildrenObjects.end())
		{
			return;
		}

		m_ChildrenObjects.insert(child);

		// 如果已有父对象，解除关系
		if (child->m_AttachType!=ATTACH_TYPE_NONE)
		{
			child->DetachFromParent(keepWorldTransform);
		}
		child->SetParentObject(this, keepRotation);
		child->m_AttachType = ATTACH_TYPE_PARENT_OBJECT;

		// 递归指定物体的场景
		// TODO: 不需要了吧？
		child->SetSceneGraphRecursively(m_Scene);

		// 维持子对象在世界空间中的变换
		if (keepWorldTransform)
		{
			// 将子结点的空间变换到父结点空间中
			child->m_Transform = child->WorldTransform() * WorldTransform().GetInverseMatrix();
		}
		else // 维持对象当前的局部空间变换
		{
			child->m_WorldTransform = WorldTransform() * child->Transform();
		}

	}

	void SceneObject::AttachChildObjectToAnchor(SceneObject* child, const String& anchorName)
	{
		// 所附加对象必须确保在同一个场景中
		AssertFatal(child->GetSceneGraph()==GetSceneGraph(),
			"SceneObject::AttachChildObjectToAnchor(): Unable to attach a object that in a different scene graph.");

		// 没有指定名称的锚点，返回
		if (m_Anchors.find(anchorName)==m_Anchors.end()) return;

		// 检查child是否已经存在于列表中
		if (m_ChildrenObjects.find(child) != m_ChildrenObjects.end())
		{
			return;
		}

		m_ChildrenObjects.insert(child);

		// 如果已有父对象，解除关系
		if (child->m_AttachType!=ATTACH_TYPE_NONE)
		{
			child->DetachFromParent(false);
		}
		child->SetParentObject(this, false);
		child->m_AttachType = ATTACH_TYPE_PARENT_ANCHOR;

		RefPtr<Anchor> anchor = m_Anchors[anchorName];
		child->m_ParentAnchor = anchor;
	}

	void SceneObject::DetachFromParent(bool keepWorldTransform)
	{
		AssertFatal(m_ParentObject, "SceneObject::DetachFromParent(): Object has got no parent.");

		if (keepWorldTransform)
		{
			m_Transform = m_ParentObject->WorldTransform() * m_Transform;
		}

		// 从父对象的子对象列表中移除自己
		m_ParentObject->m_ChildrenObjects.erase(this);
		m_ParentObject = NULL;
		//SetSceneGraph(NULL);
		m_ParentAnchor = NULL;
		m_AttachType = ATTACH_TYPE_NONE;
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

	void SceneObject::PrepareObjectRendering(Frustum* frustum)
	{
		// 如果对象不可见，返回
		if (IsCulled(frustum)) return;

		RenderSingleObject();
		if (m_sDebugRender)
		{
			DebugRender();
		}

		// 遍历所有子节点
		SceneObjectSet::iterator iter;
		for (iter = m_ChildrenObjects.begin();
			iter != m_ChildrenObjects.end();
			iter++)
		{
			(*iter)->PrepareObjectRendering(frustum);
		}
	}

	void SceneObject::RenderSingleObject()
	{

	}

	void SceneObject::SetupLightForRendering()
	{
		// Disable all lights
		int maxLightNum = Renderer::Instance().GetMaxLightCount();

		for (int i=0; i<maxLightNum; i++)
		{
			Renderer::Instance().SetupLight(i, NULL);
		}
	}

	// Debug渲染
	void SceneObject::DebugRender()
	{
		DebugRenderAABB();
		DebugRenderOBB();
	}

	void SceneObject::DebugRenderAABB()
	{
		DebugRenderer::Instance().DrawBox(m_AABB.worldMin, m_AABB.worldMax);
	}

	void SceneObject::DebugRenderOBB()
	{
		DebugRenderer::Instance().DrawBox(m_OBB.localMin, m_OBB.localMax, Color4f(0.0f, 1.0f, 0.0f), WorldTransform());

	}

	void SceneObject::AddAnchor(const String& anchorName, RefPtr<Anchor> anchor)
	{
		AssertFatal(m_Anchors.find(anchorName)==m_Anchors.end(),
					"SceneObject::AddAnchor() : Duplicated anchor name found.");

		if (!anchor) return;

		m_Anchors[anchorName] = anchor;
	}
}

