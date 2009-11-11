//-----------------------------------------------------------------------------------
/// DistantViewObject.cpp 远景对象，渲染诸如天空盒、远景景物等物体的对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "DistantViewObject.h"
#include "Engine.h"

namespace Gen
{
	DistantViewObject::DistantViewObject()
		: m_OffsetScale(0.0f, 0.0f, 0.0f)
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

		// 将渲染时使用的变换矩阵进行hack
		Matrix4 invViewMatrix = renderer->ViewMatrix().GetInverseMatrix();
		Matrix4 distviewMatrix = m_WorldTransform;

		Vector3f offset = invViewMatrix.GetPosition();
		offset.x *= m_OffsetScale.x;
		offset.y *= m_OffsetScale.y;
		offset.z *= m_OffsetScale.z;

		distviewMatrix.SetPosition(invViewMatrix.GetPosition() - offset);

		for (int i=0; i<m_Mesh->GetElementCount(); i++)
		{
			MeshElement* elem = m_Mesh->GetElement(i);
			renderer->RenderVertexBuffer(elem->GetVertexBuffer(), m_Mesh->GetMaterial(i), distviewMatrix);
		}
		renderer->ClearBuffer(false);
	}

	bool DistantViewObject::IsCulled(Frustum* frustum)
	{
		return false;
	}

	void DistantViewObject::SetMesh(IMesh* mesh)
	{
		AssertFatal(mesh, "DistantViewObject::SetMesh() : Mesh cannot be null!");
		m_Mesh = mesh;
	}
}
