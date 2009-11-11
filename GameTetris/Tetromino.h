#ifndef _TETROMINO_H
#define _TETROMINO_H

// 由4个方块组成的一个基本形状

#include "GameDefine.h"

//#include "Block.h"

class Block;
class TetrisGame;

class Tetromino
{
public:
	Tetromino();
	~Tetromino();

	void SetPosition(int x, int y);
	inline void GetPosition(int* x, int* y) { *x = m_Position.x; *y = m_Position.y; }

	// 尝试向指定方向移动，无法移动则返回false
	bool MoveDown();
	bool MoveLeft();
	bool MoveRight();

	bool Rotate();

	bool TryPosition(Point2D pos, int rot_type);

	bool Apply();

	// 供Tetromino访问的静态成员
	static TetrisGame*		m_sGame;

	static void ClearBlocksInGame();

	static list<Block*>		m_sBlocksInGame;

private:
	// 构造一个Tetromino
	void BuildShape();

	unsigned char GetNextShape();

	void UpdatePos();

	// 获得旋转后的偏移坐标(顺时针方向)
	Point2D GetRotatedOffset(int rot_type, const Point2D offset);

	Block*			m_Blocks[4];

	//int				m_PosX, m_PosY;
	Point2D			m_Position;

	unsigned char	m_ShapeType;
	unsigned char	m_RotType;
};

#endif
