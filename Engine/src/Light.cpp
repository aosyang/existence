//-----------------------------------------------------------------------------------
/// Light.cpp 灯光，可以照亮场景中的各种对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Light.h"
#include "LightingManager.h"
#include "Engine.h"

namespace Gen
{
	Light::Light()
		: m_Ambient(0.0f, 0.0f, 0.0f, 1.0f),
		m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
		m_Specular(1.0f, 1.0f, 1.0f, 1.0f),
		m_Type(LIGHT_TYPE_POINT),
		m_Direction(0.0f, 0.0f, -1.0f),
		m_ConstantAttenuation(1.0f),
		m_LinearAttenuation(0.0f),
		m_QuadraticAttenuation(0.0f),
		m_Range(10.0f)
	{
	}

	Light::~Light()
	{
	}

	//void Light::DebugRender()
	//{
	//	BaseSceneObject::DebugRender();
	//	//renderer->RenderSphere(m_WorldTransform.GetPosition(), m_BoundingSphereRadius);
	//
	//	switch (m_Type)
	//	{
	//	case LIGHT_TYPE_POINT:
	//	case LIGHT_TYPE_SPOT:
	//		// 光照范围
	//		renderer->RenderSphere(m_WorldTransform.GetPosition(), m_Range, Color4f(1.0f, 1.0f, 0.0f));
	//		break;
	//
	//	case LIGHT_TYPE_DIRECTIONAL:
	//		renderer->RenderLine(m_Direction * 10, Vector3f(0.0f, 0.0f, 0.0f));
	//		break;
	//	}
	//}

	void Light::Destroy()
	{
		LightingManager::Instance().RemoveLight(this);
	}

	const Vector3f Light::GetPosition() const
	{
		return m_WorldTransform.GetPosition();
	}

	float Light::GetAttenuationFactor(float d)
	{
		return 1.0f / (m_ConstantAttenuation + m_LinearAttenuation * d + m_QuadraticAttenuation * d * d);
	}
}
