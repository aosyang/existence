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
  m_Tetromino(NULL),
  m_Sun(NULL),
  m_CanMove(true), m_Time(0), m_TimeToFall(0),
  m_Camera(NULL),
  m_HighScore(0),
  m_GameState(GAME_STATE_PLAYING)
{
	// 清除记录方块的列表
	memset(m_BlockList, 0, sizeof(Block*) * 10 * 20);
}

void TetrisGame::StartGame()
{
	m_Scene = new SceneGraph;

	Block::InitializeData();
	Tetromino::m_sGame = this;

	m_Camera = FACTORY_CREATE(m_Scene, Camera);
	//m_Camera->SetPosition(Vector3f(11.0f, 11.0f, 30.0f));
	m_Camera->SetPosition(Vector3f(0.0f, 11.0f, 30.0f));

	m_Sun = FACTORY_CREATE(m_Scene, Light);
	m_Sun->SetLightType(LIGHT_TYPE_DIRECTIONAL);
	Vector3f sunDir = Vector3f(0.2f, 1.0f, 0.5f);
	sunDir.normalize();
	m_Sun->SetDirection(sunDir);
	LightingManager::Instance().AddGlobalLight(m_Sun);

	m_UIScore = EGUIManager::Instance().CreateTextUIControl();
	m_UIScore->SetWidth(1.0f);
	m_UIScore->SetHBorderSize(0.05f);
	m_UIScore->SetVBorderSize(0.05f);

	m_GameOverText = EGUIManager::Instance().CreateTextUIControl();
	m_GameOverText->SetWidth(1.0f);
	m_GameOverText->SetHBorderSize(0.05f);
	m_GameOverText->SetVBorderSize(0.25f);
	m_GameOverText->SetText("Game Over!!\nPress Enter to restart");

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

	Restart();

	InputEventHandler<TetrisGame>* inputHandler =
		new InputEventHandler<TetrisGame>(this,
										  &TetrisGame::OnKeyPressed,
										  NULL,
										  &TetrisGame::OnMouseMoved);
	Input->SetEventHandler(inputHandler);

}

void TetrisGame::Shutdown()
{
	for (vector<Block*>::iterator iter = m_Boundary.begin(); iter != m_Boundary.end(); iter++)
	{
		delete *iter;
	}

	SAFE_DELETE(m_Tetromino);

	Tetromino::ClearBlocksInGame();

	// 释放模型、纹理资源
	Block::ReleaseData();

	delete m_Scene;
}

void TetrisGame::Restart()
{
	m_Score = 0;

	Tetromino::ClearBlocksInGame();
	m_GameState = GAME_STATE_PLAYING;

	if (m_Tetromino) SAFE_DELETE(m_Tetromino);
	m_Tetromino = new Tetromino();

	for (int i=0; i<10; i++)
		for (int j=0; j<20; j++)
			m_BlockList[i][j] = NULL;

	//m_BlockList.push_back(m_Block);
	m_Time = 0;
	m_TimeToFall = 0;

	m_GameOverText->SetVisible(false);
}

void TetrisGame::GameOver()
{
	m_GameOverText->SetVisible(true);
}

bool TetrisGame::OnNotifyQuitting()
{
	return true;
}

void TetrisGame::OnKeyPressed(KeyCode key)
{
	switch (key)
	{
	case KC_SPACE:
		if (m_GameState==GAME_STATE_PLAYING)
			Rotate();
		break;
	case KC_RETURN:
		if (m_GameState==GAME_STATE_OVER)
			Restart();
		break;
	}
}

void TetrisGame::OnMouseMoved(int x, int y, int rel_x, int rel_y)
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

void TetrisGame::Update(unsigned long deltaTime)
{
	switch (m_GameState)
	{
	case GAME_STATE_PLAYING:
		{
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

			//if (Input->GetKeyDown(KC_UP))
			//	MoveUp();
			if (Input->GetKeyDown(KC_DOWN))
				MoveDown();
			if (Input->GetKeyDown(KC_LEFT))
				MoveLeft();
			if (Input->GetKeyDown(KC_RIGHT))
				MoveRight();
			//if (Input->GetKeyDown(KC_SPACE))
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
			//if (m_TimeToFall >= 100)
			{
				m_TimeToFall = 0;
				if (!m_Tetromino->MoveDown())
				{
					CheckAndRemove();
				}
			}

			//Renderer::Instance().SetViewMatrix(m_Camera->GetViewMatrix());

			String score;
			score.Format("High Score: %d\n\nScore: %d", m_HighScore, m_Score);
			m_UIScore->SetText(score);
		}
		break;
	case GAME_STATE_OVER:
		{

		}
	}

	m_Scene->UpdateScene(deltaTime);
	LightingManager::Instance().Update();
}

void TetrisGame::RenderScene()
{
	Renderer::Instance().SetViewport(0, 0, 0, 0);

	// 设定渲染视点
	m_Scene->SetupRenderView(m_Camera);
	Renderer::Instance().BeginRender();
	m_Scene->RenderScene();
	Renderer::Instance().EndRender();
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

	AudioBuffer* buffer = AudioManager::Instance().GetByName("move.wav");
	//Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));
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

	//IAudioBuffer* buffer = Engine::Instance().AudioSystem()->GetAudioBuffer("move.wav");
	//Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));
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

	//IAudioBuffer* buffer = Engine::Instance().AudioSystem()->GetAudioBuffer("move.wav");
	//Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));
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

void TetrisGame::AddScore(int score)
{
	m_Score += score;
	if (m_Score>m_HighScore)
		m_HighScore = m_Score;
}

void TetrisGame::CheckAndRemove()
{
	bool remove(false);

	int multipyCount = 0;

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
			AddScore(100 + multipyCount * 50);
			multipyCount++;
		}
		
	}

	if (remove)
	{
		//IAudioBuffer* buffer = Engine::Instance().AudioSystem()->GetAudioBuffer("remove.wav");
		//Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));
	}
}

void TetrisGame::RemoveLine(int line)
{
	// 删除这一行
	for (int x=0; x<10; x++)
	{
		//list<Block*>::iterator iter = find(Tetromino::m_sBlocksInGame.begin(), Tetromino::m_sBlocksInGame.end(), m_BlockList[x][line]);
		list<Block*>::iterator iter = find_if(Tetromino::m_sBlocksInGame.begin(), Tetromino::m_sBlocksInGame.end(), bind2nd(equal_to<Block*>(),(m_BlockList[x][line])));
		
		if (iter != Tetromino::m_sBlocksInGame.end())
			Tetromino::m_sBlocksInGame.erase(iter);
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

