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
#include "ShadowManager.h"

SceneGraph::SceneGraph()
: m_RootObject(NULL),
  m_ProcessingUpdate(false),
  //m_Camera(NULL),
  m_Ambient(0.2f, 0.2f, 0.2f, 0.1f)
{
	// �������ڹ�����������ĸ�����
	m_RootObject = new SceneRootObject();
}

SceneGraph::~SceneGraph()
{
	set<BaseSceneObject*>::iterator iter;
	for (iter=m_AutoDeleteList.begin(); iter!=m_AutoDeleteList.end(); iter++)
	{
		delete (*iter);
	}

	SAFE_DELETE(m_RootObject)
}

void SceneGraph::AddObject(BaseSceneObject* object, bool autoDelete)
{
	//m_SceneObjects.push_back(object);

	m_RootObject->AttachChildObject(object);
	object->m_Scene = this;

	if (autoDelete)
	{
		m_AutoDeleteList.insert(object);
	}
}

void SceneGraph::RemoveObject(BaseSceneObject* object, bool deleteObj)
{
	// TODO: �������ɾ�����Ӷ���Ķ���ʱ���������

	// ����Updateѭ���ڲ�ִ��ɾ��������ʱ����ӵ�ɾ���б��Ժ�����
	if (m_ProcessingUpdate)
	{
		RemoveListElement elem;
		elem.obj = object;
		elem.deleteObj = deleteObj;
		m_RemoveList.push_back(elem);
	}
	else	// ����ֱ��ɾ������
	{
		RemoveObjectInternal(object, deleteObj);
	}
}
//
//void SceneGraph::SetCamera(Camera* camera, bool useCameraFrustum)
//{
//	m_Camera = camera; 
//
//	//NotifyUpdatingProjectionMatrix();
//
//	// This hacks: ����һ��frustum�Ĺ������
//	//renderer->ProjectionMatrix() = frustum.BuildPrespectiveProjMatrix();
//	if (useCameraFrustum)
//	{
//		camera->UpdateFrustum();
//		SetFrustum(camera->GetFrustum());
//	}
//}
//
//void SceneGraph::NotifyUpdatingProjectionMatrix()
//{
//	renderer->ProjectionMatrix() = m_Camera->GetProjMatrix();
//}

void SceneGraph::UpdateScene(unsigned long deltaTime)
{
	m_ProcessingUpdate = true;
	// ͨ�����������ÿ���Ӷ���
	m_RootObject->Update(deltaTime);

	m_ProcessingUpdate = false;

	ProcessRemove();
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
	m_RootObject->CollectRenderObjects(m_RenderView);
	renderer->SetProjectionMode(PROJECTION_MODE_PERSPECTIVE);
	//renderer->ToggleLighting(true);
	renderer->ClearBuffer();
	//renderer->ToggleDepthTest(true);
	renderer->BeginRender();
	m_RootObject->Render();
	//renderer->ClearBuffer(false);
	renderer->EndRender();

}

void SceneGraph::CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, int type, int collisionGroup)
{
	m_RootObject->CollectRayPickingSceneObject(ray, baseSceneObjects, type, collisionGroup);
}

//-----------------------------------------------------------------------------------
/// \brief
/// ���û�������ɫ
/// 
/// \param color
/// ��ɫֵ
/// 
/// ��������ɫ��Ӱ��û��ֱ�ӹ��յĶ����
//-----------------------------------------------------------------------------------
void SceneGraph::SetAmbientColor(const Color4f color)
{
	m_Ambient = color;
	renderer->SetAmbientColor(color);
}

void SceneGraph::RemoveObjectInternal(BaseSceneObject* object, bool deleteObj)
{
	if (m_RootObject->DetachChildObject(object))
	{
		// ���Զ�ɾ���б���ɾ������
		set<BaseSceneObject*>::iterator iter = m_AutoDeleteList.find(object);
		if (iter!=m_AutoDeleteList.end())
			m_AutoDeleteList.erase(iter);

		if (deleteObj)
			delete object;
	}
}

void SceneGraph::ProcessRemove()
{
	vector<RemoveListElement>::iterator remove_iter;
	for (remove_iter=m_RemoveList.begin(); remove_iter!=m_RemoveList.end(); remove_iter++)
	{
		RemoveObjectInternal(remove_iter->obj, remove_iter->deleteObj);
	}

	m_RemoveList.clear();
}
