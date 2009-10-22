#include "BspObject.h"
#include "Engine.h"

BspObject::BspObject()
: m_Mesh(NULL),
  m_Bsp(NULL)
{
	
}

BspObject::~BspObject()
{
	SAFE_DELETE(m_Bsp)
}

void BspObject::RenderSingleObject()
{
	RenderableObjectBase::RenderSingleObject();

	for (int i=0; i<m_Mesh->GetElementsNum(); i++)
	{
		MeshElement* elem = m_Mesh->GetElement(i);

		renderer->RenderVertexBuffer(elem->m_VertexBuffer, m_Mesh->GetMaterial(i), m_WorldTransform);
	}
}

bool BspObject::IntersectsRay(const Ray& ray, CollisionResult& info)
{
	CollisionResult local_info;

	// 先对碰撞盒进行判断
	if (RenderableObjectBase::IntersectsRay(ray, local_info))
	{
		// 如果射线从solid空间射出，认定直接碰撞
		if (IsPointInSolid(ray.origin))
		{
			info.distance = 0.0f;
			info.normal = -ray.direction;
			info.obj = this;
			info.point = ray.origin;

			return true;
		}

		Vector3f point;
		BspTriangle* tri;

		// 进行精确BSP碰撞
		if (Intersects(ray, &point, &tri))
		{
			info.distance = (point - ray.origin).Length();
			info.normal = tri->normal;
			info.obj = this;
			info.point = point;

			return true;
		}
	}

	return false;
}


void BspObject::SetMesh(Mesh* mesh)
{
	AssertFatal(mesh, "BspObject::SetMesh() : Mesh cannot be null!");
	m_Mesh = mesh;

	// 根据mesh的包围球半径更新对象包围球半径
	m_BoundingSphereRadius = m_Mesh->GetBoundingRadius();

	SAFE_DELETE(m_Bsp)
	m_Bsp = new BspTree();
	BuildBspTreeFromMesh(m_Bsp, mesh);

	m_OBB.Reset();
	m_OBB.Expand(m_Mesh->GetOBB());
}

bool BspObject::PushSphere(const Vector3f& pos, Vector3f& newpos, float radius)
{
	// 如果球和BSP的AABB不相交，返回
	if (!m_AABB.IntersectsSphere(pos, radius)) return false;

	// TODO: 将pos根据世界坐标做逆变换
	Vector3f localPos = m_WorldTransform.GetInverseMatrix() * pos;
	Vector3f localNewpos;

	bool result = m_Bsp->PushSphere(localPos, localNewpos, radius);

	// 临时存储新的位置
	Vector3f tmpPos, tmpNewpos;

	// 如果已经有新位置，使用这个位置进行下一次push
	if (result)
		tmpPos = m_WorldTransform * localNewpos;
	else
		tmpPos = pos;


	// 调用基类函数，做递归调用
	if (RenderableObjectBase::PushSphere(tmpPos, tmpNewpos, radius))
	{
		result = true;
		newpos = tmpNewpos;
	}
	// 如果子对象都没有碰撞，则用临时的新位置作为最终位置
	else if (result)
	{
		newpos = tmpPos;
	}

	return result;

}

// TODO: 这个修改使得bsp与worldtransform相关
void BspObject::TraverseTree(
							 vector< BspTriangle* >* polyList,
							 const Vector3f& loc )
{
	m_Bsp->TraverseTree(polyList, loc);
}

// TODO: 这个修改成和其他object一致的射线检测方法
// 这里返回的三角形信息是bsp原始信息，故顶点及法线坐标位于local空间
bool BspObject::Intersects(const Ray& ray, Vector3f* point, BspTriangle** triangle)
{
	Vector3f localPoint;// = m_WorldTransform.GetInverseMatrix() * (*point);
	Ray localRay = m_WorldTransform.GetInverseMatrix() * ray;

	bool result = m_Bsp->Intersects(localRay, &localPoint, triangle);

	if (result)
	{
		*point = m_WorldTransform * localPoint;
	}
	
	return result;
}

bool BspObject::IsPointInSolid(const Vector3f& point)
{
	return m_Bsp->IsPointInSolid(m_WorldTransform.GetInverseMatrix() * point);
}

