//-----------------------------------------------------------------------------------
/// ShadowManager.h ÒõÓ°¹ÜÀíÆ÷
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "ShadowManager.h"
#include "Engine.h"

namespace Gen
{
#define SHADOW_MAP_SIZE 512

	ShadowManager::ShadowManager()
		: m_Light(NULL)
	{
		ITexture* depthTexture = renderer->BuildDepthTexture("depth", SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
		//depthTexture->SetMinFilterType(FILTER_TYPE_NEAREST);
		//depthTexture->SetMagFilterType(FILTER_TYPE_NEAREST);
	}
	//
	//void ShadowManager::RenderLightViewScene(SceneRootObject* rootObj)
	//{
	//	if (!m_Light)
	//		return;
	//
	//	if (m_Light->GetLightType()!=LIGHT_TYPE_DIRECTIONAL)
	//		return;
	//
	//	Vector3f dir = m_Light->GetDirection();
	//	Matrix4 lightViewMatrix;
	//	lightViewMatrix.MakeIdentity();
	//	lightViewMatrix.SetLookAt(dir, Vector3f(0.0f, 1.0f, 0.0f));
	//	lightViewMatrix.SetPosition(Vector3f(1.0f, 1.0f, 1.0f));
	//
	//	renderer->ViewMatrix() = lightViewMatrix;
	//
	//}
	//
	//void ShadowManager::RenderUntexturedScene(SceneRootObject* rootObj)
	//{
	//
	//}
}
