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
: m_Material(NULL),
  m_Radius(1.0f),
  BaseLightableObject()
{
	m_VertexBuffer = renderer->BuildVertexBuffer();

	unsigned int face[] = { 0, 2, 1, 2, 3, 1 };
	//memcpy(m_FaceArray, face, sizeof(unsigned int) * 6);

	float vert[] = { -0.5f, 0.5f, 0.0f,
					 0.5f, 0.5f, 0.0f,
					 -0.5f, -0.5f, 0.0f,
					 0.5f, -0.5f, 0.0f };
	//memcpy(m_VertexArray, vert, sizeof(float) * 12);

	float normal[] = { 0.0f, 0.0f, 1.0f,
					   0.0f, 0.0f, 1.0f,
					   0.0f, 0.0f, 1.0f,
					   0.0f, 0.0f, 1.0f };
	//memcpy(m_NormalArray, normal, sizeof(float) * 12);

	float uv[] = { 0.0f, 1.0f,
				   1.0f, 1.0f,
				   0.0f, 0.0f,
				   1.0f, 0.0f };
	//memcpy(m_TexCoordArray, uv, sizeof(float) * 8);

	m_VertexBuffer->CreateBuffer(vert, normal, uv, 4, face, 2);
}

Billboard::~Billboard()
{
	SAFE_DELETE(m_VertexBuffer)
}

void Billboard::Render()
{
	Matrix4 invViewMatrix = renderer->ViewMatrix().GetInverseMatrix();
	Matrix4 billboardingMat = m_WorldTransform;
	Matrix3 identityRotMatrix;
	identityRotMatrix.Identity();
	billboardingMat.SetRotation(invViewMatrix.GetRotationMatrix());

	renderer->RenderVertexBuffer(m_VertexBuffer, m_Material, billboardingMat);
	BaseSceneObject::Render();

}

void Billboard::PrepareRenderObjects(ChildrenSceneObjectsSet& objects)
{
	// TODO: Frustum checking
	BaseSceneObject::PrepareRenderObjects(objects);
	//objects.insert(this);
}
