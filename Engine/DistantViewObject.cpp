//-----------------------------------------------------------------------------------
/// DistantViewObject.cpp Զ��������Ⱦ������պС�Զ�����������Ķ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "DistantViewObject.h"
#include "Engine.h"

DistantViewObject::DistantViewObject()
: BaseSceneObject(),
  m_OffsetScale(0.0f, 0.0f, 0.0f)
{
	// render very first
	m_RenderOrder = 70;
}

DistantViewObject::~DistantViewObject()
{
}

void DistantViewObject::Update(unsigned long deltaTime)
{
	BaseSceneObject::Update(deltaTime);
}

void DistantViewObject::Render()
{
	// TODO: ����ؼ�������Ⱦʱʹ�õı任�������hack
	Matrix4 invViewMatrix = renderer->ViewMatrix().GetInverseMatrix();
	Matrix4 distviewMatrix = m_WorldTransform;
	
	Vector3f offset = invViewMatrix.GetPosition();
	offset.x *= m_OffsetScale.x;
	offset.y *= m_OffsetScale.y;
	offset.z *= m_OffsetScale.z;

	distviewMatrix.SetPosition(invViewMatrix.GetPosition() - offset);

	int maxLightNum = renderer->GetMaxLightsNumber();

	for (int i=0; i<maxLightNum; i++)
	{
		renderer->SetupLight(i, NULL);
	}


	for (int i=0; i<m_Mesh->GetElementsNum(); i++)
	{
		MeshElement* elem = m_Mesh->GetElement(i);
		renderer->RenderVertexBuffer(elem->m_VertexBuffer, m_Mesh->GetMaterial(i), distviewMatrix);
	}
	renderer->ClearBuffer(false);

	BaseSceneObject::Render();
}

void DistantViewObject::DebugRender()
{
	// nothing to do
}

void DistantViewObject::PrepareRenderObjects(ChildrenSceneObjectsSet& objects)
{
	// always render
	BaseSceneObject::PrepareRenderObjects(objects);
}

bool DistantViewObject::IntersectsRay(const Ray& ray, CollisionInfo& info, CollisionType type)
{
	// never get cought by a ray
	return false;
}

void DistantViewObject::SetMesh(Mesh* mesh)
{
	AssertFatal(mesh, "DistantViewObject::SetMesh() : Mesh cannot be null!");
	m_Mesh = mesh;

	//// NOTE: ���������ı�����ԭ�ȵ�����
	//for (int i=0; i<m_Mesh->GetMaterialsNum(); i++)
	//{
	//	Material* mat = m_Mesh->GetMaterial(i);
	//	if (!mat) continue;
	//	mat->SetDepthWriting(false);
	//	mat->SetLighting(false);

	//	// TODO: ĳЩ�Կ���֧���������
	//	mat->GetTextureRenderState()->wrapType = WRAP_TYPE_CLAMP_TO_EDGE;
	//	//mat->GetTextureRenderState()->magFilterType = FILTER_TYPE_NEAREST;
	//}
}

bool DistantViewObject::RayPicking(const Ray& ray, Vector3f& point, Vector3f& normal, float& d, bool infiniteLength)
{
	// forget it...
	return false;
}
