#ifndef _BLOCK_H
#define _BLOCK_H

// ��λ����
// �������Ԫ
#include "GameDefine.h"

//class TetrisGame;
#include "TetrisGame.h"

class Block
{
	friend class TetrisGame;
public:
	Block();
	~Block();

	// �趨�ڶ�ά�ռ��е�����
	void SetPosition(int x, int y);

	// ��ȡ��ά�ռ�����
	inline void GetPosition(int *x, int *y) { *x = m_PosX; *y = m_PosY; }

	// ������ɫ
	//void SetColor(float r, float g, float b, float a = 1.0f);

	void SetColorType(unsigned int type);

	static void InitializeData();
	static void ReleaseData();

private:
	MeshObject	m_Shape;
	int			m_PosX, m_PosY;

	static Material*	s_BlockMaterial[MAX_SHAPE_TYPE_NUM + 1];
	static ITexture*	s_BlockTexture;
	static Mesh*		s_BlockMesh;
	static bool			s_DataInitialized;
};

#endif
