#include "GameCollisionTest.h"

GameCollisionTest::GameCollisionTest()
 : m_Scene(NULL),
   m_Camera(NULL),
   m_Sun(NULL),
   m_ApplyGravity(false),
   m_UIFps(NULL)
{
}

GameCollisionTest::~GameCollisionTest()
{
}

void GameCollisionTest::StartGame()
{
	m_Scene = new SceneGraph;

	m_Scene->SetAmbientColor(Color4f(0.7f, 0.7f, 0.7f));

	m_Camera = new Camera;
	m_Camera->SetNearClippingDistance(0.1f);
	m_Camera->SetPosition(Vector3f(0.0f, 50.0f, 0.0f));

	m_Sun = new Light();
	m_Sun->SetLightType(LIGHT_TYPE_DIRECTIONAL);
	Vector3f sunDir = Vector3f(0.2f, 1.0f, 0.5f);
	sunDir.normalize();
	m_Sun->SetDirection(sunDir);
	LightingManager::Instance().AddGlobalLight(m_Sun);
	//ShadowManager::Instance().SetLight(m_Sun);

	Material* sceneMat = ResourceManager<Material>::Instance().Create();
	m_SceneObject = new MeshObject();
	m_SceneObject->SetMesh(ResourceManager<Mesh>::Instance().GetByName("scene"));
	m_SceneObject->SetMaterial(sceneMat);
	m_Scene->AddObject(m_SceneObject);

	// 不添加到场景的摄像机将无法移动
	m_Scene->AddObject(m_Camera);

	m_Scene->SetCamera(m_Camera);

	LightingManager::Instance().AddLightableObject(m_SceneObject);

	Material* matDecal = ResourceManager<Material>::Instance().Create("MatDecal");
	matDecal->SetAlphaTest(true);
	matDecal->SetAlphaRef(0.01f);
	matDecal->SetTexture(renderer->GetTexture("smoke"));

	m_UIFps = new TextUIControl();
	//m_UIFps->SetFont("DefaultFont");
	m_Scene->AddUIObject(m_UIFps);
}

void GameCollisionTest::Shutdown()
{
	SAFE_DELETE(m_UIFps);
	SAFE_DELETE(m_SceneObject);
	SAFE_DELETE(m_Sun);
	SAFE_DELETE(m_Camera);

	delete m_Scene;
}

void GameCollisionTest::OnKeyPressed(unsigned int key)
{
	switch(key)
	{
	case KC_SPACE:
		m_ApplyGravity = !m_ApplyGravity;
		break;
	}
}
void GameCollisionTest::OnMousePressed(unsigned int id)
{
	switch(id)
	{
	case MB_Left:
		// apply decal on mouse hit point
		RenderWindowParam* param = System::Instance().GetRenderWindowParameters();

		unsigned int width = param->width;
		unsigned int height = param->height;

		float x = (float)Input::Instance().GetMouseAbsX() / width;
		float y = (float)Input::Instance().GetMouseAbsY() / height;

		Ray ray = m_Camera->GetCameratRay(x, y);

		ObjectsCollisionInfos colInfo;
		m_Scene->CollectRayPickingSceneObject(ray, colInfo, COLLISION_TYPE_MESH);
		if (colInfo.size())
		{
			CollisionInfo* nearest = &(*colInfo.begin());

			for (ObjectsCollisionInfos::iterator iter = colInfo.begin();
				iter != colInfo.end();
				iter++)
			{
				if ((*iter).squaredDistance < nearest->squaredDistance)
				{
					nearest = &(*iter);
				}

				// TODO: Auto remove decals from scene...
				Decal* decal = new Decal();
				decal->SetMaterial(ResourceManager<Material>::Instance().GetByName("MatDecal"));
				m_Scene->AddObject(decal);
				// 浮起一些距离，防止z-fighting
				decal->SetPosition(nearest->point + nearest->normal * 0.01f);
				decal->SetDirection(nearest->normal);

			}
		}
		break;
	}
}

bool GameCollisionTest::OnNotifyQuitting()
{
	return true;
}

void GameCollisionTest::OnResizeWindow(unsigned int width, unsigned int height)
{
	// 窗口缩放以后要更新摄像机的纵横比...
	float aspect = (float)width / height;

	// ...以及投影矩阵
	if (m_Camera)
	{
		m_Camera->SetAspect(aspect);
		renderer->ProjectionMatrix() = m_Camera->GetProjMatrix();
	}

}

void GameCollisionTest::Update(unsigned long deltaTime)
{
	float boost;
	char buf[1024] = "\0";

	// 左Shift键加速移动
	if (Input::Instance().GetKeyDown(KC_LSHIFT))
		boost = 4.0f;
	else
		boost = 1.0f;

	float forward = 0.0f;
	float right = 0.0f;

	Vector3f pos = m_Camera->WorldTransform().GetPosition();
	Vector3f moveVec(0.0f, 0.0f, 0.0f);
	Ray moveRay(pos, Vector3f(0.0f, 0.0f, 0.0f), 0.001f * deltaTime * boost);

	Vector3f forwardVec = m_Camera->WorldTransform().GetForwardVector();
	Vector3f rightVec = m_Camera->WorldTransform().GetRightVector();

	// W S A D控制摄像机移动
	if (Input::Instance().GetKeyDown(KC_W))
		//forward += 0.1f * deltaTime / 10.0f * boost;
		moveVec += forwardVec;
	if (Input::Instance().GetKeyDown(KC_S))
		//forward += -0.1f * deltaTime / 10.0f * boost;
		moveVec -= forwardVec;
	if (Input::Instance().GetKeyDown(KC_A))
		//right += -0.1f * deltaTime / 10.0f * boost;
		moveVec -= rightVec;
	if (Input::Instance().GetKeyDown(KC_D))
		//right += 0.1f * deltaTime / 10.0f * boost;
		moveVec += rightVec;

	//forward += -0.1f * deltaTime / 10.0f * boost * Input::Instance().GetJoyStickAbs(2) / 0x7FFF;
	//right += 0.1f * deltaTime / 10.0f * boost * Input::Instance().GetJoyStickAbs(3) / 0x7FFF;

	//m_Camera->MoveLocal(forward, right, 0.0f);
	//Vector3f camPos = m_Camera->WorldTransform().GetPosition();
	moveVec.normalize();
	moveRay.direction = moveVec;
	//if (moveVec!=Vector3f(0.0f, 0.0f, 0.0f))
		m_Camera->SetPosition(MoveAloneRay(moveRay));

	if (m_ApplyGravity)
	{
		Vector3f pos = m_Camera->WorldTransform().GetPosition();

		float fallingDist = 0.1f * deltaTime / 10.0f;
		static float stepHeight = 1.0f;

		Ray ray(pos, Vector3f(0.0f, -1.0f, 0.0f), fallingDist + 1.0f);
		ObjectsCollisionInfos colInfo;
		m_Scene->CollectRayPickingSceneObject(ray, colInfo, COLLISION_TYPE_MESH);
		if (colInfo.size())
		{
			CollisionInfo* nearest = &(*colInfo.begin());

			for (ObjectsCollisionInfos::iterator iter = colInfo.begin();
				iter != colInfo.end();
				iter++)
			{
				if ((*iter).squaredDistance < nearest->squaredDistance)
				{
					nearest = &(*iter);
				}
			}

			if (pos.y - nearest->point.y > fallingDist + stepHeight)
			{
				m_Camera->SetPosition(pos - Vector3f(0.0f, fallingDist, 0.0f));
			}
			else
			{
				m_Camera->SetPosition(nearest->point + Vector3f(0.0f, stepHeight, 0.0f));
			}
		}
		else
		{
			m_Camera->SetPosition(pos - Vector3f(0.0f, fallingDist, 0.0f));
		}
	}

	if (Input::Instance().GetKeyDown(KC_ESCAPE))
		Engine::Instance().SetQuitting(true);


	// 按住鼠标右键调整视角
	if (Input::Instance().GetMouseButtonDown(MB_Right))
	{
		float x = -(float)Input::Instance().GetMouseRelX() / 5.0f;
		float y = -(float)Input::Instance().GetMouseRelY() / 5.0f;

		m_Camera->RotateLocal(x, y);
	}

	// 使用手柄控制当前摄像机
	float x = -(float)Input::Instance().GetJoyStickAbs(1) / 0x7FFF * 0.1f * deltaTime;
	float y = -(float)Input::Instance().GetJoyStickAbs(0) / 0x7FFF * 0.1f * deltaTime;
	m_Camera->RotateLocal(x, y);

	m_Scene->UpdateScene(deltaTime);

	// 显示FPS
	unsigned int fps = Engine::Instance().GetFPS();
	sprintf(buf, "FPS: %d\nUse gravity: %s", fps, (m_ApplyGravity)?"true":"false");
	m_UIFps->SetText(buf);

	// 更新光照
	LightingManager::Instance().Update();
}

void GameCollisionTest::RenderScene()
{
	m_Scene->RenderScene();
}

Vector3f GameCollisionTest::MoveAloneRay(Ray& ray, int depth)
{
	//ObjectsCollisionInfos colInfo;
	//m_Scene->CollectRayPickingSceneObject(ray, colInfo, COLLISION_TYPE_MESH);
	//if (colInfo.size())
	//{
	//	CollisionInfo* nearest = &(*colInfo.begin());

	//	for (ObjectsCollisionInfos::iterator iter = colInfo.begin();
	//		iter != colInfo.end();
	//		iter++)
	//	{
	//		if ((*iter).squaredDistance < nearest->squaredDistance)
	//		{
	//			nearest = &(*iter);
	//		}
	//	}


	//	if (depth>=4)
	//		return nearest->point;

	//	Vector3f lineIn = /*ray.origin + */ray.direction;
	//	Vector3f n_normal = nearest->normal;
	//	n_normal.normalize();
	//	Vector3f rightVec = CrossProduct(lineIn, nearest->normal);
	//	rightVec.normalize();
	//	Vector3f moveAlone = CrossProduct(nearest->normal, rightVec);
	//	moveAlone.normalize();

	//	Ray new_ray(nearest->point + nearest->normal * 0.001f, moveAlone, ray.distance - sqrt(nearest->squaredDistance));
	//	//if (new_ray.distance < 0)
	//	//	int whatever = 0;
	//	return MoveAloneRay(new_ray, depth+1);
	//}
	//else	// 没有碰撞则返回射线的终点
		return (ray.origin + ray.direction * ray.distance);
}
