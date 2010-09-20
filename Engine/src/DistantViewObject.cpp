//-----------------------------------------------------------------------------------
/// DistantViewObject.cpp 远景对象实现代码
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
	: BaseClass(scene),
	  m_OffsetScale(0.0f, 0.0f, 0.0f),
	  m_Mesh(NULL)
	{
		// render very first
		m_RenderOrder = 70;
	}

	DistantViewObject::~DistantViewObject()
	{
	}

	void DistantViewObject::RenderSingleObject()
	{
		BaseClass::RenderSingleObject();

		// 将渲染时使用的变换矩阵进行hack
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

			RenderCommand cmd;
			cmd.indexBuffer = elem->GetIndexBuffer();
			cmd.vertexBuffer = m_Mesh->GetVertexBuffer();
			cmd.primType = PRIM_TRIANGLES;
			cmd.transform = distviewMatrix;
			cmd.material = m_Mesh->GetMaterial(i);
			cmd.renderOrder = m_RenderOrder;
			cmd.sceneObj = this;

			Renderer::Instance().SubmitRenderCommand(cmd);


			//Renderer::Instance().SetupMaterial(m_Mesh->GetMaterial(i));
			//Renderer::Instance().RenderPrimitives(m_Mesh->GetVertexBuffer(), elem->GetIndexBuffer(), distviewMatrix);
		}
		//Renderer::Instance().ClearBuffer(false);
	}

	bool DistantViewObject::IsCulled(Frustum* frustum)
	{
		// 远景对象将不会被剔除
		return false;
	}

	void DistantViewObject::SetMesh(BaseMesh* mesh)
	{
		AssertFatal(mesh, "DistantViewObject::SetMesh() : Mesh cannot be null!");
		m_Mesh = mesh;
		mesh->Trigger();

		// 将模型使用到的材质激活
		for (int i=0; i<mesh->GetElementCount(); i++)
		{
			BaseMaterial* m = mesh->GetMaterial(i);
			if (m) m->Trigger();
		}

		m_OBB.Reset();
		m_OBB.Expand(m_Mesh->GetOBB());
	}
}
