#include "GameBasicTransform.h"

GameBasicTransform::GameBasicTransform()
: m_Scene(NULL)
{
}

GameBasicTransform::~GameBasicTransform()
{
}


void GameBasicTransform::StartGame()
{
	m_Scene = new SceneGraph;

	// 创建摄像机
	m_Camera = new Camera;
	m_Camera->SetPosition(Vector3f(1.0f, 2.0f, 5.0f));

	// 注：需要将Camera添加到场景才能正确渲染
	m_Scene->AddObject(m_Camera);
	m_Scene->SetCamera(m_Camera);

	// 说明看板
	m_BoardMaterial = new Material();
	m_BoardMaterial->SetTexture(renderer->GetTexture("tutorial1_1"));

	m_BoardMesh = new Mesh();
	m_BoardMesh->CreatePositiveYPlane(4.0f);

	m_TutorialBoard = new MeshObject();
	m_TutorialBoard->SetMesh(m_BoardMesh);
	m_TutorialBoard->SetMaterial(m_BoardMaterial);
	m_TutorialBoard->SetPosition(Vector3f(5.0f, 0.0f, 2.5f));

	//renderer->AddRenderObject(m_TutorialBoard);
	m_Scene->AddObject(m_TutorialBoard);

	// 示例box
	m_BoxMaterial = new Material();
	m_BoxMaterial->SetTexture(renderer->GetTexture("check"));

	m_BoxMesh = new Mesh();
	m_BoxMesh->CreateBox(1.0f);
	//m_BoxMesh->LoadFromFile("../../../data/sphere.emd");

	m_TutorialBox = new MeshObject();
	m_TutorialBox->SetMesh(m_BoxMesh);
	m_TutorialBox->SetMaterial(m_BoxMaterial);

	//renderer->AddRenderObject(m_TutorialBox);
	m_Scene->AddObject(m_TutorialBox);
}	
void GameBasicTransform::Shutdown()
{
	delete m_BoxMesh;
	delete m_TutorialBox;
	delete m_BoxMaterial;

	delete m_TutorialBoard;
	delete m_BoardMesh;

	delete m_BoardMaterial;

	delete m_Camera;
	delete m_Scene;
}

bool GameBasicTransform::OnNotifyQuitting()
{
	return true;
}

void GameBasicTransform::OnKeyPressed(unsigned int key) {}
void GameBasicTransform::OnKeyReleased(unsigned int key) {}

void GameBasicTransform::Update(unsigned long deltaTime)
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


	// 按住鼠标右键调整视角
	if (Input::Instance().GetMouseButtonDown(MB_Right))
	{
		float x = -(float)Input::Instance().GetMouseRelX() / 5.0f;
		float y = -(float)Input::Instance().GetMouseRelY() / 5.0f;

		m_Camera->RotateLocal(x, y);
	}

	//renderer->SetViewMatrix(m_Camera->GetViewMatrix());
	m_Scene->UpdateScene(deltaTime);
}

void GameBasicTransform::RenderScene()
{
	m_Scene->RenderScene();
}
