#include "GameEditor.h"

GameEditor::GameEditor()
 : m_Camera(NULL),
   m_BoxMesh(NULL),
   m_Angle(0.0f)
{
}

GameEditor::~GameEditor()
{
}

void GameEditor::StartGame()
{
	m_Scene = new SceneGraph;

	m_Camera = new Camera;
	m_Camera->SetPosition(Vector3f(1.0f, 0.0f, 5.0f));

	m_Scene->AddObject(m_Camera);
	m_Scene->SetCamera(m_Camera);

	// 创建一个立方体Mesh
	m_BoxMesh = ResourceManager<Mesh>::Instance().Create();
	m_BoxMesh->CreateBox(1.0f);

	// 将三个Box添加到渲染列表中
	for (int i=0; i<3; i++)
	{
		m_Box[i].SetMesh(m_BoxMesh);
		m_Scene->AddObject(&m_Box[i], false);
	}

	// 为三个Box指定不同的材质
	m_Box[0].SetPosition(Vector3f(1.0f, 0.0f, 0.0f));
	m_MatRed = ResourceManager<Material>::Instance().Create();
	m_MatRed->SetDiffuse(Color::RED);
	m_MatRed->SetTexture(renderer->GetTexture("check"));
	m_Box[0].SetMaterial(m_MatRed, 0);

	m_Box[1].SetPosition(Vector3f(0.0f, 1.0f, 0.0f));
	m_MatGreen = ResourceManager<Material>::Instance().Create();
	m_MatGreen->SetDiffuse(Color::GREEN);
	m_MatGreen->SetTexture(renderer->GetTexture("check"));
	m_Box[1].SetMaterial(m_MatGreen, 0);

	m_Box[2].SetPosition(Vector3f(1.0f, 1.0f, 2.0f));
	m_MatBlue = ResourceManager<Material>::Instance().Create();
	m_MatBlue->SetDiffuse(Color4f(1.0f, 1.0f, 1.0f));
	m_MatBlue->SetTexture(renderer->GetTexture("check"));
	m_Box[2].SetMaterial(m_MatBlue, 0);

}

void GameEditor::Shutdown()
{
	delete m_Scene;
}

bool GameEditor::OnNotifyQuitting()
{
	return true;
}

void GameEditor::OnKeyPressed(unsigned int key)
{

}

void GameEditor::OnKeyReleased(unsigned int key)
{
}

void GameEditor::Update(unsigned long deltaTime)
{
	float boost;

	if (Input::Instance().GetKeyDown(KC_LSHIFT))
		boost = 4.0f;
	else
		boost = 1.0f;

	float forward = 0.0f;
	float right = 0.0f;

	if (Input::Instance().GetKeyDown(KC_W))
		forward += 0.1f * deltaTime / 10.0f * boost;
	if (Input::Instance().GetKeyDown(KC_S))
		forward += -0.1f * deltaTime / 10.0f * boost;
	if (Input::Instance().GetKeyDown(KC_A))
		right += -0.1f * deltaTime / 10.0f * boost;
	if (Input::Instance().GetKeyDown(KC_D))
		right += 0.1f * deltaTime / 10.0f * boost;

	m_Camera->MoveLocal(forward, right, 0.0f);

	if (Input::Instance().GetKeyDown(KC_ESCAPE))
		Engine::Instance().SetQuitting(true);


	// 按住鼠标右键调整视角
	if (Input::Instance().GetMouseButtonDown(MB_Right))
	{
		float x = -(float)Input::Instance().GetMouseRelX() / 5.0f;
		float y = -(float)Input::Instance().GetMouseRelY() / 5.0f;

		m_Camera->RotateLocal(x, y);
	}

	m_Angle += 0.005f * deltaTime;

	m_Box[0].SetRotation(Matrix3::BuildRollRotationMatrix(m_Angle));

	//renderer->SetViewMatrix(m_Camera->GetViewMatrix());

	m_Scene->UpdateScene(deltaTime);
}

void GameEditor::RenderScene()
{
	m_Scene->RenderScene();
}
