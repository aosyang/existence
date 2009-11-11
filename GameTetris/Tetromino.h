#ifndef _TETROMINO_H
#define _TETROMINO_H

// ��4��������ɵ�һ��������״

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

	// ������ָ�������ƶ����޷��ƶ��򷵻�false
	bool MoveDown();
	bool MoveLeft();
	bool MoveRight();

	bool Rotate();

	bool TryPosition(Point2D pos, int rot_type);

	bool Apply();

	// ��Tetromino���ʵľ�̬��Ա
	static TetrisGame*		m_sGame;

	static void ClearBlocksInGame();

	static list<Block*>		m_sBlocksInGame;

private:
	// ����һ��Tetromino
	void BuildShape();

	unsigned char GetNextShape();

	void UpdatePos();

	// �����ת���ƫ������(˳ʱ�뷽��)
	Point2D GetRotatedOffset(int rot_type, const Point2D offset);

	Block*			m_Blocks[4];

	//int				m_PosX, m_PosY;
	Point2D			m_Position;

	unsigned char	m_ShapeType;
	unsigned char	m_RotType;
};

#endif
