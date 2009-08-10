//-----------------------------------------------------------------------------------
/// Billboard.cpp 公告板对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Billboard.h"
#include "Engine.h"

Billboard::Billboard()
: BaseLightableObject(),
  m_Material(NULL),
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
	m_VertexBuffer = renderer->BuildVertexBuffer();

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

	m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Normal|VFormat_Color|VFormat_Texcoord0, vert, normal, color, uv, 4, face, 2);
}

Billboard::~Billboard()
{
	SAFE_DELETE(m_VertexBuffer)
}

void Billboard::Update(unsigned long deltaTime)
{
	UpdateVertexData();

	BaseSceneObject::Update(deltaTime);

	//float n = renderer->GetFrustum()->SphereInFrustum(m_WorldTransform.GetPosition(), 1.0f) / 10.0f;
	//SetColor(Color4f(1.0f, 1.0f, 1.0f, max(n, 0.0f)));
}

void Billboard::Render()
{
	Matrix4 invViewMatrix = renderer->ViewMatrix().GetInverseMatrix();
	Matrix4 billboardingMat = m_WorldTransform;
	//Matrix3 identityRotMatrix;
	//identityRotMatrix.Identity();
	billboardingMat.SetRotation(invViewMatrix.GetRotationMatrix());

	renderer->RenderVertexBuffer(m_VertexBuffer, m_Material, billboardingMat);
	BaseSceneObject::Render();

}

void Billboard::PrepareRenderObjects(SceneObjectList& objects)
{
	// TODO: Frustum checking
	BaseSceneObject::PrepareRenderObjects(objects);
	//objects.insert(this);
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

