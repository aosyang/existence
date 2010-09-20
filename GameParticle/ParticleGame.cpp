#include "ParticleGame.h"

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
CameraShakeEffect	g_CameraShake;

ParticleEmitter* emitter;
Billboard* bb;

void particleGame::StartGame()
{
	m_Scene = new SceneGraph;

	Renderer::Instance().SetAmbientColor(Color4f(0.7f, 0.7f, 0.7f));

	m_Camera = static_cast<Camera*>(m_Scene->CreateSceneObject("Camera"));
	m_Camera->SetPosition(Vector3f(1.0f, 0.0f, 5.0f));
	m_Camera->SetFarClippingDistance(1000.0f);

	m_AudioListener = static_cast<AudioListener*>(m_Scene->CreateSceneObject("AudioListener"));
	m_Camera->AttachChildObject(m_AudioListener, false);

	m_MatSmoke = MaterialManager::Instance().GetByName("smoke");
	//m_MatSmoke->SetTexture(Renderer::Instance().GetTexture("smoke"));
	//m_MatSmoke->SetLighting(false);
	//m_MatSmoke->SetDepthWriting(false);

	//m_MatSmoke->SetAlphaTest(true);
	//m_MatSmoke->SetAlphaRef(0.2f);
	//m_MatSmoke->GetTextureRenderState(0)->envMode = ENV_MODE_ADD;

	m_UIFps = EGUIManager::Instance().CreateTextUIControl();
	m_UIFps->SetWidth(640);

	//Mesh* mesh = MeshManager::Instance().GetByName("marcus");
	PrimitiveMesh* mesh = MeshManager::Instance().CreatePrimitiveMesh();
	mesh->CreateBox(1.0f);
	MeshObject* obj = static_cast<MeshObject*>(m_Scene->CreateSceneObject("MeshObject"));
	obj->SetMesh(mesh);

	ParticlePool* pool = static_cast<ParticlePool*>(m_Scene->CreateSceneObject("ParticlePool"));
	pool->SetMaterial(m_MatSmoke);

	for (int i=0; i<50; i++)
	{
		Particle p;
		p.m_Active = true;
		//p.m_Color = Color4f(0.0f, 0.0f, 0.0f, 0.5f);
		p.m_Color = Color4f(1.0f, 1.0f, 1.0f, 0.5f);
		p.scale = Math::Random(50.0f, 100.0f);
		p.spin = Math::Random(-Math::kPi, Math::kPi);
		p.spinInc = Math::Random(-0.1f, 0.1f);
		//p.m_ScreenScaleX = Math::Random(0.5f, 1.5f);
		//p.m_ScreenScaleY = Math::Random(0.5f, 1.5f);
		p.m_Position = Vector3f(Math::Random(-50.0f, 50.0f), Math::Random(0.0f, 10.0f), Math::Random(-50.0f, 50.0f));

		pool->AddParticle(p);
	}

	Material* matFlare = MaterialManager::Instance().GetByName("flare");
	//ITexture* tex_flare = Renderer::Instance().GetTexture("flare");
	//matFlare->SetTexture(tex_flare);
	//matFlare->SetLighting(false);
	//matFlare->SetDepthWriting(false);

	//// 混合模式 叠加
	//matFlare->GetTextureRenderState(0)->useBlending = true;
	//matFlare->GetTextureRenderState(0)->srcBlendFactor = BLEND_FACTOR_ONE;
	//matFlare->GetTextureRenderState(0)->dstBlendFactor = BLEND_FACTOR_ONE;
	////matFlare->GetTextureRenderState(0)->envMode = ENV_MODE_ADD;

	// 光晕
	bb = static_cast<Billboard*>(m_Scene->CreateSceneObject("Billboard"));
	bb->SetMaterial(matFlare);
	bb->SetScale(10.0f);
	bb->SetScreenSpaceScale(10.0f, 1.0f);
	bb->SetColor(Color4f(1.0f, 0.5f, 0.0f, 1.0f));
	//bb->SetRenderOrder(120);

	emitter = static_cast<ParticleEmitter*>(m_Scene->CreateSceneObject("ParticleEmitter"));
	emitter->SetInterval(50);
	emitter->SetMaterial(m_MatSmoke);
	emitter->SetEmitterShape(EMITTER_SHAPE_BOX);
	emitter->SetBoxRange(Vector3f(-1.0f, -1.0f, -1.0f), Vector3f(1.0f, 1.0f, 1.0f));
	//emitter->SetParticleInitStateFunc(ParticleState);
	emitter->SetPosition(Vector3f(0.0f, 150.0f, 0.0f));
	//emitter->SetParticleBehaviorFunc(&ParticleUpdate);
	emitter->SetParticleLifetime(5000);
	emitter->SetInitialScale(10.0f);
	emitter->SetScaleInc(20.0f);
	emitter->SetSpinVar(Math::kPi);
	emitter->SetSpinIncVar(0.5f);
	//m_Camera->AttachChildObject(emitter, true);
	emitter->AttachChildObject(bb);

	//Engine::Instance().ToggleDebugRender(true);

	//IAudioBuffer* buffer = Engine::Instance().AudioSystem()->GetAudioBuffer("down");
	//IAudioSource* source = Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));
	//source->SetLooping(true);
	//source->Play();

	//AudioSourceObject* sound = new AudioSourceObject();
	//sound->CreateAudioSource("down");
	//sound->SetLooping(true);
	//sound->Play();
	//sound->SetVelocityFactor(1.0f);
	//m_Scene->AddObject(sound);
	//emitter->AttachChildObject(sound);

	//Sleep(2000);
	//IAudioSource* source2 = Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(5.0f, 0.0f, 0.0f));
	//source2->SetLoop(true);
	//source2->Play();

	InputEventHandler<particleGame>* inputHandler =
		new InputEventHandler<particleGame>(this,
										    &particleGame::OnKeyPressed,
										    NULL,
										    &particleGame::OnMouseMoved);
	Input->SetEventHandler(inputHandler);
}

void particleGame::Shutdown()
{
	//ResourceManager<Material>::ResourceMap::iterator iter;
	//ResourceManager<Material>::ResourceMap map = ResourceManager<Material>::Instance().GetResourceMap();
	//for (iter=ResourceManager<Material>::Instance().GetResourceMap().begin();
	//	 iter!=ResourceManager<Material>::Instance().GetResourceMap().end();
	//	 iter++)
	//{
	//	iter->second->SaveToFile(iter->first + ".emt");
	//}


	SAFE_DELETE(m_Scene);
}

void particleGame::OnKeyPressed(KeyCode key)
{
	switch (key)
	{
	case KC_SPACE:
		g_CameraShake.Create(1.f, 20.0f, 2000);
		break;
	case KC_H:
		MeshManager::Instance().DumpToLog();
		TextureManager::Instance().DumpToLog();
		MaterialManager::Instance().DumpToLog();
		FontManager::Instance().DumpToLog();
		break;
	case KC_P:
		{
			static bool debugRender = false;
			debugRender = !debugRender;
			DebugRenderer::ToggleSceneObjectDebugRender(debugRender);
		}
		break;
	}
}

void particleGame::OnMouseMoved(int x, int y, int rel_x, int rel_y)
{
	// 按住鼠标右键调整视角
	if (Input->GetMouseButtonDown(MB_Right))
	{
		float x = -(float)rel_x / 5.0f;
		float y = -(float)rel_y / 5.0f;

		m_Camera->RotateLocal(x, y);
	}
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
		Renderer::Instance().SetProjectionMatrix(m_Camera->GetProjMatrix());
	}

}

void particleGame::Update(unsigned long deltaTime)
{
	//deltaTime *= 0.1f;
	float boost;
	String text;

	if (Input->GetKeyDown(KC_LSHIFT))
		boost = 4.0f;
	else
		boost = 1.0f;

	float forward = 0.0f;
	float right = 0.0f;

	if (Input->GetKeyDown(KC_W))
		forward += 0.1f * deltaTime / 10.0f * boost;
	if (Input->GetKeyDown(KC_S))
		forward += -0.1f * deltaTime / 10.0f * boost;
	if (Input->GetKeyDown(KC_A))
		right += -0.1f * deltaTime / 10.0f * boost;
	if (Input->GetKeyDown(KC_D))
		right += 0.1f * deltaTime / 10.0f * boost;

	forward += -0.1f * deltaTime / 10.0f * boost * Input->GetJoyStickAbs(2) / 0x7FFF;
	right += 0.1f * deltaTime / 10.0f * boost * Input->GetJoyStickAbs(3) / 0x7FFF;

	m_Camera->MoveLocal(forward, right, 0.0f);

	if (Input->GetKeyDown(KC_ESCAPE))
		Engine::Instance().SetQuitting(true);

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

		//float fCamDist = Renderer::Instance().GetFrustum()->SphereInFrustum(iter->bb->WorldTransform().GetPosition(), 5.0f);
		//if (fCamDist < fPercent)
		//{
		float fAplha  = max((float)iter->life, 0.0f) / 30000;
		iter->bb->SetColor(Color4f(1.0f, 1.0f, 1.0f, fAplha));
		//}
	}

	static float rot = 0.0f;
	rot += float(deltaTime) / 1000.0f;

	g_CameraShake.Update(deltaTime);
	Matrix4 camoffset = g_CameraShake.GetOffsetMatrix();
	m_Camera->SetViewOffsetMatrix(g_CameraShake.GetOffsetMatrix());

	emitter->SetPosition(Vector3f(sinf(rot) * 100.0f, 50.0f, cosf(rot) * 100.0f));
	bb->SetScale(sinf(rot * 20.0f) * 1.0f + 10.0f);

	m_Scene->UpdateScene(deltaTime);

	unsigned int fps = Engine::Instance().GetFPS();
	text.Format("FPS: %d", fps);
	//text.Format("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
	//	camoffset.mArray[0], camoffset.mArray[1], camoffset.mArray[2], camoffset.mArray[3],
	//	camoffset.mArray[4], camoffset.mArray[5], camoffset.mArray[6], camoffset.mArray[7],
	//	camoffset.mArray[8], camoffset.mArray[9], camoffset.mArray[10], camoffset.mArray[11],
	//	camoffset.mArray[12], camoffset.mArray[13], camoffset.mArray[14], camoffset.mArray[15]);
	//System::Instance().SetWindowTitle(buf);
	m_UIFps->SetText(text);
}

void particleGame::RenderScene()
{
	// 全屏渲染
	Renderer::Instance().SetViewport(0, 0, 0, 0);
	//Renderer::Instance().SetViewport(160, 120, 320, 240);

	// 设定渲染视点
	m_Scene->SetupRenderView(m_Camera);
	Renderer::Instance().BeginRender();
	m_Scene->RenderScene();

	// 使用BeginRender和EndRender方法，进行其他自定义渲染操作
	DebugRenderer::Instance().DrawLine(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(5000.0f, 0.0f, 0.0f), Color4f(0.0f, 1.0f, 1.0f));
	Renderer::Instance().EndRender();
}

void Emitparticle(const Vector3f& pos, SceneGraph* scene, particlePool& pool)
{
	particle particle;
	particle.bb = static_cast<Billboard*>(scene->CreateSceneObject("Billboard"));
	particle.bb->SetPosition(pos);
	particle.bb->SetMaterial(m_MatSmoke);
	particle.bb->SetColor(Color4f(1.0f, 0.5f, 0.5f, 1.0f));
	particle.rot = Math::Random(-Math::kPi, Math::kPi);
	particle.bb->SetZRotation(particle.rot);
	//particle.bb->SetScreenSpaceScale(1.2f, 0.8f);
	particle.life = 30000;
	particle.scale = Math::Random(0.5f, 1.5f);
	particle.scale_inc = Math::Random(0.05f, .1f);
	particle.rot_inc = Math::Random(-0.005f, 0.005f);
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

