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

	// �ȶ���ײ�н����ж�
	if (RenderableObjectBase::IntersectsRay(ray, local_info))
	{
		// ������ߴ�solid�ռ�������϶�ֱ����ײ
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

		// ���о�ȷBSP��ײ
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

	// ����mesh�İ�Χ��뾶���¶����Χ��뾶
	m_BoundingSphereRadius = m_Mesh->GetBoundingRadius();

	SAFE_DELETE(m_Bsp)
	m_Bsp = new BspTree();
	BuildBspTreeFromMesh(m_Bsp, mesh);

	m_OBB.Reset();
	m_OBB.Expand(m_Mesh->GetOBB());
}

bool BspObject::PushSphere(const Vector3f& pos, Vector3f& newpos, float radius)
{
	// ������BSP��AABB���ཻ������
	if (!m_AABB.IntersectsSphere(pos, radius)) return false;

	// TODO: ��pos����������������任
	Vector3f localPos = m_WorldTransform.GetInverseMatrix() * pos;
	Vector3f localNewpos;

	bool result = m_Bsp->PushSphere(localPos, localNewpos, radius);

	// ��ʱ�洢�µ�λ��
	Vector3f tmpPos, tmpNewpos;

	// ����Ѿ�����λ�ã�ʹ�����λ�ý�����һ��push
	if (result)
		tmpPos = m_WorldTransform * localNewpos;
	else
		tmpPos = pos;


	// ���û��ຯ�������ݹ����
	if (RenderableObjectBase::PushSphere(tmpPos, tmpNewpos, radius))
	{
		result = true;
		newpos = tmpNewpos;
	}
	// ����Ӷ���û����ײ��������ʱ����λ����Ϊ����λ��
	else if (result)
	{
		newpos = tmpPos;
	}

	return result;

}

// TODO: ����޸�ʹ��bsp��worldtransform���
void BspObject::TraverseTree(
							 vector< BspTriangle* >* polyList,
							 const Vector3f& loc )
{
	m_Bsp->TraverseTree(polyList, loc);
}

// TODO: ����޸ĳɺ�����objectһ�µ����߼�ⷽ��
// ���ﷵ�ص���������Ϣ��bspԭʼ��Ϣ���ʶ��㼰��������λ��local�ռ�
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

