//-----------------------------------------------------------------------------------
/// TetrisGame.h ����˹������Ϸ�࣬ʵ����Ϸ�߼�
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _TETRIS_H
#define _TETRIS_H

#include "Existence.h"

#include "Tetromino.h"
#include <vector>

using namespace std;

enum GameState
{
	GAME_STATE_TITLE,
	GAME_STATE_PLAYING,
	GAME_STATE_OVER,
};

class Block;

class TetrisGame : public IGame
{
public:
	TetrisGame();
	~TetrisGame() {}

	void StartGame();
	void Shutdown();

	void Restart();
	void GameOver();

	bool OnNotifyQuitting();

	void OnKeyPressed(unsigned int key);
	void OnKeyReleased(unsigned int key);

	void Update(unsigned long deltaTime);
	void RenderScene();

	SceneGraph* GetScene() { return m_Scene; }

	// ��Ӧ���������������ƶ�
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void Rotate();

	inline void SetGameState(GameState state) { m_GameState = state; }

	void AddScore(int score);

	//void CreateBlock();

	Block*			m_BlockList[10][20];	// �����ռ�

	// x, y�������£�
	//						   [x ][y ]
	//  [0][y]
	// |
	// |
	// |
	// |
	// |[0][0]
	// +-------------> [x][0]

private:

	// ����Ƿ��������
	void CheckAndRemove();

	void RemoveLine(int line);

private:
	SceneGraph*		m_Scene;

	Tetromino*		m_Tetromino;
	vector<Block*>	m_Boundary;

	Light*			m_Sun;

	bool			m_CanMove;
	unsigned long	m_Time;
	unsigned long	m_TimeToFall;

	Camera*			m_Camera;

	int				m_Score;
	int				m_HighScore;
	TextUIControl*	m_UIScore;

	GameState		m_GameState;

	TextUIControl*	m_GameOverText;
};

#endif
