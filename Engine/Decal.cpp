//-----------------------------------------------------------------------------------
/// Decal.cpp ����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Decal.h"
#include "Engine.h"

Decal::Decal()
: m_Material(NULL),
  BaseLightableObject()
{
	m_VertexBuffer = renderer->BuildVertexBuffer();
	SetSize(1.0f);
}

Decal::~Decal()
{
	SAFE_DELETE(m_VertexBuffer)
}


void Decal::Render()
{
	renderer->RenderVertexBuffer(m_VertexBuffer, m_Material, m_WorldTransform);
	BaseSceneObject::Render();
}

void Decal::PrepareRenderObjects(ChildrenSceneObjectsSet& objects)
{
	// Frustum checking
	Frustum* frustum = renderer->GetFrustum();

	// ���ӽ���-��Χ�򡱲ü�
	if (!frustum || frustum->SphereInFrustum(m_WorldTransform.GetPosition(), m_BoundingSphereRadius) > 0.0f)
	{
		BaseSceneObject::PrepareRenderObjects(objects);
		//objects.insert(this);
	}
}

bool Decal::IntersectsRay(const Ray& ray, CollisionInfo& info, CollisionType type)
{
	// Do not collide with decal,  for now...
	return false;
}

void Decal::SetDirection(const Vector3f normal)
{
	// TODO: SetLookAt�ķ�������ɶ���η����α�
	Matrix4 matLocal = this->m_Transform;

	// this hacks: �����߷���������y����һ�»����෴ʱ�����Ǳ��뻻һ��up vector������������
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

	// ���¶���
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
	m_VertexBuffer->CreateBuffer(vert, normal, uv, 4, face, 2);

	m_BoundingSphereRadius = m_Size * 0.5f;
	m_OBB.localMax = Vector3f(m_Size * 0.5f, m_Size * 0.5f, m_Size * 0.5f);
	m_OBB.localMin = Vector3f(-m_Size * 0.5f, -m_Size * 0.5f, -m_Size * 0.5f);
}

