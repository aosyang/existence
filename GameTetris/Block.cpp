#include "Block.h"

extern Color4f BLOCK_COLOR[MAX_SHAPE_TYPE_NUM];

Material*		Block::m_sBlockMaterial[MAX_SHAPE_TYPE_NUM + 1];
ITexture*		Block::m_sBlockTexture;
PrimitiveMesh*	Block::m_sBlockMesh;
bool			Block::m_sDataInitialized = false;

SceneGraph* GetScene()
{
	return reinterpret_cast<TetrisGame*>(Engine::Instance().GetGame())->GetScene();
}

Block::Block()
 : m_PosX(0), m_PosY(0)
{
	m_Shape = FACTORY_CREATE(GetScene(), MeshObject);
	m_Shape->SetMesh(m_sBlockMesh);

	m_Shape->CreateLightableObject();
}

Block::~Block()
{
	m_Shape->DestroyLightableObject();
	GetScene()->RemoveObject(m_Shape);
}

void Block::SetPosition(int x, int y)
{
	m_PosX = x;
	m_PosY = y;
	m_Shape->SetPosition(Vector3f(1.1f * x, 1.1f * y, 0.0f));
}

//void Block::SetColor(float r, float g, float b, float a)
//{
//	m_Mesh.SetMeshColor(r, g, b, a);
//}

void Block::SetColorType(unsigned int type)
{
	m_Shape->SetMaterial(m_sBlockMaterial[type], 0);
	//m_Shape->SetColor(color);
}

void Block::InitializeData()
{
	if (m_sDataInitialized)
		return;

	// 创建方块的模型
	m_sBlockMesh = MeshManager::Instance().CreatePrimitiveMesh();
	m_sBlockMesh->CreateBox(1.0f);

	// 指定方块材质

	//Block::m_sBlockTexture = TextureManager::Instance().GetTexture("block_surface");
	Block::m_sBlockTexture = renderer->GetTexture("blank.bmp");

	// 生成不同形状用的材质
	for (int i=0; i<MAX_SHAPE_TYPE_NUM + 1; i++)
	{
		Block::m_sBlockMaterial[i] = ResourceManager<Material>::Instance().Create();
		Block::m_sBlockMaterial[i]->SetTexture(Block::m_sBlockTexture);

		if (i<MAX_SHAPE_TYPE_NUM)
		{
			Block::m_sBlockMaterial[i]->SetDiffuse(BLOCK_COLOR[i]);
		}
	}

	m_sDataInitialized = true;
}

void Block::ReleaseData()
{
	if (!m_sDataInitialized)
		return;

	m_sDataInitialized = false;
}