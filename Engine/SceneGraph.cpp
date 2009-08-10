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
  m_UIRootObject(NULL),
  m_Camera(NULL),
  m_Ambient(0.2f, 0.2f, 0.2f, 0.1f)
{
	// 创建用于管理其他对象的根对象
	m_RootObject = new SceneRootObject();
	m_UIRootObject = new UIRootObject();
}

SceneGraph::~SceneGraph()
{
	set<BaseSceneObject*>::iterator iter;
	for (iter=m_AutoDeleteList.begin(); iter!=m_AutoDeleteList.end(); iter++)
	{
		delete (*iter);
	}

	SAFE_DELETE(m_UIRootObject)
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

	// TODO: 在Update过程中调用这个方法会导致潜在错误

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

void SceneGraph::AddUIObject(BaseUIObject* object)
{
	m_UIRootObject->AttachChildObject(object);
}

void SceneGraph::SetCamera(Camera* camera, bool useCameraFrustum)
{
	m_Camera = camera; 

	//NotifyUpdatingProjectionMatrix();

	// This hacks: 测试一下frustum的工作情况
	//renderer->ProjectionMatrix() = frustum.BuildPrespectiveProjMatrix();
	if (useCameraFrustum)
	{
		camera->UpdateFrustum();
		SetFrustum(camera->GetFrustum());
	}
}
//
//void SceneGraph::NotifyUpdatingProjectionMatrix()
//{
//	renderer->ProjectionMatrix() = m_Camera->GetProjMatrix();
//}

//-----------------------------------------------------------------------------------
/// \brief
/// 指定平头视截体
/// 
/// \param frustum
/// 视截体
/// 
/// \remarks
/// 视截体用于裁剪渲染对象
//-----------------------------------------------------------------------------------
void SceneGraph::SetFrustum(Frustum* frustum)
{
	//m_Frustum = frustum;

	//renderer->ProjectionMatrix() = frustum->BuildPrespectiveProjMatrix();
	renderer->SetFrustum(frustum);
}

void SceneGraph::UpdateScene(unsigned long deltaTime)
{
	// 通过根对象更新每个子对象
	m_RootObject->Update(deltaTime);
	m_UIRootObject->Update(deltaTime);
}

void SceneGraph::RenderScene()
{

	if (m_Camera)
	{
		renderer->ViewMatrix() = m_Camera->GetViewMatrix();
		renderer->ProjectionMatrix() = m_Camera->GetProjMatrix();
	}
	else
	{
		renderer->ViewMatrix().Identity();
		renderer->ProjectionMatrix().Identity();
	}

	//ShadowManager::Instance().RenderLightViewScene(m_RootObject);

	DrawScene();
	DrawUI();

	renderer->SwapBuffer();
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

void SceneGraph::DrawLine(const Vector3f& start, const Vector3f& end, const Color4f& color)
{
	ElementLine elem;
	elem.v1 = start;
	elem.v2 = end;
	elem.color = color;
	m_LineElements.push_back(elem);
}

void SceneGraph::DrawAssistantElements()
{
	vector<ElementLine>::iterator iter;
	for (iter=m_LineElements.begin(); iter!=m_LineElements.end(); iter++)
	{
		renderer->RenderLine(iter->v1, iter->v2, iter->color);
	}
}

void SceneGraph::DrawScene()
{
	// 渲染场景
	m_RootObject->CollectRenderObjects();
	renderer->SetProjectionMode(PROJECTION_MODE_PERSPECTIVE);
	//renderer->ToggleLighting(true);
	renderer->ClearBuffer();
	//renderer->ToggleDepthTest(true);
	renderer->BeginRender();
	m_RootObject->Render();
	//renderer->ClearBuffer(false);
	DrawAssistantElements();
	renderer->EndRender();

	m_LineElements.clear();

}

void SceneGraph::DrawUI()
{
	// 渲染屏幕界面
	renderer->SetProjectionMode(PROJECTION_MODE_ORTHO);
	renderer->ToggleLighting(false);
	renderer->ViewMatrix().Identity();
	renderer->ClearBuffer(false);
	renderer->ToggleDepthTest(false);
	renderer->BeginRender();
	m_UIRootObject->Render();

	renderer->EndRender();

}

