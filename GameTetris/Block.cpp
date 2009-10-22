#include "Block.h"

extern Color4f BLOCK_COLOR[MAX_SHAPE_TYPE_NUM];

Material*	Block::s_BlockMaterial[MAX_SHAPE_TYPE_NUM + 1];
ITexture*	Block::s_BlockTexture;
Mesh*		Block::s_BlockMesh;
bool		Block::s_DataInitialized = false;

Block::Block()
 : m_PosX(0), m_PosY(0)
{
	m_Shape.SetMesh(s_BlockMesh);

	// �������Ⱦ�����б�
	reinterpret_cast<TetrisGame*>(Engine::Instance().GetGame())->GetScene()->AddObject(&m_Shape);
	m_Shape.CreateLightableObject();
	//m_Shape.SetFrustumCulling(false);
	//renderer->AddRenderObject(&m_Shape);
}

Block::~Block()
{
	m_Shape.DestroyLightableObject();
	reinterpret_cast<TetrisGame*>(Engine::Instance().GetGame())->GetScene()->RemoveObject(&m_Shape, false);
	//static_cast<TetrisGame*>(Engine::Instance().GetGame())->GetScene()->AddObject(&m_Shape);
	//renderer->RemoveRenderObject(&m_Shape);
}

void Block::SetPosition(int x, int y)
{
	m_PosX = x;
	m_PosY = y;
	m_Shape.SetPosition(Vector3f(1.1f * x, 1.1f * y, 0.0f));
}

//void Block::SetColor(float r, float g, float b, float a)
//{
//	m_Mesh.SetMeshColor(r, g, b, a);
//}

void Block::SetColorType(unsigned int type)
{
	m_Shape.SetMaterial(s_BlockMaterial[type], 0);
	//m_Shape.SetColor(color);
}

void Block::InitializeData()
{
	if (s_DataInitialized)
		return;

	// ���������ģ��
	s_BlockMesh = ResourceManager<Mesh>::Instance().Create();
	s_BlockMesh->CreateBox(1.0f);

	// ָ���������

	//Block::s_BlockTexture = TextureManager::Instance().GetTexture("block_surface");
	Block::s_BlockTexture = renderer->GetTexture("blank.bmp");

	// ���ɲ�ͬ��״�õĲ���
	for (int i=0; i<MAX_SHAPE_TYPE_NUM + 1; i++)
	{
		Block::s_BlockMaterial[i] = ResourceManager<Material>::Instance().Create();
		Block::s_BlockMaterial[i]->SetTexture(Block::s_BlockTexture);

		if (i<MAX_SHAPE_TYPE_NUM)
		{
			Block::s_BlockMaterial[i]->SetDiffuse(BLOCK_COLOR[i]);
		}
	}

	s_DataInitialized = true;
}

void Block::ReleaseData()
{
	if (!s_DataInitialized)
		return;

	s_DataInitialized = false;
}