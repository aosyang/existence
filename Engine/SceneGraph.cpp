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
#include "ShadowManager.h"

SceneGraph::SceneGraph()
: m_RootObject(NULL),
  m_ProcessingUpdate(false),
  //m_Camera(NULL),
  m_Ambient(0.2f, 0.2f, 0.2f, 0.1f)
{
	// 创建用于管理其他对象的根对象
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
	// TODO: 这个方法删除有子对象的对象时候会怎样？

	// 当在Update循环内部执行删除操作的时候，添加到删除列表并稍后销毁
	if (m_ProcessingUpdate)
	{
		RemoveListElement elem;
		elem.obj = object;
		elem.deleteObj = deleteObj;
		m_RemoveList.push_back(elem);
	}
	else	// 否则直接删除对象
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
//	// This hacks: 测试一下frustum的工作情况
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
	// 通过根对象更新每个子对象
	m_RootObject->Update(deltaTime);

	m_ProcessingUpdate = false;

	ProcessRemove();
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
/// 设置环境光颜色
/// 
/// \param color
/// 颜色值
/// 
/// 环境光颜色会影响没有直接光照的多边形
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
		// 从自动删除列表中删除对象
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
