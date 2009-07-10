#include "EmptyGame.h"

bool toggleCollision = false;
bool toggleWireframe = false;

EmptyGame::EmptyGame()
 : m_Scene(NULL),
   m_Camera1(NULL),
   m_Camera2(NULL),
   m_CurrentCamera(NULL),
   m_AudioListener(NULL),
   m_BoxMesh(NULL),
   m_Sun(NULL),
   m_Light(NULL),
   m_Angle(0.0f),
   m_UIControl(NULL),
   m_UIFps(NULL)
{
}

EmptyGame::~EmptyGame()
{
}

void EmptyGame::StartGame()
{
	m_Scene = new SceneGraph;

	m_Scene->SetAmbientColor(Color4f(0.7f, 0.7f, 0.7f));

	m_CurrentCamera = m_Camera1 = new Camera;
	m_Camera1->SetFarClippingDistance(1000.0f);
	//m_Camera1->SetPosition(Vector3f(1.0f, 0.0f, 5.0f));

	m_Camera2 = new Camera;
	m_Camera2->SetPosition(Vector3f(10.0f, 0.0f, 10.0f));

	m_AudioListener = new AudioListener();
	m_Camera1->AttachChildObject(m_AudioListener, false);

	m_Sun = new Light();
	m_Sun->SetLightType(LIGHT_TYPE_DIRECTIONAL);
	Vector3f sunDir = Vector3f(0.2f, 1.0f, 0.5f);
	sunDir.normalize();
	m_Sun->SetDirection(sunDir);
	LightingManager::Instance().AddGlobalLight(m_Sun);
	//ShadowManager::Instance().SetLight(m_Sun);

	// 不添加到场景的摄像机将无法移动
	m_Scene->AddObject(m_Camera1);
	m_Scene->AddObject(m_Camera2);
	m_Scene->SetCamera(m_CurrentCamera);

	// Billboard here...
	Material* matSmoke = ResourceManager<Material>::Instance().Create();
	matSmoke->SetTexture(renderer->GetTexture("smoke"));
	//matSmoke->SetLighting(false);
	m_Billboard = new Billboard();
	m_Billboard->SetMaterial(matSmoke);
	m_Scene->AddObject(m_Billboard);

	m_UIControl = new BaseUIObject();
	m_Scene->AddUIObject(m_UIControl);

	// 创建一个立方体Mesh
	m_BoxMesh = new Mesh();
	m_BoxMesh->CreateBox(1.0f);

	//m_CharacterMesh = new Mesh();
	//m_CharacterMesh->CreateBox(1.0f);
	//m_CharacterMesh->LoadFromFile("../../../data/scene.emd");
	//m_CharacterMesh->LoadFromFile("../../../data/marcus_bin.emd");
	//m_CharacterMesh->LoadFromFile("../../../data/marcus.emd");
	m_CharacterMesh = MeshManager::Instance().GetMesh("scene");

	//BuildBspTreeFromMesh(&m_BspScene, m_CharacterMesh);
	m_BspScene->SetMesh(m_CharacterMesh);
	m_Scene->AddObject(&m_BspScene);

	// 将三个Box添加到渲染列表中
	for (int i=0; i<2; i++)
	{
		m_Box[i].SetMesh(m_BoxMesh);
		
		m_Scene->AddObject(&m_Box[i]);
		LightingManager::Instance().AddLightableObject(&m_Box[i]);
	}

	m_CameraAttachedBox.SetMesh(MeshManager::Instance().GetMesh("mp5"));
	//m_CameraAttachedBox.SetPosition(Vector3f(0.0f, 0.0f, -5.0f));
	LightingManager::Instance().AddLightableObject(&m_CameraAttachedBox);

	// TODO: AddObject和AttachChildObject二者只需要一个？
	m_Scene->AddObject(&m_CameraAttachedBox);
	m_Camera1->AttachChildObject(&m_CameraAttachedBox, false);

	m_Box[2].SetMesh(m_CharacterMesh);
	m_Scene->AddObject(&m_Box[2]);
	LightingManager::Instance().AddLightableObject(&m_Box[2]);

	m_Light = new Light();
	m_Light->SetConstantAttenuation(0.0f);
	m_Light->SetLinearAttenuation(1.0f);
	m_Scene->AddObject(m_Light);
	m_Light->SetPosition(Vector3f(0.0f, 3.0f, 3.0f));

	LightingManager::Instance().AddLight(m_Light);
	LightingManager::Instance().AddLightableObject(&m_CameraAttachedBox);

	Material* matColorTest = ResourceManager<Material>::Instance().Create();
	//matColorTest->SetTexture(renderer->GetTexture("colortest"));
	//renderer->GetTexture("colortest")->SetUseEyeSpaceTexCoord(true);
	//renderer->GetTexture("no_material")->SetWrapType(WRAP_TYPE_REPEAT);
	// 为三个Box指定不同的材质
	m_Box[0].SetPosition(Vector3f(1.0f, 0.0f, 0.0f));
	Material* matRed = ResourceManager<Material>::Instance().Create();
	matRed->SetDiffuse(Color::RED);
	matRed->SetAmbient(Color::RED);
	ITexture* texCheck = renderer->GetTexture("check");
	matRed->SetTexture(texCheck);
	m_Box[0].SetMaterial(matRed);

	//texCheck->SetVertexProgram("C2E1v_green.cg", "C2E1v_green");
	//texCheck->SetFragmentProgram("C2E2f_passthru.cg", "C2E2f_passthru");

	m_Box[1].SetPosition(Vector3f(0.0f, 1.0f, 0.0f));
	Material* matGreen = ResourceManager<Material>::Instance().Create();
	matGreen->SetDiffuse(Color::GREEN);
	matGreen->SetAmbient(Color4f(0.0f, 0.2f, 0.0f));
	matGreen->SetTexture(renderer->GetTexture("check"));
	m_Box[1].SetMaterial(matGreen);

	//m_Box[2].SetPosition(Vector3f(1.0f, 1.0f, 2.0f));
	Material* matBlue = ResourceManager<Material>::Instance().Create();
	matBlue->SetDiffuse(Color4f(0.7f, 0.7f, 0.7f));
	matBlue->SetSpecular(Color4f(1.0f, 1.0f, 1.0f));
	matBlue->SetSpecularLevel(128.0f);
	//m_MatBlue->SetEmissive(Color4f(1.0f, 1.0f, 1.0f));
	//matBlue->SetTexture(renderer->GetTexture("check"));
	m_Box[2].SetMaterial(matBlue);
	//m_Box[2].SetMaterial(matColorTest);

	//m_CameraAttachedBox.SetMaterial(matBlue);

	//Material* testMat = ResourceManager<Material>::Instance().Create();
	//ResourceManager<Material>::Instance().Destroy(testMat);

	//m_CameraAttachedBox.SetMaterial(m_MatBlue);

	//FontManager::Instance().LoadFont("simhei", "../../../Data/simhei.ttf");

	m_UIFps = new TextUIControl();
	//m_UIFps->SetFont("DefaultFont");
	m_Scene->AddUIObject(m_UIFps);
}

void EmptyGame::Shutdown()
{
	SAFE_DELETE(m_UIFps);
	delete m_UIControl;

	SAFE_DELETE(m_Billboard);

	delete m_BoxMesh;
	//delete m_CharacterMesh;
	SAFE_DELETE(m_Light);
	SAFE_DELETE(m_Sun);
	SAFE_DELETE(m_AudioListener);
	SAFE_DELETE(m_Camera2);
	SAFE_DELETE(m_Camera1);

	delete m_Scene;
}

void EmptyGame::OnKeyPressed(unsigned int key)
{
	switch(key)
	{
	case KC_1:
		// Switch to camera 1
		m_CurrentCamera = m_Camera1;
		break;
	case KC_2:
		// Switch to camera 2
		m_CurrentCamera = m_Camera2;
		break;
	case KC_F:
		{
			bool debugRender = Engine::Instance().GetDebugRender();
			Engine::Instance().ToggleDebugRender(!debugRender);
		}
		break;
	case KC_SPACE:
		Engine::Instance().AudioSystem()->PlaySource("down");
		break;
	case KC_C:
		toggleCollision = !toggleCollision;
		break;
	case KC_G:
		toggleWireframe = !toggleWireframe;
		break;
	}

	m_Scene->SetCamera(m_CurrentCamera, false);
}

bool EmptyGame::OnNotifyQuitting()
{
	return true;
}

void EmptyGame::OnResizeWindow(unsigned int width, unsigned int height)
{
	// 窗口缩放以后要更新摄像机的纵横比...
	float aspect = (float)width / height;
	if (m_Camera1) m_Camera1->SetAspect(aspect);
	if (m_Camera2) m_Camera2->SetAspect(aspect);

	// ...以及投影矩阵
	if (m_CurrentCamera)
		renderer->ProjectionMatrix() = m_CurrentCamera->GetProjMatrix();

}

void EmptyGame::Update(unsigned long deltaTime)
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

	// W S A D控制摄像机移动
	if (Input::Instance().GetKeyDown(KC_W))
		forward += 0.1f * deltaTime / 10.0f * boost;
	if (Input::Instance().GetKeyDown(KC_S))
		forward += -0.1f * deltaTime / 10.0f * boost;
	if (Input::Instance().GetKeyDown(KC_A))
		right += -0.1f * deltaTime / 10.0f * boost;
	if (Input::Instance().GetKeyDown(KC_D))
		right += 0.1f * deltaTime / 10.0f * boost;

	forward += -0.1f * deltaTime / 10.0f * boost * Input::Instance().GetJoyStickAbs(2) / 0x7FFF;
	right += 0.1f * deltaTime / 10.0f * boost * Input::Instance().GetJoyStickAbs(3) / 0x7FFF;

	m_CurrentCamera->MoveLocal(forward, right, 0.0f);

	if (Input::Instance().GetKeyDown(KC_ESCAPE))
		Engine::Instance().SetQuitting(true);


	// 按住鼠标右键调整视角
	if (Input::Instance().GetMouseButtonDown(MB_Right))
	{
		float x = -(float)Input::Instance().GetMouseRelX() / 5.0f;
		float y = -(float)Input::Instance().GetMouseRelY() / 5.0f;

		m_CurrentCamera->RotateLocal(x, y);
	}

	// 鼠标左键点击进行射线检测
	if (Input::Instance().GetMouseButtonDown(MB_Left))
	{
		RenderWindowParam* param = System::Instance().GetRenderWindowParameters();

		unsigned int width = param->width;
		unsigned int height = param->height;

		float x = (float)Input::Instance().GetMouseAbsX() / width;
		float y = (float)Input::Instance().GetMouseAbsY() / height;


		Ray ray = m_Camera1->GetCameratRay(x, y);
		ObjectsCollisionInfos vec_obj;
		m_Scene->CollectRayPickingSceneObject(ray, vec_obj, COLLISION_ALL);

		CollisionInfo* nearest;
		if (vec_obj.size())
			nearest = &(*vec_obj.begin());

		for (ObjectsCollisionInfos::iterator iter = vec_obj.begin();
			iter != vec_obj.end();
			iter++)
		{
			//(*iter)->SetDebugRender(true);
			float dist1 = (nearest->obj->WorldTransform().GetPosition() - m_Camera1->WorldTransform().GetPosition()).SquaredLength();
			float dist2 = ((*iter).obj->WorldTransform().GetPosition() - m_Camera1->WorldTransform().GetPosition()).SquaredLength();
			if (dist2 < dist1 && (*iter).obj!= m_Camera1/* && (*iter).obj!=&m_Box[1]*/)
			{
				nearest = &(*iter);
			}
		}

		if (vec_obj.size() /*&& nearest->obj!=&m_Box[1]*/)
		{
			nearest->obj->SetDebugRender(true);


			//Vector3f lookat = nearest->normal;
			//Vector3f vec_right = CrossProduct(lookat, Vector3f(0.0f, 1.0f, 0.0f));
			//m_Box[1].Transform().SetLookAt(lookat, CrossProduct(vec_right, lookat));
			//m_Box[1].SetPosition(nearest->point);
		}

		//sprintf(buf, "Picking num: %d", vec_obj.size());
	}

	// 使用手柄控制当前摄像机
	float x = -(float)Input::Instance().GetJoyStickAbs(1) / 0x7FFF * 0.1f * deltaTime;
	float y = -(float)Input::Instance().GetJoyStickAbs(0) / 0x7FFF * 0.1f * deltaTime;
	m_CurrentCamera->RotateLocal(x, y);

	m_Angle += 0.005f * deltaTime;

	m_Box[0].SetRotation(Matrix3::BuildRollRotationMatrix(m_Angle));

	//Vector3f lookat = m_Camera1->WorldTransform().GetPosition() - m_Box[2].Transform().GetPosition();
	//lookat.normalize();
	//Vector3f vec_right = CrossProduct(lookat, Vector3f(0.0f, 1.0f, 0.0f));
	//m_Box[2].Transform().SetLookAt(lookat, CrossProduct(vec_right, lookat));

	//Vector3f sunDir = Vector3f(0.5f * sin(m_Angle), 1.0f, 0.5f * cos(m_Angle));
	//sunDir.normalize();
	//m_Sun->SetDirection(sunDir);

	//renderer->ViewMatrix() = m_CurrentCamera->GetViewMatrix();

	//float val = (sin(m_Angle) + 1.0f) / 2;
	//m_Light->SetDiffuseColor(Color4f(val, val, val));
	//m_Light->SetPosition(Vector3f(sin(m_Angle / 5) * 10.0f, 3.0f, 3.0f));

	Vector3f campos_old = m_CurrentCamera->WorldTransform().GetPosition();

	// ******************************************* 更新 *******************************************
	m_Scene->UpdateScene(deltaTime);
	// ******************************************* 更新 *******************************************

	static Matrix4 biasMat = Matrix4(0.5f, 0.0f, 0.0f, 0.5f,
									 0.0f, 0.5f, 0.0f, 0.5f,
									 0.0f, 0.0f, 0.5f, 0.5f,
									 0.0f, 0.0f, 0.0f, 1.0f);

	/*biasMat * *//*renderer->ProjectionMatrix() **/ /*m_Camera1->GetViewMatrix()*/
	//renderer->GetTexture("colortest")->SetEyeSpaceMatrix(biasMat * m_Camera1->GetProjMatrix() * m_Camera1->GetViewMatrix()/*.GetInverseMatrix()*/);
	//Matrix4 rotMat;
	//rotMat.Identity();
	//rotMat.SetRotation(Matrix3::BuildRollRotationMatrix(m_Angle / 5.0f));
	// In Object Space?
	//renderer->GetTexture("no_material")->SetEyeSpaceMatrix(biasMat * rotMat/* * m_CameraAttachedBox.WorldTransform()*/);

	Vector3f campos = m_CurrentCamera->WorldTransform().GetPosition();

	// 重力
	//campos += Vector3f(0.0f, -0.1f, 0.0f);

	// 摄像机碰撞检测
	bool inSolid = m_BspScene->IsPointInSolid(campos);

	// TODO: 这里的方法用碰撞检测类管理，应当适用于所有对象
	if (toggleCollision)
	{
		Vector3f cam_hit;
		BspTriangle* cam_hit_tri;
		//if (m_BspScene->Intersects(Ray(campos_old, campos), &cam_hit, &cam_hit_tri))
		//{
		//	campos = cam_hit + cam_hit_tri->normal;
		//}
		Vector3f newpos;
		m_BspScene->PushSphere(campos, newpos, 5.0f);

		m_CurrentCamera->SetPosition(newpos);
	}
	
	Ray los = m_CurrentCamera->GetCameratRay(0.5, 0.5);
	Vector3f hit_pos;
	BspTriangle* tri;
	bool los_col = m_BspScene->Intersects(los, &hit_pos, &tri);

	if (toggleWireframe)
	{
		vector<BspTriangle*> triList;
		m_BspScene->TraverseTree(&triList, m_CurrentCamera->WorldTransform().GetPosition());

		for (vector<BspTriangle*>::iterator iter=triList.begin(); iter!=triList.end(); iter++)
		{
			m_Scene->DrawLine((*iter)->vertices[0], (*iter)->vertices[1], Color4f(0.5f, 0.5f, 1.0f));
			m_Scene->DrawLine((*iter)->vertices[1], (*iter)->vertices[2], Color4f(0.5f, 0.5f, 1.0f));
			m_Scene->DrawLine((*iter)->vertices[2], (*iter)->vertices[0], Color4f(0.5f, 0.5f, 1.0f));
		}
	}

	// 显示FPS
	unsigned int fps = Engine::Instance().GetFPS();

	// 射线与bsp发生碰撞，显示当前碰撞的三角形
	char hitpos[1024];
	if (los_col)
	{
		sprintf(hitpos, "Hit pos: (%f, %f, %f)", hit_pos.x, hit_pos.y, hit_pos.z);
		m_Light->SetPosition(hit_pos);

		m_Scene->DrawLine(tri->vertices[0], tri->vertices[1], Color4f(1.0f, 0.0f, 0.0f));
		m_Scene->DrawLine(tri->vertices[1], tri->vertices[2], Color4f(1.0f, 0.0f, 0.0f));
		m_Scene->DrawLine(tri->vertices[2], tri->vertices[0], Color4f(1.0f, 0.0f, 0.0f));

		m_Scene->DrawLine(tri->vertices[0], tri->vertices[0] + tri->normal * 5.0f, Color4f(0.5f, 1.0f, 0.5f));
		m_Scene->DrawLine(tri->vertices[1], tri->vertices[1] + tri->normal * 5.0f, Color4f(0.5f, 1.0f, 0.5f));
		m_Scene->DrawLine(tri->vertices[2], tri->vertices[2] + tri->normal * 5.0f, Color4f(0.5f, 1.0f, 0.5f));
	}
	else
		sprintf(hitpos, "No hit pos.");
	sprintf(buf, "FPS: %d\nCamera in solid: %d\nLOS hit: %d\n%s\nCam pos: %f %f %f", fps, inSolid, los_col, hitpos, campos.x, campos.y, campos.z);
	//sprintf(buf, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	//System::Instance().SetWindowTitle(buf);
	m_UIFps->SetText(buf);

	// 更新光照
	LightingManager::Instance().Update();
}

void EmptyGame::RenderScene()
{
	m_Scene->RenderScene();
}

