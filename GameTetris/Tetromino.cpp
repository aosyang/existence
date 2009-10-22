#include "Tetromino.h"

#include "TetrisGame.h"
#include "Block.h"

// 各种形状
static Point2D BLOCK_OFFSET[MAX_SHAPE_TYPE_NUM][4] =
{
	{Point2D(0, 2), Point2D(0, 1), Point2D(0, 0), Point2D(0, -1)},		// I
	{Point2D(-1, 1), Point2D(-1, 0), Point2D(0, 0), Point2D(1, 0)},		// J
	{Point2D(1, 1), Point2D(-1, 0), Point2D(0, 0), Point2D(1, 0)},		// L
	{Point2D(1, 1), Point2D(0, 1), Point2D(0, 0), Point2D(1, 0)},		// O
	{Point2D(1, 1), Point2D(-1, 0), Point2D(0, 0), Point2D(0, 1)},		// S
	{Point2D(1, 0), Point2D(-1, 0), Point2D(0, 0), Point2D(0, 1)},		// T
	{Point2D(1, 0), Point2D(-1, 1), Point2D(0, 0), Point2D(0, 1)},		// Z
};

// 每个形状的颜色
Color4f BLOCK_COLOR[MAX_SHAPE_TYPE_NUM] =
{
	Color4f(1.0f, 0.0f, 0.0f),		// I
	Color4f(0.0f, 0.0f, 1.0f),		// J
	Color4f(1.0f, 0.5f, 0.25f),		// L
	Color4f(0.75f, 0.75f, 0.75f),	// O
	Color4f(0.0f, 1.0f, 0.0f),		// S
	Color4f(1.0f, 0.0f, 1.0f),		// T
	Color4f(0.0f, 1.0f, 1.0f),		// Z
};

TetrisGame* Tetromino::s_Game;

list<Block*> Tetromino::s_BlocksInGame;

Tetromino::Tetromino()
: /*m_PosX(0), m_PosY(0), */m_Position(0, 0), m_RotType(ROT_0)
{
	BuildShape();
}

Tetromino::~Tetromino()
{
	//for (int i=0; i<4; i++)
	//{
	//	if (m_Blocks[i])
	//	{
	//		delete m_Blocks[i];
	//		m_Blocks[i] = 0;
	//	}
	//}
}

void Tetromino::SetPosition(int x, int y)
{
	m_Position.x = x;
	m_Position.y = y;
	UpdatePos();
}

void Tetromino::BuildShape()
{
	m_ShapeType = GetNextShape();

	for (int i=0; i<4; i++)
	{
		m_Blocks[i] = new Block();
		m_Blocks[i]->SetColorType(m_ShapeType);

		s_BlocksInGame.push_back(m_Blocks[i]);
	}

	UpdatePos();
	SetPosition(4, 19);
}

unsigned char Tetromino::GetNextShape()
{
	// TODO: 产生随机序列，防止重复

	static int index = MAX_SHAPE_TYPE_NUM - 1;
	static int seq[MAX_SHAPE_TYPE_NUM];

	index++;

	if (index >= MAX_SHAPE_TYPE_NUM)	
	{
		// 重新生成一批序列
		for (int i=0; i<MAX_SHAPE_TYPE_NUM; i++)
			seq[i] = i;

		for (int i=0; i<MAX_SHAPE_TYPE_NUM; i++)
		{
			int s = Math::Random<int>(0, MAX_SHAPE_TYPE_NUM);
			if (s==i) continue;
			swap(seq[s], seq[i]);
		}

		index = 0;
	}

	return seq[index];
}


bool Tetromino::MoveDown()
{
	int x = m_Position.x;
	int y = m_Position.y - 1;

	if (!TryPosition(Point2D(x, y), m_RotType))
	{
		// 向下移动失败则视作接触，开始下一个方块的下落
		if (!Apply())
		{
			// Game over
			s_Game->SetGameState(GAME_STATE_OVER);
			s_Game->GameOver();
			return true;
		}
		return false;
	}

	m_Position.x = x;
	m_Position.y = y;

	UpdatePos();

	return true;
}

bool Tetromino::MoveLeft()
{
	int x = m_Position.x - 1;
	int y = m_Position.y;

	if (!TryPosition(Point2D(x, y), m_RotType))
		return false;

	m_Position.x = x;
	m_Position.y = y;

	UpdatePos();

	return true;
}

bool Tetromino::MoveRight()
{
	int x = m_Position.x + 1;
	int y = m_Position.y;

	if (!TryPosition(Point2D(x, y), m_RotType))
		return false;

	m_Position.x = x;
	m_Position.y = y;

	UpdatePos();

	return true;
}

bool Tetromino::Rotate()
{
	unsigned char rot = Math::Repeat(0, 4, m_RotType+1);

	if (!TryPosition(m_Position, rot))
		return false;

	m_RotType = rot;
	UpdatePos();

	return true;
}

bool Tetromino::TryPosition(Point2D pos, int rot_type)
{
	for (int i=0; i<4; i++)
	{
		Point2D new_pos = pos;
		new_pos += GetRotatedOffset(rot_type, BLOCK_OFFSET[m_ShapeType][i]);

		//int bx = x, by = y;

		//bx += BLOCK_OFFSET[m_ShapeType][i].x;
		//by += BLOCK_OFFSET[m_ShapeType][i].y;

		// 位于合理范围以外
		if ( new_pos.x < 0 || new_pos.x >9 || new_pos.y < 0/* || by > 19 */)
			return false;

		// 如果还没有进入游戏范围的不予以考虑，防止数组越界
		if (new_pos.y > 19)
			continue;

		// 此格已经被占据
		if (s_Game->m_BlockList[new_pos.x][new_pos.y])
			return false;
	}

	return true;
}

// 固定在当前位置
bool Tetromino::Apply()
{
	for (int i=0; i<4; i++)
	{
		Point2D pos = m_Position + GetRotatedOffset(m_RotType, BLOCK_OFFSET[m_ShapeType][i]);
		//int x = m_PosX + BLOCK_OFFSET[m_ShapeType][i].x;
		//int y = m_PosY + BLOCK_OFFSET[m_ShapeType][i].y;

		if (pos.y > 19)
			continue;

		s_Game->m_BlockList[pos.x][pos.y] = m_Blocks[i];
	}

	// TODO: 创建新的Block之前需要将旧的Block保存起来，方便退出程序时进行删除
	BuildShape();

	IAudioBuffer* buffer = Engine::Instance().AudioSystem()->GetAudioBuffer("down.wav");
	Engine::Instance().AudioSystem()->CreateSourceInstance(buffer, Vector3f(0.0f, 0.0f, 0.0f));

	// GameOver的判定
	if (!TryPosition(m_Position, m_RotType))
		// Gameover;
		return false;

	return true;
}

void Tetromino::ClearBlocksInGame()
{
	for (list<Block*>::iterator iter=s_BlocksInGame.begin();
		iter!=s_BlocksInGame.end();
		iter++)
	{
		delete (*iter);
		*iter = 0;
	}
}

void Tetromino::UpdatePos()
{
	// 使用各个方块形状的定义更新每一部分
	for (int i=0; i<4; i++)
	{
		Point2D pos = m_Position + GetRotatedOffset(m_RotType, BLOCK_OFFSET[m_ShapeType][i]);
		//m_Blocks[i]->SetPosition(
		//	m_PosX + BLOCK_OFFSET[m_ShapeType][i].x,
		//	m_PosY + BLOCK_OFFSET[m_ShapeType][i].y );

		m_Blocks[i]->SetPosition(pos.x, pos.y);
	}
}

Point2D Tetromino::GetRotatedOffset(int rot_type, const Point2D offset)
{
	//| y       | o (x, y)
	//|         |
	//|         |
	//|o''(-y,x)|(0, 0)
	//+---------+--------
	//|         |       o' (y, -x)
	//|         |
	//|         |
	//+---------+--------> x

	switch(rot_type)
	{
	case ROT_0:
		return offset;
	case ROT_90:
		return Point2D(offset.y, -offset.x);
	case ROT_180:
		return Point2D(-offset.x, -offset.y);
	case ROT_270:
		return Point2D(-offset.y, offset.x);
	default:
		break;
	}

	// 错误的参数
	return Point2D(0, 0);
}
