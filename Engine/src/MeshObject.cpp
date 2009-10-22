//-----------------------------------------------------------------------------------
/// MeshObject.cpp 网格对象，可以将网格资源渲染到屏幕
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "MeshObject.h"
#include "Engine.h"

MeshObject::MeshObject()
: m_Mesh(NULL),
  m_SubMeshCount(0),
  m_Materials(NULL)
  //m_FrustumCulling(true)
{
}

MeshObject::~MeshObject()
{
	SAFE_DELETE_ARRAY(m_Materials);
}

void MeshObject::RenderSingleObject()
{
	RenderableObjectBase::RenderSingleObject();

	if (m_Mesh)
	{
		// TODO: 以后使用这种方式渲染
		// m_Mesh->RenderMesh(&m_WorldTransform);

		for (int i=0; i<m_Mesh->GetElementsNum(); i++)
		{
			MeshElement* elem = m_Mesh->GetElement(i);
			renderer->RenderVertexBuffer(elem->m_VertexBuffer,
										 m_Materials[i],
										 m_WorldTransform);
		}
	}
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

//-----------------------------------------------------------------------------------
/// \brief
/// 射线与模型相交判定
/// 
/// \param ray
/// 世界空间中的射线
/// 
/// \param point
/// 返回交点位置(世界空间)
/// 
/// \param normal
/// 返回交点法线方向
/// 
/// \param d
/// 返回交点距射线出发点的距离
/// 
/// \param infiniteLength
/// 射线是否无限延伸
/// 
/// \returns
/// 返回true表明有交点，否则返回false
//-----------------------------------------------------------------------------------
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
		point = m_WorldTransform * p;
		d = dist;
	}

	return result;
}
//
//bool MeshObject::RayPickingTriangle(const Ray& ray)
//{
//	bool result = false;
//
//	Vector3f p;
//	float dist;
//	result = ray.IntersectsBox(m_AABB.worldMin, m_AABB.worldMax, p, dist);
//	if (result)
//	{
//		Ray localRay = m_WorldTransform.GetInverseMatrix() * ray;
//		result = localRay.IntersectsBox(m_OBB.localMin, m_OBB.localMax, p, dist);
//	}
//
//	if (result)
//	{
//
//		if (!m_Mesh->m_FaceNum)
//			return false;
//
//		Ray localRay;
//		Matrix4 invWorld = m_WorldTransform.GetInverseMatrix();
//		Matrix3 invRot = invWorld.GetRotationMatrix();
//		localRay.origin = invWorld * ray.origin;
//		localRay.direction = invRot * ray.direction;
//		localRay.distance = ray.distance;
//
//		//Vector3f point;
//		float minDistance = 9999990.0f;
//
//		unsigned int index[3];
//
//		for (int i=0; i<m_Mesh->m_FaceNum; i++)
//		{
//			index[0] = m_Mesh->m_FaceArray[i * 3];
//			index[1] = m_Mesh->m_FaceArray[i * 3 + 1];
//			index[2] = m_Mesh->m_FaceArray[i * 3 + 2];
//
//			Vector3f v1 = Vector3f(&m_Mesh->m_VertexArray[index[0] * 3]);
//			Vector3f v2 = Vector3f(&m_Mesh->m_VertexArray[index[1] * 3]);
//			Vector3f v3 = Vector3f(&m_Mesh->m_VertexArray[index[2] * 3]);
//
//			//v1 = m_WorldTransform * v1;
//			//v2 = m_WorldTransform * v2;
//			//v3 = m_WorldTransform * v3;
//
//			// 交点
//			Vector3f p;
//
//			// 交点离开射线起点的距离
//			float dist;
//
//			if (localRay.IntersectsTriangle(v1, v2, v3, p, dist) && dist<minDistance)
//			{
//				point = p;
//				//normal = n;
//				normal = CrossProduct(v2 - v1, v3 - v1);
//				minDistance = dist;
//
//				result = true;
//			}
//		}
//
//		if (result)
//		{
//			d = minDistance;
//			point = m_WorldTransform * point;
//			normal = m_WorldTransform.GetRotationMatrix() * normal;
//			normal.normalize();
//		}
//	}
//
//}
