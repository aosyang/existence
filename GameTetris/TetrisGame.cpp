//-----------------------------------------------------------------------------------
/// TetrisGame.cpp 俄罗斯方块游戏类的实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "TetrisGame.h"
#include "Block.h"
#include <algorithm>
#include <functional>

TetrisGame::TetrisGame()
: m_Scene(NULL),
  m_Sun(NULL),
  m_CanMove(true), m_Time(0), m_TimeToFall(0),
  m_Camera(NULL)
{
	memset(m_BlockList, 0, sizeof(Block*) * 10 * 20);
}

void TetrisGame::StartGame()
{
	m_Scene = new SceneGraph;

	Block::InitializeData();
	Tetromino::s_Game = this;

	m_Camera = new Camera();
	m_Camera->SetPosition(Vector3f(11.0f, 11.0f, 30.0f));

	m_Scene->AddObject(m_Camera);

	m_Sun = new Light();
	m_Sun->SetLightType(LIGHT_TYPE_DIRECTIONAL);
	Vector3f sunDir = Vector3f(0.2f, 1.0f, 0.5f);
	sunDir.normalize();
	m_Sun->SetDirection(sunDir);
	LightingManager::Instance().AddGlobalLight(m_Sun);

	// 创建边框

	// 左
	for (int y=0; y<20; y++)
	{
		int x = -1;
		Block* b = new Block();
		b->SetPosition(x, y);
		b->SetColorType(MAX_SHAPE_TYPE_NUM);
		m_Boundary.push_back(b);
	}

	// 右
	for (int y=0; y<20; y++)
	{
		int x = 10;
		Block* b = new Block();
		b->SetPosition(x, y);
		b->SetColorType(MAX_SHAPE_TYPE_NUM);
		m_Boundary.push_back(b);
	}

	// 下
	for (int x=-1; x<11; x++)
	{
		int y = -1;
		Block* b = new Block();
		b->SetPosition(x, y);
		b->SetColorType(MAX_SHAPE_TYPE_NUM);
		m_Boundary.push_back(b);
	}

	m_Tetromino = new Tetromino();
	m_Tetromino->BuildShape();
	m_Tetromino->SetPosition(4, 19);
	//m_BlockList.push_back(m_Block);
	m_Time = 0;
	m_TimeToFall = 0;
}

void TetrisGame::Shutdown()
{
	for (vector<Block*>::iterator iter = m_Boundary.begin(); iter != m_Boundary.end(); iter++)
	{
		delete *iter;
	}

	delete m_Tetromino;

	Tetromino::ClearBlocksInGame();

	SAFE_DELETE(m_Sun);

	// 释放模型、纹理资源
	Block::ReleaseData();

	delete m_Scene;
}

bool TetrisGame::OnNotifyQuitting()
{
	return true;
}

void TetrisGame::OnKeyPressed(unsigned int key)
{
	switch (key)
	{
	case KC_SPACE:
		Rotate();
		break;
	case KC_F:
		bool debugRender = Engine::Instance().GetDebugRender();
		Engine::Instance().ToggleDebugRender(!debugRender);
		break;
	}
}

void TetrisGame::OnKeyReleased(unsigned int key)
{

}

void TetrisGame::Update(unsigned long deltaTime)
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

	//if (Input::Instance().GetKeyDown(KC_UP))
	//	MoveUp();
	if (Input::Instance().GetKeyDown(KC_DOWN))
		MoveDown();
	if (Input::Instance().GetKeyDown(KC_LEFT))
		MoveLeft();
	if (Input::Instance().GetKeyDown(KC_RIGHT))
		MoveRight();
	//if (Input::Instance().GetKeyDown(KC_SPACE))
	//	Rotate();


	// 重复按键时间间隔
	m_Time += deltaTime;

	if (m_Time >= 100)
	{
		m_Time = 0;
		m_CanMove = true;
	}

	// 下落时间
	m_TimeToFall += deltaTime;
	if (m_TimeToFall >= 1000)
	{
		m_TimeToFall = 0;
		if (!m_Tetromino->MoveDown())
		{
			CheckAndRemove();
		}
	}

	//renderer->SetViewMatrix(m_Camera->GetViewMatrix());

	m_Scene->UpdateScene(deltaTime);
	LightingManager::Instance().Update();
}

void TetrisGame::RenderScene()
{
	RenderView rv;

	rv.position = m_Camera->WorldTransform().GetPosition();
	rv.viewMatrix = m_Camera->GetViewMatrix();
	rv.projMatrix = m_Camera->GetProjMatrix();
	rv.frustum = m_Camera->GetFrustum();

	renderer->SetViewport(0, 0, 0, 0);

	// 设定渲染视点
	m_Scene->SetupRenderView(rv);
	m_Scene->RenderScene();
}

// 向上移动
void TetrisGame::MoveUp()
{
	if (!m_CanMove)
		return;
	m_Time = 0;
	m_CanMove = false;

	int x, y;
	m_Tetromino->GetPosition(&x, &y);

	y = Math::Clamp(0, 19, ++y);

	m_Tetromino->SetPosition(x, y);
}

// 向下移动
void TetrisGame::MoveDown()
{
	if (!m_CanMove)
		return;
	m_Time = 0;
	m_TimeToFall = 0;
	m_CanMove = false;

	//int x, y;
	//m_Tetromino->GetPosition(&x, &y);

	//y = MathClamp(0, 19, --y);

	//m_Tetromino->SetPosition(x, y);
	if (!m_Tetromino->MoveDown())
	{
		CheckAndRemove();
	}

	IAudioBuffer* buffer = Engine::Instance().AudioSystem()->GetAudioBuffer("move.wav");
	Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));
}

// 向左移动
void TetrisGame::MoveLeft()
{
	if (!m_CanMove)
		return;
	m_Time = 0;
	m_CanMove = false;

	//int x, y;
	//m_Tetromino->GetPosition(&x, &y);

	//x = MathClamp(0, 9, --x);

	//m_Tetromino->SetPosition(x, y);
	m_Tetromino->MoveLeft();

	IAudioBuffer* buffer = Engine::Instance().AudioSystem()->GetAudioBuffer("move.wav");
	Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));
}

// 向右移动
void TetrisGame::MoveRight()
{
	if (!m_CanMove)
		return;
	m_Time = 0;
	m_CanMove = false;

	//int x, y;
	//m_Tetromino->GetPosition(&x, &y);

	//x = MathClamp(0, 9, ++x);

	//m_Tetromino->SetPosition(x, y);
	m_Tetromino->MoveRight();

	IAudioBuffer* buffer = Engine::Instance().AudioSystem()->GetAudioBuffer("move.wav");
	Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));
}

void TetrisGame::Rotate()
{
	//if (!m_CanMove)
	//	return;
	//m_Time = 0;
	//m_CanMove = false;

	m_Tetromino->Rotate();
}
//
//void TetrisGame::CreateBlock()
//{
//	if (!m_CanMove)
//		return;
//
//	m_Time = 0;
//	m_CanMove = false;
//
//	//m_Tetromino = new Tetromino();
//	//m_Tetromino->SetPosition(4, 19);
//	//m_BlockList.push_back(m_Block);
//
//	m_Tetromino->Apply(m_BlockList);
//}

void TetrisGame::CheckAndRemove()
{
	bool remove(false);

	// 自上而下检查，否则下沉一行以后还需要再多检查一次
	for (int y=19; y>=0; y--)
	{
		bool full = true;
		for (int x=0; x<10; x++)
		{
			// 有一格为空就会被置为false
			if ( !(full &= (m_BlockList[x][y] != 0)))
				break;
		}

		// 满一行
		if (full)
		{
			RemoveLine(y);
			remove = true;
		}
		
	}

	if (remove)
	{
		IAudioBuffer* buffer = Engine::Instance().AudioSystem()->GetAudioBuffer("remove.wav");
		Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));
	}
}

void TetrisGame::RemoveLine(int line)
{
	// 删除这一行
	for (int x=0; x<10; x++)
	{
		//list<Block*>::iterator iter = find(Tetromino::s_BlocksInGame.begin(), Tetromino::s_BlocksInGame.end(), m_BlockList[x][line]);
		list<Block*>::iterator iter = find_if(Tetromino::s_BlocksInGame.begin(), Tetromino::s_BlocksInGame.end(), bind2nd(equal_to<Block*>(),(m_BlockList[x][line])));
		
		if (iter != Tetromino::s_BlocksInGame.end())
			Tetromino::s_BlocksInGame.erase(iter);
		delete m_BlockList[x][line];
		m_BlockList[x][line] = NULL;
	}

	// 上面的内容向下移动一行
	for (int y=line; y<19; y++)
	{
		for (int x=0; x<10; x++)
		{
			if (m_BlockList[x][y] = m_BlockList[x][y+1])
			{
				int bx, by;
				m_BlockList[x][y]->GetPosition(&bx, &by);

				by--;
				m_BlockList[x][y]->SetPosition(bx, by);
			}
		}
	}

	// 最上面一排空出来
	for (int x=0; x<10; x++)
	{
		m_BlockList[x][19] = NULL;
	}

}

