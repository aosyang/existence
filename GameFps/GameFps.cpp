#include "GameFps.h"

bool toggleCollision = true;
bool toggleWireframe = false;
bool toggleGravity = true;
bool flareVisible = false;
unsigned long timeToHide = 0;
CameraShakeEffect	g_CameraShake;

//#define RENDER_TARGET_TEST

#ifdef RENDER_TARGET_TEST
IRenderTarget*	g_RenderTarget = NULL;
ITexture*		g_RTT = NULL;
#endif

GameFps::GameFps()
 : m_Scene(NULL),
   m_Camera(NULL),
   m_AudioListener(NULL),
   m_PlayerShadow(NULL),
   m_Sun(NULL),
   m_UIControl(NULL),
   m_UIFps(NULL)
{
	for (int i=0; i<MAX_DECAL_NUM; i++)
		m_Decals[i] = NULL;

	for (int i=0; i<MAX_SHELL_NUM; i++)
	{
		m_Shells[i].needUpdate = false;
		m_Shells[i].obj = NULL;
	}
}

GameFps::~GameFps()
{

}

void GameFps::StartGame()
{
	//Engine::Instance().ToggleDebugRender(true);


	m_Scene = new SceneGraph;
	//m_Scene->SetAmbientColor(Color4f(0.7f, 0.7f, 0.7f));
	renderer->SetAmbientColor(Color4f(1.f, 1.f, 1.f));

	renderer->SetClearColor(Color4f(0.0f, 0.5f, 0.5f));

	m_Camera = new Camera;
	m_Camera->SetFarClippingDistance(1000.0f);
	//m_Camera->SetPosition(Vector3f(1.0f, 0.0f, 5.0f));

	// 摄像机作为收听者
	m_AudioListener = new AudioListener();
	m_Scene->AddObject(m_AudioListener);
	//m_Camera->AttachChildObject(m_AudioListener, false);

	// 设置全局的方向光照
	m_Sun = new Light();
	m_Sun->SetLightType(LIGHT_TYPE_DIRECTIONAL);
	Vector3f sunDir = Vector3f(0.2f, 1.0f, 0.5f);
	sunDir.normalize();
	m_Sun->SetDirection(sunDir);
	m_Scene->AddObject(m_Sun);
	LightingManager::Instance().AddGlobalLight(m_Sun);
	//ShadowManager::Instance().SetLight(m_Sun);

	// 不添加到场景的摄像机将无法移动
	m_Scene->AddObject(m_Camera);

	m_UIControl = EGUIManager::Instance().CreateImageUIControl();

	ITexture* no_mat = renderer->GetTexture("Brick.jpg");
	unsigned char color[32 * 32 * 3] = { 0x0 };

	for (int i=0; i<32*32; i++)
	{
		color[i*3] = 0xff;// * i / (32 * 32);
		color[i*3+1] = 0xff;// * i / (32 * 32);
		color[i*3+2] = 0xff;// * i / (32 * 32);
	}

	no_mat->ModifyRectData(32, 32, 32, 32, &color);
	//unsigned int bpp = no_mat->GetBpp();

	Mesh* bspScene = ResourceManager<Mesh>::Instance().GetByName("scene.EMD");

	//BuildBspTreeFromMesh(&bsp, bspScene);

	m_ViewGun = new MeshObject();
	m_ViewGun->SetMesh(ResourceManager<Mesh>::Instance().GetByName("mp5.EMD"));
	m_ViewGun->CreateLightableObject();

	Material* matFlare = ResourceManager<Material>::Instance().GetByName("flare");
	//if (matFlare)
	//{
	//	matFlare->SetTexture(renderer->GetTexture("flare"));
	//	matFlare->GetTextureRenderState()->srcBlendFactor = BLEND_FACTOR_ONE;
	//	matFlare->GetTextureRenderState()->dstBlendFactor = BLEND_FACTOR_ONE;
	//	matFlare->SetDepthWriting(false);
	//}
	//matFlare->SaveToFile("flare.emt");

	Image cubeMap[6];
	cubeMap[0].LoadFromFile("../../../data/sky/snowlake_bk.tga");
	cubeMap[1].LoadFromFile("../../../data/sky/snowlake_ft.tga");
	cubeMap[2].LoadFromFile("../../../data/sky/snowlake_up.tga");
	cubeMap[3].LoadFromFile("../../../data/sky/snowlake_dn.tga");
	cubeMap[4].LoadFromFile("../../../data/sky/snowlake_rt.tga");
	cubeMap[5].LoadFromFile("../../../data/sky/snowlake_lf.tga");
	unsigned char* data[6];
	for (int i=0; i<6; i++)
		data[i] = cubeMap[i].GetData();
	renderer->BuildCubeTexture("cubeMap", cubeMap[0].GetWidth(), cubeMap[0].GetHeight(), cubeMap[0].GetBPP(), data);

	Material* matShell = ResourceManager<Material>::Instance().Create("goldshell");
	if (matShell)
	{
		matShell->SetTexture(renderer->GetTexture("goldshell"));
		matShell->SetTextureLayerEnabled(1, true);
		matShell->GetTextureRenderState(1)->texture = renderer->GetTexture("env_sphere");
		matShell->GetTextureRenderState(1)->envMode = ENV_MODE_ADD;
		matShell->GetTextureRenderState(1)->genMode = GEN_MODE_SPHERE;

		//matShell->SaveToFile("goldshell.emt", true);
	}

	Material* matCubeSky = ResourceManager<Material>::Instance().Create("cube_sky");
	matCubeSky->GetTextureRenderState(0)->texture = renderer->GetTexture("cubeMap");
	matCubeSky->GetTextureRenderState(0)->envMode = ENV_MODE_MODULATE;
	matCubeSky->GetTextureRenderState(0)->genMode = GEN_MODE_CUBE_MAP;

	//matCubeSky->SaveToFile("cube_sky.emt");

	//Mesh* mesh = ResourceManager<Mesh>::Instance().Create("skybox");
	//// 修改天空盒材质为replace
	//for (int i=0; i<6; i++)
	//{
	//	Material* mat = mesh->GetMaterial(i);
	//	mat->GetTextureRenderState(0)->envMode = ENV_MODE_REPLACE;
	//}

	m_Sky = new DistantViewObject();
	m_Sky->SetMesh(ResourceManager<Mesh>::Instance().GetByName("skybox.EMD"));
	//m_Sky->SetOffsetScale(Vector3f(0.1f, 0.1f, 0.1f));
	//m_Sky->SetOffsetScale(Vector3f(1.f, 1.f, 1.f));
	m_Scene->AddObject(m_Sky);

	String matName[] = { "lowerrec", "buttstock", "handle", "barrel", "clip", "forearm", "rearsight" };
	for (int i=0; i<7; i++)
	{
		Material* gunMat = ResourceManager<Material>::Instance().Create(matName[i]);
		gunMat->SetTexture(renderer->GetTexture(matName[i]));
		gunMat->SetTextureLayerEnabled(1, true);
		gunMat->GetTextureRenderState(1)->texture = renderer->GetTexture("cubeMap");
		gunMat->GetTextureRenderState(1)->envMode = ENV_MODE_MODULATE;
		gunMat->GetTextureRenderState(1)->genMode = GEN_MODE_CUBE_MAP;
		//gunMat->SaveToFile(String(matName[i]) + ".emt");
	}

	Material* matSphere = ResourceManager<Material>::Instance().Create("metal_sphere");
	//matSphere->SetTextureLayerEnabled(1, true);
	//matSphere->GetTextureRenderState(0)->texture = renderer->GetTexture("no_material");
	//matSphere->GetTextureRenderState(1)->texture = renderer->GetTexture("cubeMap");
	//matSphere->GetTextureRenderState(1)->envMode = ENV_MODE_MODULATE;
	//matSphere->GetTextureRenderState(1)->genMode = GEN_MODE_CUBE_MAP;
	IGpuProgram* vprogram = renderer->LoadGpuProgram("./base_vp.cg", "main", GPU_VERTEX_PROGRAM);
	IGpuProgram* fprogram = renderer->LoadGpuProgram("./base_fp.cg", "main", GPU_FRAGMENT_PROGRAM);
	matSphere->SetVertexProgram(vprogram);
	matSphere->SetFragmentProgram(fprogram);
	//matSphere->SetTextureLayerEnabled(1, true);
	//matSphere->GetTextureRenderState(0)->texture = renderer->GetTexture("no_material");
	//matSphere->GetTextureRenderState(1)->texture = renderer->GetTexture("Brick");

	MeshObject* sphere = new MeshObject();
	sphere->SetMesh(ResourceManager<Mesh>::Instance().GetByName("sphere.EMD"));
	sphere->SetMaterial(matSphere, 0);
	m_Scene->AddObject(sphere);
	sphere->SetPosition(Vector3f(0.0f, 30.0f, 0.0f));

	sphere->CreateLightableObject();


	m_ViewGunFlare = new MeshObject();
	m_ViewGunFlare->SetMesh(ResourceManager<Mesh>::Instance().GetByName("gunflare.EMD"));
	m_ViewGunFlare->SetVisible(false);

	// TODO: AddObject和AttachChildObject二者只需要一个？
	m_Scene->AddObject(m_ViewGun);
	m_Camera->AttachChildObject(m_ViewGun, false);
	m_Camera->AttachChildObject(m_ViewGunFlare, false);

	m_BspScene = new BspObject();
	m_BspScene->SetMesh(bspScene);
	m_Scene->AddObject(m_BspScene);
	m_BspScene->SetRenderOrder(90);
	//LightingManager::Instance().AddLightableObject(&m_BspScene);

	//LightingManager::Instance().AddLight(m_Light);
	//LightingManager::Instance().AddLightableObject(&m_ViewGun);

	// FIXME: 开枪时候纹理会闪烁
	Material* matDecal = ResourceManager<Material>::Instance().Create("MatDecal");
	//matDecal->GetTextureRenderState()->srcBlendFactor = BLEND_FACTOR_ZERO;
	//matDecal->GetTextureRenderState()->dstBlendFactor = BLEND_FACTOR_SRC_ALPHA_SATURATE;
	matDecal->SetAlphaTest(true);
	matDecal->SetDepthWriting(false);
	//matDecal->SetDepthTest(false);
	matDecal->SetAlphaRef(0.01f);
	matDecal->SetTexture(renderer->GetTexture("bullethole"));
	//matDecal->SaveToFile("MatDecal.emt");

	m_PlayerShadow = new Decal();
	m_PlayerShadow->SetSize(10.0f);
	m_PlayerShadow->SetMaterial(matDecal);
	m_PlayerShadow->SetRenderOrder(95);

	m_Scene->AddObject(m_PlayerShadow);
	m_Scene->AddObject(m_ViewGunFlare);

	System::Instance().ToggleMouseCursor(false);

	m_UIFps = EGUIManager::Instance().CreateTextUIControl();
	//m_UIFps->SetFont("DefaultFont");
	m_UIFps->SetWidth(640);


	//ParticlePool* pool = new ParticlePool;
	//m_Scene->AddObject(pool);

	//for (int i=0; i<50; i++)
	//{
	//	Particle p;
	//	p.m_Active = true;
	//	
	//	p.m_Position = Vector3f(Math::Random(-10.0f, 10.0f), Math::Random(-10.0f, 10.0f), Math::Random(-10.0f, 10.0f));

	//	pool->AddParticle(p);
	//}


#ifdef RENDER_TARGET_TEST
	m_ScreenQuadScene = new SceneGraph;

	g_RTT = renderer->BuildTexture("RTT", 640, 480, 32, NULL);
	g_RenderTarget = renderer->CreateRenderTarget();
	g_RenderTarget->SetTexture(g_RTT);
	g_RenderTarget->GenerateMipmap();

	Material* matScreenQuad = ResourceManager<Material>::Instance().Create("screen quad");
	IGpuProgram* quadvs = renderer->LoadGpuProgram("ScreenQuad.cg", "main", GPU_VERTEX_PROGRAM);
	IGpuProgram* quadfs = renderer->LoadGpuProgram("ScreenQuadFP.cg", "main", GPU_FRAGMENT_PROGRAM);
	matScreenQuad->SetVertexProgram(quadvs);
	matScreenQuad->SetFragmentProgram(quadfs);
	matScreenQuad->SetTexture(g_RTT);
	matScreenQuad->SetDepthWriting(false);

	Mesh* plane = ResourceManager<Mesh>::Instance().Create("plane");
	plane->CreatePositiveZPlane(1.0f);
	MeshObject* screenQuad = new MeshObject();
	screenQuad->SetMesh(plane);

	m_ScreenQuadScene->AddObject(screenQuad);
	screenQuad->SetMaterial(matScreenQuad, 0);
	screenQuad->SetFrustumCulling(false);
#endif
}

void GameFps::Shutdown()
{


	//for (int i=0; i<MAX_DECAL_NUM; i++)
	//	delete m_Decals[i];

	//SAFE_DELETE(m_PlayerShadow);
	//delete m_CharacterMesh;
	//SAFE_DELETE(m_Sun);
	//SAFE_DELETE(m_AudioListener);
	//SAFE_DELETE(m_Camera);

	delete m_Scene;
}

void GameFps::OnKeyPressed(unsigned int key)
{
	switch(key)
	{
	case KC_C:
		toggleCollision = !toggleCollision;
		break;
	case KC_G:
		toggleGravity = !toggleGravity;
		break;
	case KC_F:
		toggleWireframe = !toggleWireframe;
		break;
	case KC_P:
		Engine::Instance().ToggleDebugRender(!Engine::Instance().GetDebugRender());
		break;
	case KC_SPACE:
		g_CameraShake.Create(0.5f, 30.0f, 2000);
		break;
	}
}

bool GameFps::OnNotifyQuitting()
{
	return true;
}

void GameFps::OnResizeWindow(unsigned int width, unsigned int height)
{
	// 窗口缩放以后要更新摄像机的纵横比...
	float aspect = (float)width / height;
	if (m_Camera)
	{
		m_Camera->SetAspect(aspect);
		
		// ...以及投影矩阵
		renderer->ProjectionMatrix() = m_Camera->GetProjMatrix();
	}

}

float angle = 0.0f;

void GameFps::Update(unsigned long deltaTime)
{
	if (deltaTime>1000) deltaTime = 1000;

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

	//if (toggleCollision && toggleGravity)
	//{
	//	Vector3f rg = m_Camera->WorldTransform().GetRightVector();
	//	Vector3f fw = CrossProduct(Vector3f(0.0f, 1.0f, 0.0f), rg);
	//	Vector3f transVec = fw * forward + rg * right;

	//	m_Camera->TranslateLocal(transVec);
	//}
	//else
		m_Camera->MoveLocal(forward, right, 0.0f);

	if (Input::Instance().GetKeyDown(KC_ESCAPE))
		Engine::Instance().SetQuitting(true);

	// 按住鼠标右键调整视角
	//if (Input::Instance().GetMouseButtonDown(MB_Right))
	{
		float x = -(float)Input::Instance().GetMouseRelX() / 5.0f;
		float y = -(float)Input::Instance().GetMouseRelY() / 5.0f;

		System::Instance().CenterMouseCursor();

		m_Camera->RotateLocal(x, y);
	}

	// 使用手柄控制当前摄像机
	float x = -(float)Input::Instance().GetJoyStickAbs(1) / 0x7FFF * 0.1f * deltaTime;
	float y = -(float)Input::Instance().GetJoyStickAbs(0) / 0x7FFF * 0.1f * deltaTime;
	m_Camera->RotateLocal(x, y);


	Vector3f campos_old = m_Camera->WorldTransform().GetPosition();

	// 摄像机震动更新
	g_CameraShake.Update(deltaTime);
	Matrix4 camoffset = g_CameraShake.GetOffsetMatrix();
	m_Camera->SetViewOffsetMatrix(g_CameraShake.GetOffsetMatrix());

	// ******************************************* 更新 *******************************************
	m_Scene->UpdateScene(deltaTime);
	// ******************************************* 更新 *******************************************

	// 更新弹壳运动
	for (int i=0; i<MAX_SHELL_NUM; i++)
	{
		if (!m_Shells[i].obj || !m_Shells[i].needUpdate) continue;
		Vector3f shellPos = m_Shells[i].obj->WorldTransform().GetPosition();

		//shellPos += m_Shells[i].vel;

		Vector3f point;
		BspTriangle* bsptri;
		//m_Shells[i].vel += Vector3f(0.0f, -.001f * deltaTime, 0.0f);
		m_Shells[i].vel += Vector3f(0.0f, -.001f * deltaTime, 0.0f);

		if (m_BspScene->Intersects(Ray(shellPos, shellPos + m_Shells[i].vel), &point, &bsptri))
		{
			// 发生碰撞

			// 反弹方向计算
			shellPos = point;
			Vector3f triNormal = m_BspScene->WorldTransform() * bsptri->normal;

			float dot = m_Shells[i].vel * triNormal;
			m_Shells[i].vel += triNormal * (-2 * dot);

			// 随机化反弹方向
			m_Shells[i].vel += Vector3f(Math::Random(-0.1f, 0.1f), Math::Random(-0.1f, 0.1f), Math::Random(-0.1f, 0.1f)) * m_Shells[i].vel.SquaredLength() * 1.0f;

			// 反弹衰减
			//m_Shells[i].vel *= 0.5f;
			m_Shells[i].vel *= Math::Random(.3f, 0.5f);

			shellPos += m_Shells[i].vel * 0.001f;

			//m_Shells[i].vel = -m_Shells[i].vel;

			// 停止速度阈值
			if (m_Shells[i].vel.SquaredLength()<0.0001f) m_Shells[i].needUpdate = false;

			// 反弹后重新给一个旋转速度，但要根据当前速度衰减
			m_Shells[i].yawSpeed = Math::Random(-0.5f, 0.5f) * m_Shells[i].vel.Length();

			//// 旋转速度
			//if (m_Shells[i].vel.SquaredLength()<0.01f)
			//{
			//	m_Shells[i].yawSpeed = 0.0f;
			//}

		}
		else
		{
			shellPos += m_Shells[i].vel;
		}

		m_Shells[i].obj->SetPosition(shellPos);

		m_Shells[i].yawStart += m_Shells[i].yawSpeed * 0.05f * deltaTime;
		Quaternion rot;
		rot.CreateFromLocalAxisAngle(Vector3f(0.0f, 1.0f, 0.0f), m_Shells[i].yawStart);
		m_Shells[i].obj->SetRotation(rot);

	}

	Vector3f campos = m_Camera->WorldTransform().GetPosition();

	bool pushed = false;

	// 摄像机碰撞检测
	if (toggleCollision)
	{
		// 从摄像机所在位置向下作一条射线，如果触到墙壁则不应用重力
		Vector3f cam_hit;
		//if (toggleGravity)
		//	campos += Vector3f(0.0f, -0.05f * deltaTime, 0.0f);

		//BspTriangle* cam_hit_tri;
		//if (!m_BspScene->Intersects(Ray(campos, campos + Vector3f(0.0f, -10.0f, 0.0f)), &cam_hit, &cam_hit_tri))
		//{

		//	// 重力
		//	//campos = cam_hit + cam_hit_tri->normal;
		//}
		//else
		//{
		//	campos = cam_hit + Vector3f(0.0f, 10.0f, 0.0f);
		//}

		Vector3f newcampos;

		pushed = m_BspScene->PushSphere(campos, newcampos, 5.0f);

		if (pushed) m_Camera->SetPosition(newcampos);
		else m_Camera->SetPosition(campos);

		//// 更新阴影位置
		//if (m_BspScene->Intersects(Ray(campos, campos + Vector3f(0.0f, -100.0f, 0.0f)), &cam_hit, &cam_hit_tri))
		//{
		//	Vector3f shadowNormal = m_BspScene->WorldTransform() * cam_hit_tri->normal;
		//	m_PlayerShadow->SetPosition(cam_hit + shadowNormal * 0.005f);
		//	m_PlayerShadow->SetDirection(shadowNormal);
		//}
	}
	
	// TODO: 临时在此更新
	// 为了防止摄像机在碰到墙壁时发生弹跳，应当先计算Push的结果，再对摄像机位置进行更新
	// 这里又进行了一次更新，造成了额外的更新开销，但效果正确，需要修改
	m_Scene->UpdateScene(deltaTime);

	if (Input::Instance().GetMouseButtonDown(MB_Left))
	{
		FireWeapon(deltaTime);
	}

	// TODO: 使用SoundObject，防止找不到音频引发的问题
	//IAudioBuffer* fire = Engine::Instance().AudioSystem()->GetAudioBuffer("fire1");
	//if (fire)
	//{
	//	Engine::Instance().AudioSystem()->CreateSourceInstance(fire, m_Camera->WorldTransform().GetPosition());
	//}

	if (flareVisible && timeToHide>=50)
	{
		flareVisible = false;
		m_ViewGunFlare->SetVisible(false);
		timeToHide = 0;
	}
	else
		timeToHide += deltaTime;


	//m_BspScene->SetRotation(Matrix3::BuildRollRotationMatrix(angle));
	//angle += (float)deltaTime / 10000.0f;
	//Vector3f v1(0.0f, 10.0f, 0.0f);
	//Vector3f v2(0.0f, 5.0f, 1.0f);
	//Vector3f v3(0.0f, 1.0f, 0.0f);
	//v3 *= v1 * v2 / sqrt(v2.SquaredLength());
	////v3.normalize();

	//Vector3f offset(0.0f, 100.0f, 100.0f);

	//m_Scene->DrawLine(offset, offset + v1);
	//m_Scene->DrawLine(offset, offset + v2);
	//m_Scene->DrawLine(offset + Vector3f(1.0f, 0.0f, 0.0f), offset + Vector3f(1.0f, 0.0f, 0.0f) + v3);

	//m_BspScene->SetRotation(Matrix3::BuildRollRotationMatrix(angle));
	//angle += (float)deltaTime / 10000.0f;

	unsigned int count = Engine::Instance().GetRenderBatchCount();

	// 显示FPS
	unsigned int fps = Engine::Instance().GetFPS();

	sprintf(buf, "FPS: %d\npushed: %d\nBatch count: %d", fps, pushed, count);
	m_UIFps->SetText(buf);

	// 更新光照
	LightingManager::Instance().Update();
}

void GameFps::RenderScene()
{
	RenderView rv;

	rv.position = m_Camera->WorldTransform().GetPosition();
	rv.viewMatrix = m_Camera->GetViewMatrix();
	rv.projMatrix = m_Camera->GetProjMatrix();
	rv.frustum = m_Camera->GetFrustum();

	renderer->SetViewport(0, 0, 0, 0);

#ifdef RENDER_TARGET_TEST
	renderer->SetRenderTarget(g_RenderTarget);
#endif

	// 设定渲染视点
	m_Scene->SetupRenderView(rv);
	m_Scene->RenderScene();

#ifdef RENDER_TARGET_TEST
	renderer->SetRenderTarget(NULL);

	m_ScreenQuadScene->RenderScene();
#endif

	if (toggleWireframe)
	{
		renderer->BeginRender();

		vector<BspTriangle*> triList;
		m_BspScene->TraverseTree(&triList, m_Camera->WorldTransform().GetPosition());

		for (vector<BspTriangle*>::iterator iter=triList.begin(); iter!=triList.end(); iter++)
		{
			renderer->RenderLine((*iter)->vertices[0], (*iter)->vertices[1], Color4f(0.5f, 0.5f, 1.0f));
			renderer->RenderLine((*iter)->vertices[1], (*iter)->vertices[2], Color4f(0.5f, 0.5f, 1.0f));
			renderer->RenderLine((*iter)->vertices[2], (*iter)->vertices[0], Color4f(0.5f, 0.5f, 1.0f));
		}

		renderer->EndRender();
	}

}


void GameFps::FireWeapon(unsigned long deltaTime)
{
	static int timeToFire;
	timeToFire += deltaTime;

	if (timeToFire<100) return;
	
	timeToFire = 0;
	IAudioBuffer* fire = Engine::Instance().AudioSystem()->GetAudioBuffer("fire1.wav");
	if (fire)
	{
		Engine::Instance().AudioSystem()->CreateSourceInstance(fire, m_Camera->WorldTransform().GetPosition());
	}

	Ray los = m_Camera->GetCameratRay(Math::Random(0.48f, 0.52f), Math::Random(0.48f, 0.52f));
	Vector3f hit_pos;
	BspTriangle* tri;
	bool los_col = m_BspScene->Intersects(los, &hit_pos, &tri);

	if (los_col)
	{
		static int decalIndex;
		decalIndex %= MAX_DECAL_NUM;
		if (!m_Decals[decalIndex])
		{
			m_Decals[decalIndex] = new Decal;
			m_Decals[decalIndex]->SetMaterial(ResourceManager<Material>::Instance().GetByName("MatDecal"));
			m_Scene->AddObject(m_Decals[decalIndex]);
			m_BspScene->AttachChildObject(m_Decals[decalIndex]);

			// 位于场景之后渲染
			m_Decals[decalIndex]->SetRenderOrder(93);
		}
		// 浮起一些距离，防止z-fighting
		hit_pos = m_BspScene->WorldTransform().GetInverseMatrix() * hit_pos;
		m_Decals[decalIndex]->SetPosition(hit_pos + tri->normal * 0.01f);
		m_Decals[decalIndex]->SetDirection(/*m_BspScene->WorldTransform() * */tri->normal);
		//m_Decals[decalIndex]->Transform() *= m_BspScene->WorldTransform().GetInverseMatrix();


		decalIndex++;
	}

	EjectShell(m_Camera->WorldTransform().GetPosition() + m_Camera->WorldTransform().GetForwardVector() * 2.f,
			   -m_Camera->WorldTransform().GetRightVector());
			   //Vector3f(0.0f, 1.0f, 0.0f));

	m_ViewGunFlare->SetVisible(true);
	flareVisible = true;
	timeToHide = 0;
}

void GameFps::EjectShell(const Vector3f& pos, const Vector3f& dir)
{
	//Vector3f pos = m_Camera->WorldTransform().GetPosition();
	////static Vector3f pos(0.0f, 20.0f, 0.0f);
	//Vector3f left = -m_Camera->WorldTransform().GetRightVector();

	static int shellIndex;
	shellIndex %= MAX_SHELL_NUM;

	if (!m_Shells[shellIndex].obj)
	{
		m_Shells[shellIndex].obj = new MeshObject();
		m_Shells[shellIndex].obj->SetMesh(ResourceManager<Mesh>::Instance().GetByName("shell.EMD"));
		m_Scene->AddObject(m_Shells[shellIndex].obj);

		m_Shells[shellIndex].obj->CreateLightableObject();
	}

	m_Shells[shellIndex].obj->SetPosition(pos);

	// 弹壳空中旋转速度
	m_Shells[shellIndex].yawStart = Math::Random(0.0f, 2*Math::kPi);
	m_Shells[shellIndex].yawSpeed = Math::Random(-0.5f, 0.5f);
	Quaternion rot;
	rot.CreateFromLocalAxisAngle(Vector3f(0.0f, 1.0f, 0.0f), m_Shells[shellIndex].yawStart);
	m_Shells[shellIndex].obj->SetRotation(rot);

	// 初速度，给定随机范围
	//m_Shells[shellIndex].vel = (left + Vector3f(Math::Random(-0.1f, 0.1f), Math::Random(-0.1f, 0.1f), Math::Random(-0.1f, 0.1f))) * 0.05f;
	m_Shells[shellIndex].vel = (dir + Vector3f(Math::Random(-0.5f, 0.5f), Math::Random(-0.5f, 0.5f), Math::Random(-0.5f, 0.5f))) * Math::Random(0.05f, 0.1f);
	m_Shells[shellIndex].needUpdate = true;
	shellIndex++;

}

