#include "GameGrid.h"
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

GameGrid::GameGrid()
: m_Scene(NULL),
  m_Camera(NULL),
  m_UIFps(NULL),
  m_Sun(NULL),

  m_MeshPlane(NULL),
  m_BasePlane(NULL),

  m_MeshBox(NULL), m_BoxMaterial(NULL), m_IntersectionObject(NULL),
  m_Rotation(0.0f),
  m_TestRun(false),
  m_Mode(MODE_ADD),
  m_ModeName("Add"),
  m_Layer(0)
{

}

GameGrid::~GameGrid()
{
}

void GameGrid::StartGame()
{
	//Renderer::Instance().SetClearColor(Color4f(0.5f, 1.0f, 1.0f));
	m_Scene = new SceneGraph;

	// 创建摄像机
	m_Camera = FACTORY_CREATE(m_Scene, Camera);
	m_Camera->SetPosition(Vector3f(1.0f, 0.0f, 5.0f));
	m_Camera->SetFarClippingDistance(2000.0f);

	Renderer::Instance().SetAmbientColor(Color4f(0.7f, 0.7f, 0.7f));

	// 设置默认的全局方向光
	m_Sun = FACTORY_CREATE(m_Scene, Light);
	m_Sun->SetLightType(LIGHT_TYPE_DIRECTIONAL);
	Vector3f sunDir = Vector3f(0.2f, 1.0f, 0.5f);
	sunDir.normalize();
	m_Sun->SetDirection(sunDir);
	LightingManager::Instance().AddGlobalLight(m_Sun);

	// 构建基准平面
	m_MeshPlane = MeshManager::Instance().CreatePrimitiveMesh();
	m_MeshPlane->CreatePositiveYPlane(WORLD_SIZE);

	m_BasePlane = FACTORY_CREATE(m_Scene, MeshObject);
	m_BasePlane->SetMesh(m_MeshPlane);

	// 创建盒子
	m_MeshBox = MeshManager::Instance().CreatePrimitiveMesh();
	m_MeshBox->CreateBox(1.0f);

	m_BoxMaterial = MaterialManager::Instance().Create();
	BaseTexture* gridTex = TextureManager::Instance().GetByName("grid_tex.bmp");
	m_BoxMaterial->SetTexture(gridTex);
	//m_BoxMaterial->SetDepthWriting(false);
	//m_BoxMaterial->SetDepthTest(false);
	//m_BoxMaterial->SetEmissive(Color4f(0.6f, 0.6f, 0.6f));

	BaseMesh* marker = MeshManager::Instance().GetByName("marker.EMD");
	// BaseMesh* marker = MeshManager::Instance().GetByName("Hammer.emd");

	// Skeleton* skel = SkeletonManager::Instance().GetByName("Hammer");
	// skel->Trigger();
	
	Material* matMarkerStart = MaterialManager::Instance().Create();
	matMarkerStart->SetDiffuse(Color4f(0.0f, 1.0f, 0.0f));
	matMarkerStart->SetEmissive(Color4f(0.0f, 0.5f, 0.0f));
	matMarkerStart->SetTexture(TextureManager::Instance().GetByName("blank.bmp"));
	Material* matMarkerGoal = MaterialManager::Instance().Create();
	matMarkerGoal->SetDiffuse(Color4f(1.0f, 0.0f, 0.0f));
	matMarkerGoal->SetEmissive(Color4f(0.5f, 0.0f, 0.0f));
	matMarkerGoal->SetTexture(TextureManager::Instance().GetByName("blank.bmp"));

	m_MarkerStart = FACTORY_CREATE(m_Scene, MeshObject);
	m_MarkerStart->SetMesh(marker);
	m_MarkerStart->SetMaterial(matMarkerStart, 0);
	//m_MarkerStart->CreateLightableObject();
	m_MarkerGoal = FACTORY_CREATE(m_Scene, MeshObject);
	m_MarkerGoal->SetMesh(marker);
	m_MarkerGoal->SetMaterial(matMarkerGoal, 0);
	//m_MarkerGoal->CreateLightableObject();

	//m_IntersectionObject = FACTORY_CREATE(m_Scene, MeshObject);
	//m_IntersectionObject->SetMesh(m_MeshBox);
	//m_IntersectionObject->SetMaterial(m_BoxMaterial);
	//m_Scene->AddObject(m_IntersectionObject);

	m_UIFps = EGUIManager::Instance().CreateTextUIControl();
	m_UIFps->SetWidth(640);

	for (int x=0; x<WORLD_SIZE; x++)
		for (int y=0; y<WORLD_SIZE; y++)
			for (int z=0; z<WORLD_SIZE; z++)
			{
				m_World[x][y][z].pos = Point3(x, y, z);
			}

	InputEventHandler<GameGrid>* inputHandler =
		new InputEventHandler<GameGrid>(this,
										&GameGrid::OnKeyPressed,
										NULL,
										&GameGrid::OnMouseMoved);
	Input->SetEventHandler(inputHandler);
}


void GameGrid::Shutdown()
{
	SAFE_DELETE(m_Scene);
}

bool GameGrid::OnNotifyQuitting()
{
	return true;
}

void GameGrid::OnKeyPressed(KeyCode key)
{
	switch(key)
	{
	case KC_P:
		{
			static bool debugRender = false;
			debugRender = !debugRender;
			DebugRenderer::ToggleSceneObjectDebugRender(debugRender);
		}
		break;
	case KC_SPACE:
		m_Mode++;
		if (m_Mode >= NUM_MODE)
			m_Mode -= NUM_MODE;
		switch(m_Mode)
		{
		case MODE_ADD:
			m_ModeName = "Add";
			break;
		case MODE_REMOVE:
			m_ModeName = "Remove";
			break;
		//case MODE_SHOW_NEIGHBOUR:
		//	m_ModeName = "Debug show neighbour";
		//	break;
		case MODE_SET_START:
			m_ModeName = "Set start point";
			break;
		case MODE_SET_GOAL:
			m_ModeName = "Set goal";
			break;
		default:
			m_ModeName = "Unknown mode";
			break;
		}
		break;
	case KC_Q:
		{
			int n = 1;
			if (Input->GetKeyDown(KC_LCONTROL))
				n = 10;
			m_Layer = Math::Clamp<int>(0, WORLD_SIZE-1, m_Layer+n);
			m_BasePlane->SetPosition(Vector3f(0.0f, m_Layer - 0.05f, 0.0f));
		}
		break;
	case KC_E:
		{
			int n = 1;
			if (Input->GetKeyDown(KC_LCONTROL))
				n = 10;
			m_Layer = Math::Clamp<int>(0, WORLD_SIZE-1, m_Layer-n);
			m_BasePlane->SetPosition(Vector3f(0.0f, m_Layer - 0.05f, 0.0f));
		}
		break;
	//case KC_B:
	//	{
	//		for (int x=0; x<WORLD_SIZE; x++)
	//			for (int y=0; y<WORLD_SIZE; y++)
	//				for (int z=0; z<WORLD_SIZE; z++)
	//				{
	//					BuildLinkingPath(Point3(x, y, z));
	//				}
	//	}
	//	break;
	case KC_F5:
		SaveScene("data.txt");
		break;
	case KC_F7:
		LoadScene("data.txt");
		break;
	case KC_RETURN:
		// Whatever, build path first...
		BuildLinkingPath();
		m_LastPos = m_MarkerStart->WorldTransform().GetPosition();
		m_TimeToRun = 0;
		PathFinding(m_Start, m_Goal);
		break;
	case KC_H:
		SkeletonManager::Instance().DumpToLog();
		MeshManager::Instance().DumpToLog();
		TextureManager::Instance().DumpToLog();
		MaterialManager::Instance().DumpToLog();
		FontManager::Instance().DumpToLog();
	default:
		break;
	}
}

void GameGrid::OnMouseMoved(int x, int y, int rel_x, int rel_y)
{
	// 按住鼠标右键调整视角
	if (Input->GetMouseButtonDown(MB_Right))
	{
		float x = -(float)rel_x / 5.0f;
		float y = -(float)rel_y / 5.0f;

		m_Camera->RotateLocal(x, y);
	}
}

void GameGrid::OnResizeWindow(unsigned int width, unsigned int height)
{
	float aspect = (float)width / height;
	if (m_Camera)
	{
		m_Camera->SetAspect(aspect);
		Renderer::Instance().SetProjectionMatrix(m_Camera->GetProjMatrix());
	}

}

void GameGrid::Update(unsigned long deltaTime)
{
	char buf[1024] = "\0";

	// Base camera movements
	float boost;

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

	m_Camera->MoveLocal(forward, right, 0.0f);

	if (Input->GetKeyDown(KC_ESCAPE))
		Engine::Instance().SetQuitting(true);

	if (Input->GetMouseButtonDown(MB_Left))
	{
		switch (m_Mode)
		{
		case MODE_ADD:
			{
				Vector3f p;

				if (GetIntersectionPoint(p))
				{
					int x = (int)ceil(p.x);
					int z = (int)ceil(p.z);

					x += WORLD_SIZE / 2 - 1;
					z += WORLD_SIZE / 2 - 1;
					AddBox(Point3(x, m_Layer, z));
				}
			}
			break;
		case MODE_REMOVE:
			{
				Vector3f p;

				if (GetIntersectionPoint(p))
				{
					int x = (int)ceil(p.x);
					int z = (int)ceil(p.z);

					x += WORLD_SIZE / 2 - 1;
					z += WORLD_SIZE / 2 - 1;
					RemoveBox(Point3(x, m_Layer, z));
				}
			}
			break;

		//case MODE_SHOW_NEIGHBOUR:
		//	{
		//		Vector3f p;

		//		if (GetIntersectionPoint(p))
		//		{
		//			int x = (int)ceil(p.x);
		//			int z = (int)ceil(p.z);

		//			x += WORLD_SIZE / 2 - 1;
		//			z += WORLD_SIZE / 2 - 1;
		//			ShowNeighbour(Point3(x, m_Layer, z));
		//		}
		//	}
		//	break;
		case MODE_SET_START:
			{
				Vector3f p;

				if (GetIntersectionPoint(p))
				{
					int x = (int)ceil(p.x);
					int z = (int)ceil(p.z);

					x += WORLD_SIZE / 2 - 1;
					z += WORLD_SIZE / 2 - 1;
					m_Start = Point3(x, m_Layer, z);

					p.x = ceil(p.x) - 0.5f;
					p.z = ceil(p.z) - 0.5f;
					m_MarkerStart->SetPosition(p + Vector3f(0.0f, 0.5f, 0.0f));
				}
			}
			break;
		case MODE_SET_GOAL:
			{
				Vector3f p;

				if (GetIntersectionPoint(p))
				{
					int x = (int)ceil(p.x);
					int z = (int)ceil(p.z);

					x += WORLD_SIZE / 2 - 1;
					z += WORLD_SIZE / 2 - 1;
					m_Goal = Point3(x, m_Layer, z);

					p.x = ceil(p.x) - 0.5f;
					p.z = ceil(p.z) - 0.5f;
					m_MarkerGoal->SetPosition(p + Vector3f(0.0f, 0.5f, 0.0f));
				}
			}
			break;
		default:
			break;
		}
	}

	MarkerGo(deltaTime);
	m_Rotation+=(float)deltaTime/1000;
	m_MarkerStart->SetRotation(Matrix3::BuildYawRotationMatrix(m_Rotation));
	m_MarkerGoal->SetRotation(Matrix3::BuildYawRotationMatrix(-m_Rotation));

	m_Scene->UpdateScene(deltaTime);

	unsigned int fps = Engine::Instance().GetFPS();
	String textFPS;
	textFPS.Format("FPS: %d\nMode: %s\n%d\n%d", fps, m_ModeName.Data(), Input->GetMousePosX(), Input->GetMousePosY());

	m_UIFps->SetText(textFPS);

	LightingManager::Instance().Update();
}

void GameGrid::RenderScene()
{

	// 全屏渲染
	Renderer::Instance().SetViewport(0, 0, 0, 0);
	//Renderer::Instance().SetViewport(160, 120, 320, 240);

	// 设定渲染视点
	m_Scene->SetupRenderView(m_Camera);
	Renderer::Instance().BeginRender();
	m_Scene->RenderScene();
	Renderer::Instance().EndRender();
}

// 射线与基准平面求交
bool GameGrid::GetIntersectionPoint(Vector3f& point)
{
	RenderWindowParam* param = System::Instance().GetRenderWindowParameters();

	unsigned int width = param->width;
	unsigned int height = param->height;

	float x = (float)Input->GetMousePosX() / width;
	float y = (float)Input->GetMousePosY() / height;


	Ray ray = m_Camera->GetCameraRay(x, y);

	float d;
	Vector3f normal;

	return (m_BasePlane->RayPicking(ray, point, normal, d));
}

// 添加一个盒子
void GameGrid::AddBox(const Point3& pos)
{
	// 格子被占据，不添加
	if (!m_World[pos.x][pos.y][pos.z].obj)
	{
		Vector3f p((float)pos.x - 0.5f - WORLD_SIZE / 2 + 1, (float)pos.y + 0.5f, (float)pos.z - 0.5f - WORLD_SIZE / 2 + 1);

		WorldGrid* grid = &m_World[pos.x][pos.y][pos.z];
		//grid->pos = pos;
		MeshObject* obj = FACTORY_CREATE(m_Scene, MeshObject);
		obj->SetMesh(m_MeshBox);
		obj->SetPosition(p);
		obj->SetMaterial(m_BoxMaterial, 0);

		//obj->CreateLightableObject();

		grid->obj = obj;
	}

}

// 删除一个盒子
void GameGrid::RemoveBox(const Point3& pos)
{
	Vector3f p((float)pos.x - 0.5f - WORLD_SIZE / 2 + 1, (float)pos.y + 0.5f, (float)pos.z - 0.5f - WORLD_SIZE / 2 + 1);

	// 格子为空，不删除
	if (m_World[pos.x][pos.y][pos.z].obj)
	{
		//LightingManager::Instance().RemoveLightableObject(m_World[pos.x][pos.y][pos.z].obj);
		m_Scene->RemoveObject(m_World[pos.x][pos.y][pos.z].obj);
		m_World[pos.x][pos.y][pos.z].obj = NULL;
	}

}

// 构建所有点的连接关系
void GameGrid::BuildLinkingPath()
{
	for (int x=0; x<WORLD_SIZE; x++)
		for (int y=0; y<WORLD_SIZE; y++)
			for (int z=0; z<WORLD_SIZE; z++)
			{
				m_World[x][y][z].neighbourCount = 0;
			}

	for (int x=0; x<WORLD_SIZE; x++)
		for (int y=0; y<WORLD_SIZE; y++)
			for (int z=0; z<WORLD_SIZE; z++)
			{
				BuildNodePath(Point3(x, y, z));
			}
}

// 构建某一点的邻接关系
void GameGrid::BuildNodePath(const Point3& pos)
{
	// 与12个邻接立方体间的关系
	WorldGrid* grid = &m_World[pos.x][pos.y][pos.z];

	// 下方
	if (pos.y>0)
	{
		if (pos.x>0)
		{
			if (!grid->HasNeighbour(Point3(pos.x-1, pos.y-1, pos.z)) && !m_World[pos.x-1][pos.y][pos.z].obj)
				AddGridNeighbour(grid, Point3(pos.x-1, pos.y-1, pos.z), 1.414f);
		}
		if (pos.z>0)
		{
			if (!grid->HasNeighbour(Point3(pos.x, pos.y-1, pos.z-1)) && !m_World[pos.x][pos.y][pos.z-1].obj)
				AddGridNeighbour(grid, Point3(pos.x, pos.y-1, pos.z-1), 1.414f);
		}
		if (pos.x<WORLD_SIZE-1)
		{
			if (!grid->HasNeighbour(Point3(pos.x+1, pos.y-1, pos.z)) && !m_World[pos.x+1][pos.y][pos.z].obj)
				AddGridNeighbour(grid, Point3(pos.x+1, pos.y-1, pos.z), 1.414f);
		}
		if (pos.z<WORLD_SIZE-1)
		{
			if (!grid->HasNeighbour(Point3(pos.x, pos.y-1, pos.z+1)) && !m_World[pos.x][pos.y][pos.z+1].obj)
				AddGridNeighbour(grid, Point3(pos.x, pos.y-1, pos.z+1), 1.414f);
		}
	}

	// 当前行
	if (pos.x>0)
	{
		if (!grid->HasNeighbour(Point3(pos.x-1, pos.y, pos.z)))
			if (pos.y>=WORLD_SIZE-1 || !m_World[pos.x-1][pos.y+1][pos.z].obj)
				AddGridNeighbour(grid, Point3(pos.x-1, pos.y, pos.z), 1.0f);
	}
	if (pos.z>0)
	{
		if (!grid->HasNeighbour(Point3(pos.x, pos.y, pos.z-1)))
			if (pos.y>=WORLD_SIZE-1 || !m_World[pos.x][pos.y+1][pos.z-1].obj)
				AddGridNeighbour(grid, Point3(pos.x, pos.y, pos.z-1), 1.0f);
	}
	if (pos.x<WORLD_SIZE-1)
	{
		if (!grid->HasNeighbour(Point3(pos.x+1, pos.y, pos.z)))
			if (pos.y>=WORLD_SIZE-1 || !m_World[pos.x+1][pos.y+1][pos.z].obj)
				AddGridNeighbour(grid, Point3(pos.x+1, pos.y, pos.z), 1.0f);
	}
	if (pos.z<WORLD_SIZE-1)
	{
		if (!grid->HasNeighbour(Point3(pos.x, pos.y, pos.z+1)))
			if (pos.y>=WORLD_SIZE-1 || !m_World[pos.x][pos.y+1][pos.z+1].obj)
				AddGridNeighbour(grid, Point3(pos.x, pos.y, pos.z+1), 1.0f);
	}

	// 正上方如果被阻挡，则肯定无法联通
	if (pos.y<WORLD_SIZE-2 && m_World[pos.x][pos.y+2][pos.z].obj)
		return;
	// 上方
	if (pos.y<WORLD_SIZE-1)
	{
		if (pos.x>0)
		{
			if (!grid->HasNeighbour(Point3(pos.x-1, pos.y+1, pos.z)))
				if (pos.y>=WORLD_SIZE-2 || !m_World[pos.x-1][pos.y+2][pos.z].obj)
					AddGridNeighbour(grid, Point3(pos.x-1, pos.y+1, pos.z), 1.414f);
		}
		if (pos.z>0)
		{
			if (!grid->HasNeighbour(Point3(pos.x, pos.y+1, pos.z-1)))
				if (pos.y>=WORLD_SIZE-2 || !m_World[pos.x][pos.y+2][pos.z-1].obj)
					AddGridNeighbour(grid, Point3(pos.x, pos.y+1, pos.z-1), 1.414f);
		}
		if (pos.x<WORLD_SIZE-1)
		{
			if (!grid->HasNeighbour(Point3(pos.x+1, pos.y+1, pos.z)))
				if (pos.y>=WORLD_SIZE-2 || !m_World[pos.x+1][pos.y+2][pos.z].obj)
					AddGridNeighbour(grid, Point3(pos.x+1, pos.y+1, pos.z), 1.414f);
		}
		if (pos.z<WORLD_SIZE-1)
		{
			if (!grid->HasNeighbour(Point3(pos.x, pos.y+1, pos.z+1)))
				if (pos.y>=WORLD_SIZE-2 || !m_World[pos.x][pos.y+2][pos.z+1].obj)
					AddGridNeighbour(grid, Point3(pos.x, pos.y+1, pos.z+1), 1.414f);
		}
	}
}

// 将一个位置添加为某个格子的邻接点
void GameGrid::AddGridNeighbour(WorldGrid* grid, const Point3& pos, float len)
{
	if (m_World[pos.x][pos.y][pos.z].obj)
		grid->AddNeighbour(pos, len);
}
//
//void GameGrid::ShowNeighbour(const Point3& pos)
//{
//	for (int i=0; i<WORLD_SIZE; i++)
//		for (int j=0; j<WORLD_SIZE; j++)
//			for (int k=0; k<WORLD_SIZE; k++)
//			{
//				MeshObject* obj = m_World[i][j][k].obj;
//				if (obj)
//					obj->SetDebugRender(false);
//			}
//
//	WorldGrid* grid = &m_World[pos.x][pos.y][pos.z];
//	for (int i=0; i<grid->neighbourCount; i++)
//	{
//		MeshObject* obj = m_World[grid->neighbourPos[i].x][grid->neighbourPos[i].y][grid->neighbourPos[i].z].obj;
//		if (obj)
//			obj->SetDebugRender(true);
//	}
//}

// 路径搜索算法
void GameGrid::PathFinding(const Point3& start, const Point3& end)
{
	if (start==end)
		return;

	typedef map<WorldGrid*, float> CostMap;
	CostMap	total_cost;

	typedef map<WorldGrid*, WorldGrid*> ParentMap;
	ParentMap parents;

	m_OpenList.clear();
	m_CloseList.clear();
	m_Path.clear();

	//float total_cost = 0.0f;
	WorldGrid* grid = &m_World[start.x][start.y][start.z];

	Point3 pos = start;
	m_OpenList.insert(grid);
	total_cost[grid] = 0.0f;

	while(pos != end)
	{

		OpenList::iterator iter = m_OpenList.begin();

		if (iter != m_OpenList.end())
		{
			grid = *iter;
			m_OpenList.erase(m_OpenList.begin());
			//total_cost 
		}
		else
		{
			break;
		}

		m_CloseList.insert(grid);
		for (unsigned int i=0; i<grid->neighbourCount; i++)
		{
			Point3& npos = grid->neighbourPos[i];

			// 注： 这里和原文不一样
			WorldGrid* neighbour = &m_World[npos.x][npos.y][npos.z];
			float cost = total_cost[grid] + neighbour->length[i];

			if (m_OpenList.find(neighbour) != m_OpenList.end() && (cost < total_cost[neighbour]))
				m_OpenList.erase(neighbour);

			if (m_CloseList.find(neighbour) != m_CloseList.end() && (cost < total_cost[neighbour]))
				m_CloseList.erase(neighbour);

			if (m_OpenList.find(neighbour) == m_OpenList.end() &&
				m_CloseList.find(neighbour) == m_CloseList.end())
			{
				total_cost[neighbour] = cost;
				//neighbour->priority = cost + abs(end.x - npos.x) + abs(end.y - npos.y) + abs(end.z - npos.z);
				neighbour->priority = cost + sqrt((float)(end - npos).SquaredLength());
				m_OpenList.insert(neighbour);
				parents[neighbour] = grid;
			}
		}

		//sort(m_OpenList.begin(), m_OpenList.end(), WorldGridComparer);

		OpenList::iterator openIter = m_OpenList.begin();
		if (openIter!=m_OpenList.end())
		{
			pos = (*openIter)->pos;
			if (pos == end)
				break;
		}
		else
			break;
	}

	//Mesh* sphere = MeshManager::Instance().GetByName("sphere");

	for (WorldGrid* g=&m_World[pos.x][pos.y][pos.z]; parents.find(g)!=parents.end(); g=parents[g])
	{
		// Test render path
		//MeshObject* obj = FACTORY_CREATE(m_Scene, MeshObject);
		//obj->SetPosition(g->obj->WorldTransform().GetPosition() + Vector3f(0.0f, 0.5f, 0.0f));
		//obj->SetMesh(sphere);
		//m_Scene->AddObject(obj);

		m_Path.push_front(g);
	}

	m_TestRun = true;
}

// 更新标志的移动位置
void GameGrid::MarkerGo(unsigned long deltaTime)
{
#define MOVEMENT_INTERVAL 500
	if (m_TestRun)
	{
		// 没有更多可以走的路径，停止
		PathList::iterator iter = m_Path.begin();
		if (iter==m_Path.end())
		{
			m_TestRun = false;
		}
		else
		{
			m_TimeToRun += deltaTime;
			if (m_TimeToRun>=MOVEMENT_INTERVAL)
			{
				m_TimeToRun = 0;

				m_LastPos = (*iter)->obj->WorldTransform().GetPosition();
				m_MarkerGoal->SetPosition(m_LastPos);
				m_Path.erase(iter);
			}
			else
			{
				// 在当前点和下一点之间进行位置上的插值
				float aspect = (float)m_TimeToRun / MOVEMENT_INTERVAL;

				Vector3f new_pos = (*iter)->obj->WorldTransform().GetPosition();
				Vector3f pos = (new_pos - m_LastPos) * aspect + m_LastPos;
				m_MarkerGoal->SetPosition(pos);
			}

		}
	}
}

// 将场景保存到文件
bool GameGrid::SaveScene(const String &filename)
{
	ofstream fout(filename.Data());
	if (!fout.is_open())
		return false;

	// 在x、y、z方向上依次遍历
	for (int x=0; x<WORLD_SIZE; x++)
		for (int y=0; y<WORLD_SIZE; y++)
			for (int z=0; z<WORLD_SIZE; z++)
			{
				if (m_World[x][y][z].obj)
					fout << 1 << " ";
				else
					fout << 0 << " ";
			}

	fout.close();

	return true;
}

// 从文件载入场景
bool GameGrid::LoadScene(const String &filename)
{
	ifstream fin(filename.Data());
	if (!fin.is_open())
		return false;

	for (int x=0; x<WORLD_SIZE; x++)
		for (int y=0; y<WORLD_SIZE; y++)
			for (int z=0; z<WORLD_SIZE; z++)
			{
				int b;
				fin >> b;
				if (b)
					AddBox(Point3(x, y, z));
				else
					RemoveBox(Point3(x, y, z));
			}

	fin.close();

	return true;
}
