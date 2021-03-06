#include "GameSkeletal.h"

GameSkeletal::GameSkeletal()
 : m_Scene(NULL),
   m_Camera(NULL),
   m_Sky(NULL),
   m_UIFps(NULL)
{
}

GameSkeletal::~GameSkeletal()
{
}

void GameSkeletal::StartGame()
{
	m_Scene = new SceneGraph;
	Renderer::Instance().SetAmbientColor(Color4f(1.f, 1.f, 1.f));

	//Renderer::Instance().SetClearColor(Color4f(0.0f, 0.5f, 0.5f));

	m_Camera = FACTORY_CREATE(m_Scene, Camera);
	m_Camera->SetFarClippingDistance(1000.0f);

	// 创建天空盒
	m_Sky = FACTORY_CREATE(m_Scene, DistantViewObject);
	m_Sky->SetMesh(MeshManager::Instance().GetByName("skybox.EMD"));

	// 创建主光源(阳光)
	Light* light = FACTORY_CREATE(m_Scene, Light);
	light->SetLightType(LIGHT_TYPE_DIRECTIONAL);
	light->SetDirection(Vector3f(0.787f, 0.38f, 0.486f));
	LightingManager::Instance().AddGlobalLight(light);

	// 创建辅助背光源
	Light* light2 = FACTORY_CREATE(m_Scene, Light);
	light2->SetLightType(LIGHT_TYPE_DIRECTIONAL);
	light2->SetDirection(Vector3f(-0.787f, -0.38f, -0.486f));
	light2->SetDiffuseColor(Color4f(0.2f, 0.2f, 0.2f));
	LightingManager::Instance().AddGlobalLight(light2);

	// 创建白色受光材质
	Material* white = MaterialManager::Instance().Create("white");

	// 创建骨骼动画对象
	m_SkelMesh1 = FACTORY_CREATE(m_Scene, SkeletalMeshObject);
	BaseMesh* mesh = MeshManager::Instance().GetByName("Hammer.emd");
	Skeleton* skel = SkeletonManager::Instance().GetByName("Hammer");
	m_SkelMesh1->SetMesh(mesh);
	m_SkelMesh1->SetSkeleton(skel);
	m_SkelMesh1->PlayAnimation("default");
	m_SkelMesh1->SetMaterial(white, 0);
	//m_SkelMesh1->CreateLightableObject();
	m_SkelMesh1->SetPosition(Vector3f(-25.0f, 0.0f, -30.0f));

	m_SkelMesh2 = FACTORY_CREATE(m_Scene, SkeletalMeshObject);
	m_SkelMesh2->SetMesh(mesh);
	m_SkelMesh2->SetSkeleton(skel);
	m_SkelMesh2->PlayAnimation("default");
	m_SkelMesh2->SetAnimationTime(5.0f);
	m_SkelMesh2->SetMaterial(white, 0);
	//m_SkelMesh2->AttachChildObject(m_SkelMesh1);
	m_SkelMesh2->SetPosition(Vector3f(25.0f, 0.0f, -30.0f));

	m_UIFps = EGUIManager::Instance().CreateTextUIControl();

	InputEventHandler<GameSkeletal>* inputHandler =
		new InputEventHandler<GameSkeletal>(this,
											&GameSkeletal::OnKeyPressed,
											NULL,
											&GameSkeletal::OnMouseMoved);
	Input->SetEventHandler(inputHandler);
}

void GameSkeletal::Shutdown()
{
	delete m_Scene;
}

void GameSkeletal::OnKeyPressed(KeyCode key)
{
	switch(key)
	{
	case KC_P:
		static bool debugRender = false;
		debugRender = !debugRender;
		DebugRenderer::ToggleSceneObjectDebugRender(debugRender);
		return;
	}
}

void GameSkeletal::OnMouseMoved(int x, int y, int rel_x, int rel_y)
{
	// 按住鼠标右键调整视角
	if (Input->GetMouseButtonDown(MB_Right))
	{
		float fx = -(float)rel_x / 5.0f;
		float fy = -(float)rel_y / 5.0f;

		System::Instance().CenterMouseCursor();

		m_Camera->RotateLocal(fx, fy);
	}
}

bool GameSkeletal::OnNotifyQuitting()
{
	return true;
}

void GameSkeletal::OnResizeWindow(unsigned int width, unsigned int height)
{
	// 窗口缩放以后要更新摄像机的纵横比...
	float aspect = (float)width / height;
	if (m_Camera)
	{
		m_Camera->SetAspect(aspect);
		
		// ...以及投影矩阵
		Renderer::Instance().SetProjectionMatrix(m_Camera->GetProjMatrix());
	}

}

static int frame = 0;

void GameSkeletal::Update(unsigned long deltaTime)
{

	float boost;
	char buf[1024] = "\0";

	// 左Shift键加速移动
	if (Input->GetKeyDown(KC_LSHIFT))
		boost = 4.0f;
	else
		boost = 1.0f;

	float forward = 0.0f;
	float right = 0.0f;

	// W S A D控制摄像机移动
	if (Input->GetKeyDown(KC_W))
		forward += 0.1f * deltaTime / 10.0f * boost;
	if (Input->GetKeyDown(KC_S))
		forward += -0.1f * deltaTime / 10.0f * boost;
	if (Input->GetKeyDown(KC_A))
		right += -0.1f * deltaTime / 10.0f * boost;
	if (Input->GetKeyDown(KC_D))
		right += 0.1f * deltaTime / 10.0f * boost;

	//forward += -0.1f * deltaTime / 10.0f * boost * Input->GetJoyStickAbs(2) / 0x7FFF;
	//right += 0.1f * deltaTime / 10.0f * boost * Input->GetJoyStickAbs(3) / 0x7FFF;

	m_Camera->MoveLocal(forward, right, 0.0f);

	if (Input->GetKeyDown(KC_ESCAPE))
		Engine::Instance().SetQuitting(true);

	// 使用手柄控制当前摄像机
	//float x = -(float)Input->GetJoyStickAbs(1) / 0x7FFF * 0.1f * deltaTime;
	//float y = -(float)Input->GetJoyStickAbs(0) / 0x7FFF * 0.1f * deltaTime;
	//m_Camera->RotateLocal(x, y);

	static float angle = 0.0f;
	angle += 0.001f * deltaTime;

	Quaternion quat;
	quat.CreateFromLocalAxisAngle(Vector3f(0.0f, 1.0f, 0.0f), angle);
	m_SkelMesh1->SetRotation(quat);

	// ******************************************* 更新 *******************************************
	m_Scene->UpdateScene(deltaTime);
	// ******************************************* 更新 *******************************************


	// 显示FPS
	unsigned int fps = Engine::Instance().GetFPS();

	sprintf(buf, "FPS: %d", fps);
	m_UIFps->SetText(buf);
}

void GameSkeletal::RenderScene()
{
	// 设定渲染视点
	m_Scene->SetupRenderView(m_Camera);
	m_Scene->RenderScene();
}
