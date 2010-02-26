//-----------------------------------------------------------------------------------
/// BaseMesh.h 网格模型基类
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _BASEMESH_H
#define _BASEMESH_H

#include "OBB.h"
#include "Resource.h"

#include <vector>

using namespace std;

namespace Gen
{
	class IVertexBuffer;
	class Material;
	struct SkinnedVertexInfo;
	class Skeleton;
	class MeshElement;
	class MeshBuilder;

	// TODO: 创建一个static mesh类，使顶点无法修改，节约内存占用
	class BaseMesh : public Resource
	{
		friend class MeshManager;
	public:
		BaseMesh(const String& filename);
		~BaseMesh();

		// ----- BaseMesh Methods

		int GetElementCount() const;
		MeshElement* GetElement(unsigned int index);

		// 使用当前数据生成顶点缓冲
		// Note: 这个目前仅仅用于骨骼动画
		//void BuildVertexBuffer();

		// 以当前的顶点创建顶点缓冲的副本
		IVertexBuffer* BuildVertexBufferCopy();

		// 使用模型构造器创建模型
		void BuildMesh(const MeshBuilder* builder);

		Material* GetMaterial(unsigned int index);

		inline float GetBoundingRadius() const { return m_BoundingRadius; }
		inline const OBB& GetOBB() const { return m_OBB; }

		// 通过索引获取顶点
		Vector3f GetVertexByIndex(unsigned int index) const;

		// 获取顶点数组以进行修改
		inline int GetVertexCount() const { return m_VertexNum; }
		inline float* GetVertexArray() { return m_VertexArray; }
		inline float* GetNormalArray() { return m_NormalArray; }
		inline float* GetColorArray() { return m_ColorArray; }
		inline float* GetTexCoordArray() { return m_TexcoordArray; }

		inline IVertexBuffer* GetVertexBuffer() const { return m_VertexBuffer; }

		inline SkinnedVertexInfo* GetSkin() const { return m_Skin; }
	protected:
		// ----- Overwrite Resource
		bool LoadImpl();
		void UnloadImpl();

		// ----- BaseMesh Methods

		// 添加模型子元素
		void AddMeshElement(MeshElement* elem);

		// 更新所有网格元素的obb
		void UpdateElementsOBB();

		// 清除所有子元素
		void ClearMeshElements();

	protected:
		float					m_BoundingRadius;		///< 包围球半径
		OBB						m_OBB;					///< 包围盒尺寸

		vector<MeshElement*>	m_MeshElements;			///< 子元素集合
		vector<Material*>		m_Materials;

		int						m_VertexNum;			///< 顶点数目
		float*					m_VertexArray;			///< 顶点数组
		float*					m_NormalArray;			///< 法线数组
		float*					m_ColorArray;
		float*					m_TexcoordArray;
		IVertexBuffer*			m_VertexBuffer;			///< 模型的顶点集合

		SkinnedVertexInfo*		m_Skin;					///< 蒙皮信息
	};

	class MeshElementBuilder;

	// Mesh构造器
	class MeshBuilder
	{
		friend class BaseMesh;
	public:
		MeshBuilder(unsigned int elementCount);
		~MeshBuilder();

		void AddVertex(float pos[3], float normal[3], float texcoord[2], unsigned int meshElementIndex);
	private:
		void AddIndex(unsigned int index, unsigned int meshElementIndex);
	private:
		float*			m_Vertices;
		float*			m_Normals;
		float*			m_Texcoords;

		int				m_VertexCaps;
		int				m_VertexCount;

		unsigned int	m_MeshElementCount;
		MeshElementBuilder*	m_MeshElements;

		OBB				m_OBB;
	};

	class MeshElementBuilder
	{
		friend class BaseMesh;
	public:
		MeshElementBuilder();
		~MeshElementBuilder();

		void AddIndex(unsigned int index);

	private:
		unsigned int*	m_Indices;
		int				m_IndexCaps;
		int				m_IndexCount;
	};
}

#endif

