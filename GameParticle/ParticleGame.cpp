#include "particleGame.h"

particleGame::particleGame()
: m_Scene(NULL),
  m_Camera(NULL),
  m_UIFps(NULL),
  m_EmitTime(1000)
{
}

particleGame::~particleGame()
{
}


Material*		m_MatSmoke;

// 粒子初始状态设置
void ParticleState(Particle* particle, ParticleEmitter* emitter)
{
	particle->m_LifeTime = 5000;
	particle->m_Velocity = particle->m_Position - emitter->WorldTransform().GetPosition();
	particle->m_Velocity.normalize();
	particle->m_Velocity *= 2.0f;

	particle->m_Color = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
	//particle->m_Color = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
	//particle->m_Color = Color4f(MathRandom(0.0f, 1.0f), MathRandom(0.0f, 1.0f), MathRandom(0.0f, 1.0f), 1.0f);
	particle->m_Scale = MathRandom(10.0f, 15.f);
	particle->m_ScaleInc = 20.0f;
	particle->m_ZRotation = MathRandom(-kPi, kPi);
	particle->m_ZRotationInc = MathRandom(-0.5f, 0.5f);
}

// 单一粒子更新行为
bool ParticleUpdate(Particle* particle, unsigned long deltaTime)
{
	if (particle->m_LifeTime > deltaTime)
		particle->m_LifeTime -= deltaTime;
	else
		particle->m_Active = false;

	particle->m_Position += particle->m_Velocity * (float)deltaTime * 0.001f;
	particle->m_ZRotation += particle->m_ZRotationInc * (float)deltaTime * 0.001f;
	particle->m_Scale += particle->m_ScaleInc * (float)deltaTime * 0.001f;

	float alpha = (float)particle->m_LifeTime / 5000 * kPi;
	particle->m_Color.a = sinf(alpha) * 0.5f;

	return particle->m_Active;
}

ParticleEmitter* emitter;
Billboard* bb;

void particleGame::StartGame()
{
	m_Scene = new SceneGraph;

	m_Scene->SetAmbientColor(Color4f(0.7f, 0.7f, 0.7f));

	m_Camera = new Camera();
	m_Camera->SetPosition(Vector3f(1.0f, 0.0f, 5.0f));
	m_Camera->SetFarClippingDistance(1000.0f);
	m_Scene->AddObject(m_Camera);
	m_Scene->SetCamera(m_Camera);

	m_MatSmoke = ResourceManager<Material>::Instance().Create();
	m_MatSmoke->SetTexture(renderer->GetTexture("smoke"));
	m_MatSmoke->SetLighting(false);
	m_MatSmoke->SetDepthWriting(false);
	//m_MatSmoke->SetAlphaTest(true);
	//m_MatSmoke->SetAlphaReference(0.2f);
	//m_MatSmoke->GetTextureRenderState(0)->envMode = ENV_MODE_ADD;

	m_UIFps = new TextUIControl();
	m_Scene->AddUIObject(m_UIFps);

	//Mesh* mesh = ResourceManager<Mesh>::Instance().GetByName("marcus");
	Mesh* mesh = ResourceManager<Mesh>::Instance().Create();
	mesh->CreateBox(1.0f);
	MeshObject* obj = new MeshObject();
	obj->SetMesh(mesh);
	m_Scene->AddObject(obj);

	ParticlePool* pool = new ParticlePool;
	pool->SetMaterial(m_MatSmoke);
	m_Scene->AddObject(pool);

	for (int i=0; i<50; i++)
	{
		Particle p;
		p.m_Active = true;
		//p.m_Color = Color4f(0.0f, 0.0f, 0.0f, 0.5f);
		p.m_Color = Color4f(1.0f, 1.0f, 1.0f, 0.5f);
		p.m_Scale = MathRandom(50.0f, 100.0f);
		p.m_ZRotation = MathRandom(-kPi, kPi);
		p.m_ZRotationInc = MathRandom(-0.1f, 0.1f);
		//p.m_ScreenScaleX = MathRandom(0.5f, 1.5f);
		//p.m_ScreenScaleY = MathRandom(0.5f, 1.5f);
		p.m_Position = Vector3f(MathRandom(-50.0f, 50.0f), MathRandom(0.0f, 10.0f), MathRandom(-50.0f, 50.0f));

		pool->AddParticle(p);
	}

	Material* matFlare = ResourceManager<Material>::Instance().Create();
	ITexture* tex_flare = renderer->GetTexture("flare");
	matFlare->SetTexture(tex_flare);
	matFlare->SetLighting(false);
	matFlare->SetDepthWriting(false);

	// 混合模式 叠加
	matFlare->GetTextureRenderState(0)->useBlending = true;
	matFlare->GetTextureRenderState(0)->srcBlendFactor = BLEND_FACTOR_ONE;
	matFlare->GetTextureRenderState(0)->dstBlendFactor = BLEND_FACTOR_ONE;
	//matFlare->GetTextureRenderState(0)->envMode = ENV_MODE_ADD;

	// 光晕
	bb = new Billboard();
	bb->SetMaterial(matFlare);
	bb->SetScale(10.0f);
	bb->SetScreenSpaceScale(10.0f, 1.0f);
	bb->SetColor(Color4f(1.0f, 0.5f, 0.0f, 1.0f));
	bb->SetRenderOrder(120);
	m_Scene->AddObject(bb);

	emitter = new ParticleEmitter();
	emitter->SetInterval(50);
	emitter->SetMaterial(m_MatSmoke);
	emitter->SetEmitterShape(EMITTER_SHAPE_BOX);
	emitter->SetBoxRange(Vector3f(-1.0f, -1.0f, -1.0f), Vector3f(1.0f, 1.0f, 1.0f));
	emitter->SetParticleInitStateFunc(ParticleState);
	emitter->SetPosition(Vector3f(0.0f, 150.0f, 0.0f));
	emitter->SetParticleBehaviorFunc(&ParticleUpdate);
	m_Scene->AddObject(emitter);
	//m_Camera->AttachChildObject(emitter, true);
	emitter->AttachChildObject(bb);
}

void particleGame::Shutdown()
{
	SAFE_DELETE(m_UIFps);
	SAFE_DELETE(m_Scene);
}

void particleGame::OnKeyPressed(unsigned int key)
{

}

bool particleGame::OnNotifyQuitting()
{
	return true;
}

void particleGame::OnResizeWindow(unsigned int width, unsigned int height)
{
	float aspect = (float)width / height;
	if (m_Camera)
	{
		m_Camera->SetAspect(aspect);
		renderer->ProjectionMatrix() = m_Camera->GetProjMatrix();
	}

}

void particleGame::Update(unsigned long deltaTime)
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

	//m_EmitTime -= deltaTime;

	//if (m_EmitTime<=0)
	//{
	//	Emitparticle(Vector3f(0.0f, 0.0f, 0.0f), m_Scene, m_particlePool);
	//	m_EmitTime = 500;
	//}

	float fNear = m_Camera->GetNearClippingDistance();
	float fFar = m_Camera->GetFarClippingDistance();
	float fPercent = (fFar - fNear) * 0.1f;

	particlePool::iterator iter;
	for (iter = m_particlePool.begin(); iter!=m_particlePool.end(); iter++)
	{
		iter->life -= deltaTime;
		Vector3f pos = iter->bb->WorldTransform().GetPosition();
		pos += iter->dir * deltaTime / 1000.0f;
		iter->bb->SetPosition(pos);
		iter->scale += iter->scale_inc * deltaTime / 100;
		iter->bb->SetScale(iter->scale);
		iter->rot += iter->rot_inc * deltaTime / 100;
		iter->bb->SetZRotation(iter->rot);

		//float fCamDist = renderer->GetFrustum()->SphereInFrustum(iter->bb->WorldTransform().GetPosition(), 5.0f);
		//if (fCamDist < fPercent)
		//{
		float fAplha  = max((float)iter->life, 0.0f) / 30000;
		iter->bb->SetColor(Color4f(1.0f, 1.0f, 1.0f, fAplha));
		//}
	}

	static float rot = 0.0f;
	rot += float(deltaTime) / 1000.0f;

	emitter->SetPosition(Vector3f(sinf(rot) * 100.0f, 50.0f, cosf(rot) * 100.0f));
	bb->SetScale(sinf(rot * 20.0f) * 1.0f + 10.0f);

	m_Scene->UpdateScene(deltaTime);

	unsigned int fps = Engine::Instance().GetFPS();
	sprintf(buf, "FPS: %d", fps);
	//System::Instance().SetWindowTitle(buf);
	m_UIFps->SetText(buf);
}

void particleGame::RenderScene()
{
	m_Scene->RenderScene();
}

void Emitparticle(const Vector3f& pos, SceneGraph* scene, particlePool& pool)
{
	particle particle;
	particle.bb = new Billboard();
	scene->AddObject(particle.bb);
	particle.bb->SetPosition(pos);
	particle.bb->SetMaterial(m_MatSmoke);
	particle.bb->SetColor(Color4f(1.0f, 0.5f, 0.5f, 1.0f));
	particle.rot = MathRandom(-kPi, kPi);
	particle.bb->SetZRotation(particle.rot);
	//particle.bb->SetScreenSpaceScale(1.2f, 0.8f);
	particle.life = 30000;
	particle.scale = MathRandom(0.5f, 1.5f);
	particle.scale_inc = MathRandom(0.05f, .1f);
	particle.rot_inc = MathRandom(-0.005f, 0.005f);
	float x = (float)rand() / 0xFF * 0.3f;
	float y = (float)rand() / 0xFF * 0.3f + 0.7f;
	float z = (float)rand() / 0xFF * 0.3f;
	//float x = 0.0f;
	//float y = 1.0f;
	//float z = 0.0f;
	particle.dir = Vector3f(x, y, z);
	particle.dir.normalize();

	pool.push_back(particle);
}

