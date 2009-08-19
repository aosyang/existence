//-----------------------------------------------------------------------------------
/// MeshObject.cpp 网格对象，可以将网格资源渲染到屏幕
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "MeshObject.h"
#include "Engine.h"

//Vector3f mesh_point, mesh_normal;

MeshObject::MeshObject()
: BaseLightableObject(),
  m_Mesh(NULL),
  m_SubMeshCount(0),
  m_Materials(NULL),
  m_FrustumCulling(true)
{
	//mesh_point = mesh_normal = Vector3f(0.0f, 0.0f, 0.0f);
}

MeshObject::~MeshObject()
{
	SAFE_DELETE_ARRAY(m_Materials);
}

void MeshObject::Update(unsigned long deltaTime)
{
	BaseSceneObject::Update(deltaTime);
}

void MeshObject::Render()
{
	if (m_Mesh)
	{
		BaseLightableObject::SetupLights();

		for (int i=0; i<m_Mesh->GetElementsNum(); i++)
		{
			MeshElement* elem = m_Mesh->GetElement(i);
			renderer->RenderVertexBuffer(elem->m_VertexBuffer,
										 m_Materials[i],
										 m_WorldTransform);
		}
	}

	BaseSceneObject::Render();
}

void MeshObject::DebugRender()
{
	BaseSceneObject::DebugRender();

	//renderer->RenderSphere(m_WorldTransform.GetPosition(), m_BoundingSphereRadius);
	//renderer->RenderLine(mesh_point, mesh_point + mesh_normal, Color4f(0.0f, 1.0f, 0.0f));
}

bool MeshObject::IntersectsRay(const Ray& ray, CollisionInfo& info)
{
	Vector3f p, n;
	float d;

	bool result = RayPicking(ray, p, n, d);
	if (result)
	{
		info.point = p;
		info.normal = n;
		info.squaredDistance = (p - ray.origin).SquaredLength();
		info.obj = this;

		//mesh_point = p;
		//mesh_normal = n;
	}

	return result;
}



void MeshObject::SetMesh(Mesh* mesh)
{
	AssertFatal(mesh, "MeshObject::SetMesh() : Mesh cannot be null!");
	m_Mesh = mesh;

	SAFE_DELETE_ARRAY(m_Materials);

	m_SubMeshCount = m_Mesh->GetElementsNum();

	typedef Material* MaterialPtr;
	m_Materials = new MaterialPtr[m_SubMeshCount];

	for (int i=0; i<m_SubMeshCount; i++)
		m_Materials[i] = m_Mesh->GetMaterial(i);

	// 根据mesh的包围球半径更新对象包围球半径
	m_BoundingSphereRadius = m_Mesh->GetBoundingRadius();

	m_OBB.Reset();
	m_OBB.Expand(m_Mesh->m_OBB);
}

bool MeshObject::RayPicking(const Ray& ray, Vector3f& point, Vector3f& normal, float& d, bool infiniteLength)
{
	bool result = false;

	Vector3f p;
	float dist;
	result = ray.IntersectsBox(m_AABB.worldMin, m_AABB.worldMax, p, dist, infiniteLength);
	if (result)
	{
		Ray localRay = m_WorldTransform.GetInverseMatrix() * ray;
		result = localRay.IntersectsBox(m_OBB.localMin, m_OBB.localMax, p, dist, infiniteLength);
	}

	if (result)
	{
		point = p;
		d = dist;
	}

	// TODO : Mesh的射线检测是否需要精确到多边形？
	//if (!m_Mesh->m_FaceNum)
	//	return false;

	//Ray localRay;
	//Matrix4 invWorld = m_WorldTransform.GetInverseMatrix();
	//Matrix3 invRot = invWorld.GetRotationMatrix();
	//localRay.origin = invWorld * ray.origin;
	//localRay.direction = invRot * ray.direction;
	//localRay.distance = ray.distance;

	////Vector3f point;
	//float minDistance = 9999990.0f;

	//unsigned int index[3];

	//for (int i=0; i<m_Mesh->m_FaceNum; i++)
	//{
	//	index[0] = m_Mesh->m_FaceArray[i * 3];
	//	index[1] = m_Mesh->m_FaceArray[i * 3 + 1];
	//	index[2] = m_Mesh->m_FaceArray[i * 3 + 2];

	//	Vector3f v1 = Vector3f(&m_Mesh->m_VertexArray[index[0] * 3]);
	//	Vector3f v2 = Vector3f(&m_Mesh->m_VertexArray[index[1] * 3]);
	//	Vector3f v3 = Vector3f(&m_Mesh->m_VertexArray[index[2] * 3]);

	//	//v1 = m_WorldTransform * v1;
	//	//v2 = m_WorldTransform * v2;
	//	//v3 = m_WorldTransform * v3;

	//	// 交点
	//	Vector3f p;

	//	// 交点离开射线起点的距离
	//	float dist;

	//	if (localRay.IntersectsTriangle(v1, v2, v3, p, dist, infiniteLength) && dist<minDistance)
	//	{
	//		point = p;
	//		//normal = n;
	//		normal = CrossProduct(v2 - v1, v3 - v1);
	//		minDistance = dist;

	//		result = true;
	//	}
	//}

	//if (result)
	//{
	//	d = minDistance;
	//	point = m_WorldTransform * point;
	//	normal = m_WorldTransform.GetRotationMatrix() * normal;
	//	normal.normalize();
	//}

	return result;
}

bool MeshObject::IsCulled(const RenderView& view)
{
	if (m_FrustumCulling) return BaseSceneObject::IsCulled(view);

	return false;
}
