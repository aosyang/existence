#include "ParticleGame.h"

ParticleGame::ParticleGame()
: m_Scene(NULL),
  m_Camera(NULL),
  m_UIFps(NULL),
  m_EmitTime(1000)
{
}

ParticleGame::~ParticleGame()
{
}


Material*		m_MatSmoke;

void ParticleGame::StartGame()
{
	m_Scene = new SceneGraph;

	m_Scene->SetAmbientColor(Color4f(0.7f, 0.7f, 0.7f));

	m_Camera = new Camera();
	m_Camera->SetPosition(Vector3f(1.0f, 0.0f, 5.0f));
	m_Scene->AddObject(m_Camera);
	m_Scene->SetCamera(m_Camera);

	m_MatSmoke = ResourceManager<Material>::Instance().Create();
	m_MatSmoke->SetTexture(renderer->GetTexture("smoke"));
	m_MatSmoke->SetLighting(false);

	m_UIFps = new TextUIControl();
	m_Scene->AddUIObject(m_UIFps);

	Mesh* mesh = new Mesh();
	mesh->CreateBox(1.0f);
	MeshObject* obj = new MeshObject();
	obj->SetMesh(mesh);
	m_Scene->AddObject(obj);
}

void ParticleGame::Shutdown()
{
	SAFE_DELETE(m_UIFps);
	SAFE_DELETE(m_Camera);
	SAFE_DELETE(m_Scene);
}

void ParticleGame::OnKeyPressed(unsigned int key)
{

}

bool ParticleGame::OnNotifyQuitting()
{
	return true;
}

void ParticleGame::OnResizeWindow(unsigned int width, unsigned int height)
{
	float aspect = (float)width / height;
	if (m_Camera)
		renderer->ProjectionMatrix() = m_Camera->GetProjMatrix();

}

void ParticleGame::Update(unsigned long deltaTime)
{
	float boost;
	char buf[1024] = "\0";

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

	forward += -0.1f * deltaTime / 10.0f * boost * Input::Instance().GetJoyStickAbs(2) / 0x7FFF;
	right += 0.1f * deltaTime / 10.0f * boost * Input::Instance().GetJoyStickAbs(3) / 0x7FFF;

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

	m_EmitTime -= deltaTime;

	if (m_EmitTime<=0)
	{
		EmitParticle(Vector3f(0.0f, 0.0f, 0.0f), m_Scene, m_ParticlePool);
		m_EmitTime = 500;
	}

	ParticlePool::iterator iter;
	for (iter = m_ParticlePool.begin(); iter!=m_ParticlePool.end(); iter++)
	{
		iter->life -= deltaTime;
		Vector3f pos = iter->bb->WorldTransform().GetPosition();
		pos += iter->dir * deltaTime / 1000.0f;
		iter->bb->SetPosition(pos);
	}

	m_Scene->UpdateScene(deltaTime);

	unsigned int fps = Engine::Instance().GetFPS();
	sprintf(buf, "FPS: %d", fps);
	//System::Instance().SetWindowTitle(buf);
	m_UIFps->SetText(buf);
}

void ParticleGame::RenderScene()
{
	m_Scene->RenderScene();
}

void EmitParticle(const Vector3f& pos, SceneGraph* scene, ParticlePool& pool)
{
	Particle* particle = new Particle;
	particle->bb = new Billboard();
	scene->AddObject(particle->bb);
	particle->bb->SetPosition(pos);
	particle->bb->SetMaterial(m_MatSmoke);
	particle->life = 3000;
	//float x = (float)rand() / 0xFF * 0.3f;
	//float y = (float)rand() / 0xFF * 0.3f + 0.7f;
	//float z = (float)rand() / 0xFF * 0.3f;
	float x = 0.0f;
	float y = 1.0f;
	float z = 0.0f;
	particle->dir = Vector3f(x, y, z);
	particle->dir.normalize();

	pool.push_back(*particle);
}

