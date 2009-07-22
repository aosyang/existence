#include "BspObject.h"
#include "Engine.h"

BspObject::BspObject()
: BaseSceneObject(),
  m_Mesh(NULL),
  m_Bsp(NULL)
{
	
}

BspObject::~BspObject()
{
	SAFE_DELETE(m_Bsp)
}

void BspObject::Render()
{
	// BspObjects don't use dynamic lights... for now.
	int maxLightNum = renderer->GetMaxLightsNumber();

	for (int i=0; i<maxLightNum; i++)
	{
		renderer->SetupLight(i, NULL);
	}

	for (int i=0; i<m_Mesh->GetElementsNum(); i++)
	{
		MeshElement* elem = m_Mesh->GetElement(i);

		renderer->RenderVertexBuffer(elem->m_VertexBuffer, m_Mesh->GetMaterial(i), m_WorldTransform);
	}

	BaseSceneObject::Render();
}

void BspObject::DebugRender()
{
	BaseSceneObject::DebugRender();
}


void BspObject::PrepareRenderObjects(ChildrenSceneObjectsSet& objects)
{
	// TODO: 如果在可见范围，就添加到渲染队列
	Frustum* frustum = renderer->GetFrustum();

	// “视截体-包围球”裁剪
	if (!frustum || frustum->SphereInFrustum(m_WorldTransform.GetPosition(), m_BoundingSphereRadius) > 0.0f)
	{
		BaseSceneObject::PrepareRenderObjects(objects);
		//objects.insert(this);
	}
}


bool BspObject::IntersectsRay(const Ray& ray, CollisionInfo& info)
{
	Vector3f localPoint;
	Ray localRay = m_WorldTransform.GetInverseMatrix() * ray;
	BspTriangle* triangle;

	bool result = m_Bsp->Intersects(localRay, &localPoint, &triangle);

	if (result)
	{
		info.point = m_WorldTransform * localPoint;
		info.normal = m_WorldTransform * triangle->normal;
		info.squaredDistance = (localRay.origin - localPoint).SquaredLength();
		info.obj = this;
	}

	return result;
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

bool BspObject::PushSphere(const Vector3f& pos, Vector3f& newpos, float radius) const
{
	// TODO: 将pos根据世界坐标做逆变换
	Vector3f localPos = m_WorldTransform.GetInverseMatrix() * pos;
	Vector3f localNewpos;

	bool result = m_Bsp->PushSphere(localPos, localNewpos, radius);

	if (result)
	{
		newpos = m_WorldTransform * localNewpos;
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

