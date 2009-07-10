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

static Skeletal skel;

void GameSkeletal::StartGame()
{
	m_Scene = new SceneGraph;
	m_Scene->SetAmbientColor(Color4f(1.f, 1.f, 1.f));

	renderer->SetClearColor(Color4f(0.0f, 0.5f, 0.5f));

	m_Camera = new Camera;
	m_Camera->SetFarClippingDistance(1000.0f);

	// ����ӵ���������������޷��ƶ�
	m_Scene->AddObject(m_Camera);
	m_Scene->SetCamera(m_Camera);

	m_Sky = new DistantViewObject();
	m_Sky->SetMesh(MeshManager::Instance().GetMesh("skybox"));
	m_Scene->AddObject(m_Sky);

	skel.LoadFromFile("E:/testout.txt");

	m_UIFps = new TextUIControl();
	m_Scene->AddUIObject(m_UIFps);
}

void GameSkeletal::Shutdown()
{
	SAFE_DELETE(m_UIFps);
	SAFE_DELETE(m_Sky);
	SAFE_DELETE(m_Camera);

	delete m_Scene;
}

void GameSkeletal::OnKeyPressed(unsigned int key)
{
	switch(key)
	{
	case KC_P:
		Engine::Instance().ToggleDebugRender(!Engine::Instance().GetDebugRender());
	}
}

bool GameSkeletal::OnNotifyQuitting()
{
	return true;
}

void GameSkeletal::OnResizeWindow(unsigned int width, unsigned int height)
{
	// ���������Ժ�Ҫ������������ݺ��...
	float aspect = (float)width / height;
	if (m_Camera)
	{
		m_Camera->SetAspect(aspect);
		
		// ...�Լ�ͶӰ����
		renderer->ProjectionMatrix() = m_Camera->GetProjMatrix();
	}

}

void GameSkeletal::Update(unsigned long deltaTime)
{

	float boost;
	char buf[1024] = "\0";

	// ��Shift�������ƶ�
	if (Input::Instance().GetKeyDown(KC_LSHIFT))
		boost = 4.0f;
	else
		boost = 1.0f;

	float forward = 0.0f;
	float right = 0.0f;

	// W S A D����������ƶ�
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

	m_Camera->MoveLocal(forward, right, 0.0f);

	if (Input::Instance().GetKeyDown(KC_ESCAPE))
		Engine::Instance().SetQuitting(true);

	// ��ס����Ҽ������ӽ�
	if (Input::Instance().GetMouseButtonDown(MB_Right))
	{
		float x = -(float)Input::Instance().GetMouseRelX() / 5.0f;
		float y = -(float)Input::Instance().GetMouseRelY() / 5.0f;

		System::Instance().CenterMouseCursor();

		m_Camera->RotateLocal(x, y);
	}

	// ʹ���ֱ����Ƶ�ǰ�����
	float x = -(float)Input::Instance().GetJoyStickAbs(1) / 0x7FFF * 0.1f * deltaTime;
	float y = -(float)Input::Instance().GetJoyStickAbs(0) / 0x7FFF * 0.1f * deltaTime;
	m_Camera->RotateLocal(x, y);

	static int frame = 0;
	static unsigned long timeToAdvance = 0;
	timeToAdvance += deltaTime;
	if (timeToAdvance > 33)
	{
		frame++;
		frame %= 100;

		timeToAdvance = 0;

		skel.Update(frame);
	}
	for (vector<Bone*>::iterator iter=skel.m_Bones[frame].begin(); iter!=skel.m_Bones[frame].end(); iter++)
	{
		if ((*iter)->m_Parent)
			m_Scene->DrawLine((*iter)->m_RootTransform.GetPosition(), (*iter)->m_Parent->m_RootTransform.GetPosition());
	}

	// ******************************************* ���� *******************************************
	m_Scene->UpdateScene(deltaTime);
	// ******************************************* ���� *******************************************


	// ��ʾFPS
	unsigned int fps = Engine::Instance().GetFPS();

	sprintf(buf, "FPS: %d", fps);
	m_UIFps->SetText(buf);

	// ���¹���
	LightingManager::Instance().Update();
}

void GameSkeletal::RenderScene()
{
	m_Scene->RenderScene();
}
