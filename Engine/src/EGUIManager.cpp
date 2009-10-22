//-----------------------------------------------------------------------------------
/// EGUIManager.cpp 图形界面管理器
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "EGUIManager.h"

EGUIManager::EGUIManager()
: m_UIRootObject(NULL)
{
}

void EGUIManager::Initialize()
{
	m_UIRootObject = new UIRootObject();
}

void EGUIManager::Shutdown()
{
	if (m_UIRootObject)
	{
		m_UIRootObject->RemoveAllChildObjects();
	}

	SAFE_DELETE(m_UIRootObject);
}

void EGUIManager::Update(unsigned long deltaTime)
{
	m_UIRootObject->Update(deltaTime);
}

void EGUIManager::RenderUI()
{
	// 设定视口为全屏
	renderer->SetViewport(0, 0, 0, 0);

	renderer->SetProjectionMode(PROJECTION_MODE_ORTHO);
	renderer->ToggleLighting(false);
	renderer->ViewMatrix().Identity();
	renderer->ClearBuffer(false);
	renderer->ToggleDepthTest(false);
	renderer->BeginRender();
	m_UIRootObject->Render();

	renderer->EndRender();
}


void EGUIManager::AddUIObject(BaseUIObject* object)
{
	m_UIRootObject->AttachChildObject(object);
}
