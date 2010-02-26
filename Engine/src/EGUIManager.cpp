//-----------------------------------------------------------------------------------
/// EGUIManager.cpp 图形界面管理器
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "EGUIManager.h"
#include "Renderer.h"
#include "Material.h"
#include "System.h"

namespace Gen
{
	EGUIManager::EGUIManager()
		: m_UIRootObject(NULL)
	{
	}

	void EGUIManager::Initialize()
	{
		m_UIRootObject = new UIRootObject();

		m_UIMaterial = MaterialManager::Instance().Create("#uimaterial");
		m_UIMaterial->SetTextureLayerEnabled(0, false);
		m_UIMaterial->SetDepthTest(false);
		m_UIMaterial->SetDepthWriting(false);
		m_UIMaterial->SetLighting(false);
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
		unsigned int	w = System::Instance().GetRenderWindowParameters()->width,
						h = System::Instance().GetRenderWindowParameters()->height;

		float aspect = (float)w/(float)h;

		// UI用平行投影矩阵
		Matrix4 uiOrthoMat = Matrix4::BuildOrthographicProjection(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

		// 设定视口为全屏
		Renderer::Instance().SetViewport(0, 0, 0, 0);

		// 使用预设的UI材质
		Renderer::Instance().SetupMaterial(m_UIMaterial);
		Renderer::Instance().SetProjectionMatrix(uiOrthoMat);
		Renderer::Instance().SetViewMatrix(Matrix4::IDENTITY);
		Renderer::Instance().ClearBuffer(false);
		Renderer::Instance().BeginRender();
		m_UIRootObject->Render();

		Renderer::Instance().EndRender();
	}


	void EGUIManager::AddUIObject(BaseUIObject* object)
	{
		m_UIRootObject->AttachChildObject(object);
	}
}
