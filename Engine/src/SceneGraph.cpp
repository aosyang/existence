//-----------------------------------------------------------------------------------
/// SceneGraph.cpp ����ͼ��������ӵ������Ķ���
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
		// TODO: �Կ���Ⱦ���������Ҫ�Ӳ����Ƿ�͸�����뾵ͷ����ȷ������Ƚ�
		return lhs->GetRenderOrder() < rhs->GetRenderOrder();
	}

	SceneGraph::SceneGraph()
		: //m_RootObject(NULL),
	m_ProcessingUpdate(false)
	{
		// �������ڹ�����������ĸ�����
		//m_RootObject = new SceneRootObject();
	}

	SceneGraph::~SceneGraph()
	{
		SceneObjectSet::iterator iter;

		// ���ȵ���ÿ����Ҫ���ٵĶ����destroy����
		// ע����Ҫ�ڷ����б������ɾ�����󣬷������ɵ�����ʧЧ
		// TODO: Destroy������ɾ�������Ƿ����ɵ�����ʧЧ��

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
		// ����Updateѭ���ڲ�ִ��ɾ��������ʱ����ӵ�ɾ���б��Ժ�����
		if (m_ProcessingUpdate)
		{
			RemoveListElement elem;
			elem.obj = object;
			elem.deleteObj = deleteObj;
			elem.recursive = recursive;
			m_RemoveList.push_back(elem);
		}
		else	// ����ֱ��ɾ������
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
		// ͨ�����������ÿ���Ӷ���
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

	// ������Ⱦ�ӵ���Ϣ
	void SceneGraph::SetupRenderView(const RenderView& view)
	{
		m_RenderView = view;

		renderer->ViewMatrix() = view.viewMatrix;
		renderer->ProjectionMatrix() = view.projMatrix;
	}

	void SceneGraph::RenderScene()
	{
		//ShadowManager::Instance().RenderLightViewScene(m_RootObject);

		// ��Ⱦ����

		RenderableObjectList renderList;

		SceneObjectSet::iterator iter;
		for (iter=m_SceneObjects.begin(); iter!=m_SceneObjects.end(); iter++)
		{
			(*iter)->CollectRenderableObject(renderList, m_RenderView.frustum);
		}

		Engine::Instance().SetRenderBatchCount(renderList.size());

		// �����ж����������
		sort(renderList.begin(), renderList.end(), RenderableObjectBaseComparer);

		//m_RootObject->CollectRenderObjects(m_RenderView);
		renderer->SetProjectionMode(PROJECTION_MODE_PERSPECTIVE);
		renderer->ClearBuffer(
#if defined __PLATFORM_LINUX	// Ϊ����glx���������Ȼ���������
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

		// TODO: �����õݹ�ɾ�����Ӷ���Ķ���ʱ������

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

		// ���Զ�ɾ���б���ɾ������
		SceneObjectSet::iterator iter = m_AutoDeleteList.find(object);
		if (iter!=m_AutoDeleteList.end())
		{
			m_AutoDeleteList.erase(iter);
		}

		if (deleteObj)
		{
			object->Destroy();

			// TODO: ����ݹ��趨����ͼ�ᵼ�±�����Ϊ�Σ�
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
