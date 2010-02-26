#ifndef _BLOCK_H
#define _BLOCK_H

// 单位方块
// 最基本单元
#include "GameDefine.h"

//class TetrisGame;
#include "TetrisGame.h"

class Block
{
	friend class TetrisGame;
public:
	Block();
	~Block();

	// 设定在二维空间中的坐标
	void SetPosition(int x, int y);

	// 获取二维空间坐标
	inline void GetPosition(int *x, int *y) { *x = m_PosX; *y = m_PosY; }

	// 设置颜色
	//void SetColor(float r, float g, float b, float a = 1.0f);

	void SetColorType(unsigned int type);

	static void InitializeData();
	static void ReleaseData();

private:
	MeshObject*	m_Shape;
	int			m_PosX, m_PosY;

	static Material*		m_sBlockMaterial[MAX_SHAPE_TYPE_NUM + 1];
	static BaseTexture*		m_sBlockTexture;
	static PrimitiveMesh*	m_sBlockMesh;
	static bool				m_sDataInitialized;
};

#endif
