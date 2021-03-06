//-----------------------------------------------------------------------------------
/// Billboard.cpp 公告板对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Billboard.h"
#include "Engine.h"
#include "Renderer.h"

namespace Gen
{
	Billboard::Billboard(SceneGraph* scene)
	: BaseClass(scene),
	  m_Radius(10.0f),
	  m_ZRotataion(0.0f),
	  m_ZOffset(0.0f),
	  m_Scale(1.0f),
	  m_Scale_x(1.0f),
	  m_Scale_y(1.0f),
	  m_NeedUpdateVertexData(false)
	{
		m_RenderOrder = 110;

		// 创建顶点缓冲
		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
		m_IndexBuffer = Renderer::Instance().BuildIndexBuffer();

		// 指定顶点数据
		unsigned int face[] = { 0, 2, 1, 2, 3, 1 };

		float vert[] = { -0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f };

		float normal[] = { 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f };

		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f };

		float uv[] = { 0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f };

		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Color|VFormat_Texcoord0, vert, normal, color, uv, 4);
		m_IndexBuffer->CreateBuffer(face, 2);
	}

	Billboard::~Billboard()
	{
		SAFE_DELETE(m_VertexBuffer);
		SAFE_DELETE(m_IndexBuffer);
	}

	void Billboard::Update(unsigned long deltaTime)
	{
		UpdateVertexData();

		BaseClass::Update(deltaTime);

		//float n = Renderer::Instance().GetFrustum()->SphereInFrustum(m_WorldTransform.GetPosition(), 1.0f) / 10.0f;
		//SetColor(Color4f(1.0f, 1.0f, 1.0f, max(n, 0.0f)));
	}

	void Billboard::RenderSingleObject()
	{
		BaseClass::RenderSingleObject();

		Matrix4 invViewMatrix = Renderer::Instance().GetViewMatrix().GetInverseMatrix();
		Matrix4 billboardingMat = m_WorldTransform;
		//Matrix3 identityRotMatrix;
		//identityRotMatrix.MakeIdentity();
		billboardingMat.SetRotation(invViewMatrix.GetRotationMatrix());

		RenderCommand cmd;
		cmd.indexBuffer = m_IndexBuffer;
		cmd.vertexBuffer = m_VertexBuffer;
		cmd.primType = PRIM_TRIANGLES;
		cmd.transform = billboardingMat;
		cmd.material = m_Material;
		cmd.renderOrder = m_RenderOrder;
		cmd.sceneObj = this;

		Renderer::Instance().SubmitRenderCommand(cmd);

		//Renderer::Instance().SetupMaterial(m_Material);
		//Renderer::Instance().RenderPrimitives(m_VertexBuffer, m_IndexBuffer, billboardingMat);
	}

	void Billboard::SetMaterial(const String& matName)
	{
		BaseMaterial* mat = MaterialManager::Instance().GetByName(matName);
		SetMaterial(mat);
	}

	void Billboard::SetMaterial(BaseMaterial* mat)
	{
		m_Material = mat;
		if (mat) mat->Trigger();
	}

	void Billboard::SetColor(const Color4f& color)
	{
		float new_color[16];
		for (int i=0; i<4; i++)
			memcpy(&(new_color[i*4]), color.GetArray(), sizeof(float) * 4);

		m_VertexBuffer->ModifyVertexData(VFormat_Color, 0, sizeof(float) * 16, new_color);
	}

	void Billboard::SetZRotation(float rad_angle)
	{
		m_ZRotataion = rad_angle;
		m_NeedUpdateVertexData = true;

	}

	void Billboard::SetZOffset(float offset)
	{
		m_ZOffset = offset;
		m_NeedUpdateVertexData = true;
	}

	void Billboard::SetScale(float scale)
	{
		m_Scale = scale;
		m_NeedUpdateVertexData = true;
	}

	void Billboard::SetScreenSpaceScale(float scale_x, float scale_y)
	{
		m_Scale_x = scale_x;
		m_Scale_y = scale_y;
		m_NeedUpdateVertexData = true;
	}

	void Billboard::UpdateVertexData()
	{
		// 仅当顶点属性发生变化才更新这个
		if (!m_NeedUpdateVertexData) return;


		Vector3f points[4] = { Vector3f(-0.5f, 0.5f, 0.0f),
			Vector3f(0.5f, 0.5f, 0.0f),
			Vector3f(-0.5f, -0.5f, 0.0f),
			Vector3f(0.5f, -0.5f, 0.0f) };

		float new_pos[12];

		Matrix3 rotMat = Matrix3::BuildRollRotationMatrix(m_ZRotataion);
		for (int i=0; i<4; i++)
		{
			Vector3f point = rotMat * (points[i] * m_Scale);
			point.z = m_ZOffset;
			point.x *= m_Scale_x;
			point.y *= m_Scale_y;
			memcpy(&new_pos[i*3], point.GetArray(), sizeof(float) * 3);
		}

		m_VertexBuffer->ModifyVertexData(VFormat_Position, 0, sizeof(float) * 12, new_pos);
	}
}

