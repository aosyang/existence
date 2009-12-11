//-----------------------------------------------------------------------------------
/// SceneGraph.cpp 场景图，管理添加到场景的对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "SceneGraph.h"
#include "Engine.h"
#include "Frustum.h"
#include "LightingManager.h"
#include "ShadowManager.h"
#include <algorithm>

namespace Gen
{
	bool RenderableObjectBaseComparer(RenderableObjectBase* lhs, RenderableObjectBase* rhs)
	{
		// TODO: 对可渲染物体的排序要从材质是否透明、与镜头距离等方向来比较
		return lhs->GetRenderOrder() < rhs->GetRenderOrder();
	}

	SceneGraph::SceneGraph()
		: //m_RootObject(NULL),
	m_ProcessingUpdate(false)
	{
		// 创建用于管理其他对象的根对象
		//m_RootObject = new SceneRootObject();
	}

	SceneGraph::~SceneGraph()
	{
		SceneObjectSet::iterator iter;

		// 首先调用每个需要销毁的对象的destroy方法
		// 注：不要在访问列表过程中删除对象，否则会造成迭代器失效
		// TODO: Destroy方法中删除对象是否会造成迭代器失效？

		//for (iter=m_AutoDeleteList.begin(); iter!=m_AutoDeleteList.end(); iter++)
		//{
		//	(*iter)->Destroy();
		//}

		//for (iter=m_AutoDeleteList.begin(); iter!=m_AutoDeleteList.end(); iter++)
		//{
		//	delete (*iter);
		//}

		iter=m_AutoDeleteList.begin();
		while (iter!=m_AutoDeleteList.end())
		{
			(*iter)->Destroy();
			delete (*iter);

			m_AutoDeleteList.erase(iter);

			iter=m_AutoDeleteList.begin();
		}

		//SAFE_DELETE(m_RootObject)
	}

	void SceneGraph::AddObject(SceneObject* object, bool autoDelete)
	{
		//m_SceneObjects.push_back(object);

		//m_RootObject->AttachChildObject(object);
		//object->m_Scene = this;

		AssertFatal(m_SceneObjects.find(object)==m_SceneObjects.end(), "SceneGraph::AddObject(): Object has been already added to the scene");
		m_SceneObjects.insert(object);

		if (autoDelete)
		{
			m_AutoDeleteList.insert(object);
		}

		object->SetSceneGraphRecursively(this);
	}

	void SceneGraph::RemoveObject(SceneObject* object, bool deleteObj, bool recursive)
	{
		// 当在Update循环内部执行删除操作的时候，添加到删除列表并稍后销毁
		if (m_ProcessingUpdate)
		{
			RemoveListElement elem;
			elem.obj = object;
			elem.deleteObj = deleteObj;
			elem.recursive = recursive;
			m_RemoveList.push_back(elem);
		}
		else	// 否则直接删除对象
		{
			RemoveSceneObjectInternal(object, deleteObj, recursive);
		}
	}

	SceneObject* SceneGraph::CreateSceneObject(const String& className)
	{
		SceneObject* sceneObj = m_ObjectFactory.CreateSceneObject(this, className);
		AddObject(sceneObj);

		return sceneObj;
	}

	void SceneGraph::UpdateScene(unsigned long deltaTime)
	{
		m_ProcessingUpdate = true;
		// 通过根对象更新每个子对象
		//m_RootObject->Update(deltaTime);

		SceneObjectSet::iterator iter;
		for (iter=m_SceneObjects.begin(); iter!=m_SceneObjects.end(); iter++)
		{
			//if (!(*iter)->IsUpdated())
			(*iter)->Update(deltaTime);
		}

		m_ProcessingUpdate = false;

		ProcessRemove();

		LightingManager::Instance().Update();
	}

	// 设置渲染视点信息
	void SceneGraph::SetupRenderView(const RenderView& view)
	{
		m_RenderView = view;

		renderer->ViewMatrix() = view.viewMatrix;
		renderer->ProjectionMatrix() = view.projMatrix;
	}

	void SceneGraph::RenderScene()
	{
		//ShadowManager::Instance().RenderLightViewScene(m_RootObject);

		// 渲染场景

		RenderableObjectList renderList;

		SceneObjectSet::iterator iter;
		for (iter=m_SceneObjects.begin(); iter!=m_SceneObjects.end(); iter++)
		{
			(*iter)->CollectRenderableObject(renderList, m_RenderView.frustum);
		}

		Engine::Instance().SetRenderBatchCount(renderList.size());

		// 对所有对象进行排序
		sort(renderList.begin(), renderList.end(), RenderableObjectBaseComparer);

		//m_RootObject->CollectRenderObjects(m_RenderView);
		renderer->SetProjectionMode(PROJECTION_MODE_PERSPECTIVE);
		renderer->ClearBuffer(
#if defined __PLATFORM_LINUX	// 为何在glx下面清除深度缓冲会崩溃？
			true, false
#endif
			);
		renderer->BeginRender();
		//m_RootObject->Render();
		RenderableObjectList::iterator renderIter;

		for (renderIter=renderList.begin(); renderIter!=renderList.end(); renderIter++)
		{
			(*renderIter)->RenderSingleObject();
		}

		renderer->EndRender();
	}

	void SceneGraph::RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup)
	{
		SceneObjectSet::iterator iter;
		for (iter=m_SceneObjects.begin(); iter!=m_SceneObjects.end(); iter++)
		{
			(*iter)->RayPickingSceneObject(ray, sceneObjects, type, collisionGroup);
		}
	}

	// Push a sphere thoughout all the scene objects and get a valid position without collision
	bool SceneGraph::PushSphere(const Vector3f& pos, Vector3f& newpos, float radius)
	{
		bool result = false;

		Vector3f tmpPos = pos;

		SceneObjectSet::iterator iter;
		for (iter=m_SceneObjects.begin(); iter!=m_SceneObjects.end(); iter++)
		{
			if ((*iter)->PushSphere(tmpPos, newpos, radius))
			{
				result |= true;
				tmpPos = newpos;
			}
		}

		return result;
	}

	void SceneGraph::DumpToLog()
	{
		Log.MsgLn("----------- Begin dumping SceneGraph");
		SceneObjectSet::iterator iter;
		for (iter=m_SceneObjects.begin();
			iter!=m_SceneObjects.end();
			iter++)
		{
			(*iter)->DumpToLogRecursively(0);
		}
		Log.MsgLn("----------- End dumping SceneGraph");
	}

	void SceneGraph::RemoveSceneObjectInternal(SceneObject* object, bool deleteObj, bool recursive)
	{
		//if (m_RootObject->DetachChildObject(object))
		//{

		// TODO: 不调用递归删除有子对象的对象时候会崩溃

		if (object->GetParent())
			object->DetachFromParent(false);

		if (recursive)
		{
			object->DeleteAllChildren();
		}

		SceneObjectSet::iterator soiter = m_SceneObjects.find(object);
		if (soiter!=m_SceneObjects.end())
		{
			m_SceneObjects.erase(soiter);
		}

		// 从自动删除列表中删除对象
		SceneObjectSet::iterator iter = m_AutoDeleteList.find(object);
		if (iter!=m_AutoDeleteList.end())
		{
			m_AutoDeleteList.erase(iter);
		}

		if (deleteObj)
		{
			object->Destroy();

			// TODO: 这里递归设定场景图会导致崩溃，为何？
			//object->SetSceneGraphRecursively(NULL);
			object->SetSceneGraph(NULL);
			delete object;
		}

		//}
	}

	void SceneGraph::ProcessRemove()
	{
		vector<RemoveListElement>::iterator remove_iter;
		for (remove_iter=m_RemoveList.begin(); remove_iter!=m_RemoveList.end(); remove_iter++)
		{
			RemoveSceneObjectInternal(remove_iter->obj, remove_iter->deleteObj, remove_iter->recursive);
		}

		m_RemoveList.clear();
	}
}
