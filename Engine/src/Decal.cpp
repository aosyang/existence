//-----------------------------------------------------------------------------------
/// Decal.cpp 贴花
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Decal.h"
#include "Engine.h"
#include "Renderer.h"

namespace Gen
{
	Decal::Decal(SceneGraph* scene)
	: BaseClass(scene),
	  m_Material(NULL)
	{
		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
		m_IndexBuffer = Renderer::Instance().BuildIndexBuffer();
		SetSize(1.0f);
	}

	Decal::~Decal()
	{
		SAFE_DELETE(m_VertexBuffer);
		SAFE_DELETE(m_IndexBuffer);
	}


	void Decal::RenderSingleObject()
	{
		BaseClass::RenderSingleObject();

		RenderCommand cmd;
		cmd.indexBuffer = m_IndexBuffer;
		cmd.vertexBuffer = m_VertexBuffer;
		cmd.primType = PRIM_TRIANGLES;
		cmd.transform = m_WorldTransform;
		cmd.material = m_Material;
		cmd.renderOrder = m_RenderOrder;
		cmd.sceneObj = this;

		Renderer::Instance().SubmitRenderCommand(cmd);

		//Renderer::Instance().SetupMaterial(m_Material);
		//Renderer::Instance().RenderPrimitives(m_VertexBuffer, m_IndexBuffer, m_WorldTransform);
	}
	
	void Decal::SetMaterial(Material* mat)
	{
		if (mat) mat->Trigger();
		m_Material = mat;
	}

	void Decal::SetDirection(const Vector3f normal)
	{
		// TODO: SetLookAt的方法会造成多边形发生形变
		Matrix4 matLocal = this->m_Transform;

		// this hacks: 当法线方向正好与y方向一致或者相反时，我们必须换一个up vector，否则会出问题
		if (normal==Vector3f(0.0f, 1.0f, 0.0f) || normal==Vector3f(0.0f, -1.0f, 0.0f))
		{
			matLocal.SetLookAt(-normal, Vector3f(-1.0f, 0.0f, 0.0f));
		}
		else
		{
			matLocal.SetLookAt(-normal, Vector3f(0.0f, 1.0f, 0.0f));
		}

		m_Transform = matLocal;
	}

	void Decal::SetSize(float size)
	{
		m_Size = size;

		// 更新顶点
		BuildVertexArray();
	}

	void Decal::BuildVertexArray()
	{
		static unsigned int face[] = { 0, 2, 1, 2, 3, 1 };
		//memcpy(m_FaceArray, face, sizeof(unsigned int) * 6);

		float vert[] = { -0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f };

		for (int i=0; i<12; i++)
			vert[i] *= m_Size;

		//memcpy(m_VertexArray, vert, sizeof(float) * 12);

		static float normal[] = { 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f };
		//memcpy(m_NormalArray, normal, sizeof(float) * 12);

		static float uv[] = { 0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f };
		//memcpy(m_TexCoordArray, uv, sizeof(float) * 8);

		m_VertexBuffer->Clear();
		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Texcoord0, vert, normal, NULL, uv, 4);
		m_IndexBuffer->Clear();
		m_IndexBuffer->CreateBuffer(face, 2);

		//m_BoundingSphereRadius = m_Size * 0.5f;
		m_OBB.localMax = Vector3f(m_Size * 0.5f, m_Size * 0.5f, m_Size * 0.5f);
		m_OBB.localMin = Vector3f(-m_Size * 0.5f, -m_Size * 0.5f, -m_Size * 0.5f);
	}
}
