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



namespace Gen
{
	class IVertexBuffer;
	struct SkinnedVertexInfo;
	class Skeleton;
	class MeshElement;
	class MeshBuilder;
	class BaseMaterial;

	// TODO: 创建一个static mesh类，使顶点无法修改，节约内存占用

	/// @brief
	/// 网格模型基类
	/// @par
	///		当一个网格模型加载后，网格模型基类记录了这个模型的顶点所有信息以、各个子网格模型
	///		以及该模型使用的蒙皮信息。同时建立一份顶点缓冲用于渲染
	/// @remarks
	///		模型的索引缓冲记录在各个子网格模型中，顶点缓冲统一记录网格模型基类中。
	///		便于骨骼动画一次性对所有顶点进行变形，同时不影响一个模型拥有多个不同材质的子网格模型的情况。
	class BaseMesh : public Resource
	{
		friend class MeshManager;
	public:
		/// @copydoc Resource::Resource(const String&)
		BaseMesh(const String& filename);
		~BaseMesh();

		// ----- BaseMesh Methods

		/// 获取子网格模型的个数
		int GetElementCount() const;

		/// 通过索引获取子网格模型
		MeshElement* GetElement(unsigned int index);

		// 使用当前数据生成顶点缓冲
		// Note: 这个目前仅仅用于骨骼动画
		//void BuildVertexBuffer();

		///	@brief
		/// 以当前的顶点创建顶点缓冲的副本
		/// @remarks
		///		这个方法用于骨骼动画模型，复制出的顶点用于为每个模型产生唯一的形变
		/// @see
		///		SkeletalMeshObject
		IVertexBuffer* BuildVertexBufferCopy();

		///	@brief
		/// 使用模型构造器创建网格模型
		/// @note
		///		如果网格模型已经加载，这个方法会清空该模型的顶点及子网格模型的现有信息
		void BuildMesh(const MeshBuilder* builder);

		/// @brief
		/// 获取指定索引的材质
		/// @param index
		///		所需获取的材质索引
		/// @remarks
		///		材质数目与子模型数目相同
		/// @returns
		///		成功获取材质返回指向材质的指针，如果索引超出材质范围则返回NULL
		BaseMaterial* GetMaterial(unsigned int index);

		/// 获取模型的包围球半径
		inline float GetBoundingRadius() const { return m_BoundingRadius; }

		/// 获取模型的OBB
		inline const OBB& GetOBB() const { return m_OBB; }

		///	@brief
		/// 通过索引获取顶点
		/// @param index
		///		顶点索引
		Vector3f GetVertexByIndex(unsigned int index) const;

		// 获取顶点数组以进行修改

		/// 获取模型的顶点个数
		inline int GetVertexCount() const { return m_VertexNum; }

		/// 获取模型的顶点数组
		inline float* GetVertexArray() { return m_VertexArray; }

		/// 获取模型的法线数组
		inline float* GetNormalArray() { return m_NormalArray; }

		/// 获取模型的颜色数组
		inline float* GetColorArray() { return m_ColorArray; }

		/// 获取模型的纹理坐标数组
		inline float* GetTexCoordArray() { return m_TexcoordArray; }

		/// 获取模型的顶点缓冲
		inline IVertexBuffer* GetVertexBuffer() const { return m_VertexBuffer; }

		/// 获取模型的顶点蒙皮信息
		inline SkinnedVertexInfo* GetSkin() const { return m_Skin; }
	protected:
		// ----- Overwrite Resource
		bool LoadImpl();
		void UnloadImpl();

		// ----- BaseMesh Methods

		/// 添加模型子元素
		void AddMeshElement(MeshElement* elem);

		/// 更新所有子网格元素以及该模型自身的OBB
		void UpdateElementsOBB();

		/// 清除所有子网格元素
		void ClearMeshElements();

	protected:
		float					m_BoundingRadius;		///< 包围球半径
		OBB						m_OBB;					///< 包围盒尺寸

		std::vector<MeshElement*>	m_MeshElements;			///< 子网格元素集合
		std::vector<BaseMaterial*>	m_Materials;			///< 子网格元素材质列表

		int						m_VertexNum;			///< 顶点数目
		float*					m_VertexArray;			///< 顶点数组
		float*					m_NormalArray;			///< 法线数组
		float*					m_ColorArray;			///< 颜色数组
		float*					m_TexcoordArray;		///< 纹理坐标数组
		IVertexBuffer*			m_VertexBuffer;			///< 模型的顶点缓冲

		SkinnedVertexInfo*		m_Skin;					///< 蒙皮信息
	};

	class MeshElementBuilder;

	// Mesh构造器

	/// @brief
	/// 网格模型构造器
	/// @par
	///		网格模型构造器可以通过逐个添加顶点的方式构造一个完整模型
	/// @remarks
	///		当前版本的构造器实现可能会有效率问题，应当避免在游戏逻辑中使用
	class MeshBuilder
	{
		friend class BaseMesh;
	public:
		/// 构造函数
		/// @param elementCount
		///		指定所构造的网格模型划分为几个子模型
		MeshBuilder(unsigned int elementCount);
		~MeshBuilder();

		/// @brief
		/// 添加一个顶点
		/// @param pos
		///		顶点的坐标
		/// @param normal
		///		顶点的法线方向
		/// @param texcoord
		///		顶点的纹理坐标
		/// @param meshElementIndex
		///		该顶点所属的子模型序号
		/// @returns
		///		如果添加顶点成功，返回true；如果给定索引超出子模型数量上限，返回false，不对当前数据进行修改
		/// @remarks
		///		添加顶点后，顶点数量递增一。如果顶点数量超过顶点容量，则进行扩容操作
		bool AddVertex(float pos[3], float normal[3], float texcoord[2], unsigned int meshElementIndex);
	private:

		/// @brief
		/// 添加一个索引
		/// @param index
		///		索引号
		/// @param meshElementIndex
		///		该索引所属的子模型序号
		void AddIndex(unsigned int index, unsigned int meshElementIndex);
	private:
		float*			m_Vertices;				///< 顶点数组
		float*			m_Normals;				///< 法线数组
		float*			m_Texcoords;			///< 纹理坐标数组

		int				m_VertexCaps;			///< 顶点容量
		int				m_VertexCount;			///< 当前顶点数量

		unsigned int	m_MeshElementCount;		///< 子网格模型数量
		MeshElementBuilder*	m_MeshElements;		///< 子网格模型构造器列表

		OBB				m_OBB;					///< 包围盒
	};

	/// @brief
	/// 子网格模型构造器
	/// @remarks
	///		这个类应当由MeshBuilder负责调用
	class MeshElementBuilder
	{
		friend class BaseMesh;
		friend class MeshBuilder;
	public:
		/// @brief
		/// 添加一个索引
		/// @param index
		///		添加的索引号
		/// @remarks
		///		这个方法会自动对内部数组进行扩容
		void AddIndex(unsigned int index);

	private:
		/// @brief
		/// 构造函数
		/// @remarks
		///		不应直接创建对象，由MeshBuilder负责调用
		MeshElementBuilder();
		~MeshElementBuilder();

	private:
		unsigned int*	m_Indices;			///< 索引数组
		int				m_IndexCaps;		///< 索引容量
		int				m_IndexCount;		///< 当前索引数量
	};
}

#endif

