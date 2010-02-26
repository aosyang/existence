//-----------------------------------------------------------------------------------
/// DistantViewObject.cpp Զ������ʵ�ִ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#include "DistantViewObject.h"
#include "Engine.h"
#include "MeshElement.h"
#include "Renderer.h"

namespace Gen
{
	DistantViewObject::DistantViewObject(SceneGraph* scene)
	: MeshObject(scene),
	  m_OffsetScale(0.0f, 0.0f, 0.0f)
	{
		// render very first
		m_RenderOrder = 70;
	}

	DistantViewObject::~DistantViewObject()
	{
	}

	void DistantViewObject::RenderSingleObject()
	{
		RenderableObjectBase::RenderSingleObject();

		// ����Ⱦʱʹ�õı任�������hack
		Matrix4 invViewMatrix = Renderer::Instance().GetViewMatrix().GetInverseMatrix();
		Matrix4 distviewMatrix = m_WorldTransform;

		Vector3f offset = invViewMatrix.GetPosition();
		offset.x *= m_OffsetScale.x;
		offset.y *= m_OffsetScale.y;
		offset.z *= m_OffsetScale.z;

		distviewMatrix.SetPosition(invViewMatrix.GetPosition() - offset);

		for (int i=0; i<m_Mesh->GetElementCount(); i++)
		{
			MeshElement* elem = m_Mesh->GetElement(i);
			Renderer::Instance().SetupMaterial(m_Mesh->GetMaterial(i));
			Renderer::Instance().RenderPrimitives(m_Mesh->GetVertexBuffer(), elem->GetIndexBuffer(), distviewMatrix);
		}
		Renderer::Instance().ClearBuffer(false);
	}

	bool DistantViewObject::IsCulled(Frustum* frustum)
	{
		return false;
	}
}
