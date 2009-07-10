//-----------------------------------------------------------------------------------
/// TetrisGame.h 俄罗斯方块游戏类，实现游戏逻辑
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

class Block;

class TetrisGame : public IGame
{
public:
	TetrisGame();
	~TetrisGame() {}

	void StartGame();
	void Shutdown();

	bool OnNotifyQuitting();

	void OnKeyPressed(unsigned int key);
	void OnKeyReleased(unsigned int key);

	void Update(unsigned long deltaTime);
	void RenderScene();

	SceneGraph* GetScene() { return m_Scene; }

	// 响应按键的上下左右移动
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void Rotate();

	//void CreateBlock();

	Block*			m_BlockList[10][20];	// 可填充空间

	// x, y坐标如下：
	//						   [x ][y ]
	//  [0][y]
	// |
	// |
	// |
	// |
	// |[0][0]
	// +-------------> [x][0]

private:

	// 检查是否可以消除
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
};

#endif
