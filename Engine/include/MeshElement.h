//-----------------------------------------------------------------------------------
/// MeshElement.h 网格元素类，保存具有单一材质的子网格
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MESHELEMENT_H
#define _MESHELEMENT_H

#include "Vector3f.h"
#include "Debug.h"
#include "Material.h"
#include "OBB.h"
#include "IVertexBuffer.h"

#include <fstream>

using namespace std;

namespace Gen
{
	class MeshElementBuilder;

	class MeshElement
	{
	public:
		MeshElement();
		~MeshElement();

		// 从数据创建模型
		void CreateMeshData(unsigned int vertexNum,
			unsigned int faceNum,
			unsigned int* faceArray,
			float* vertexArray,
			float* normalArray,
			float* texcoordArray);

		void CreateMeshData(MeshElementBuilder* builder);

		void ClearData();

		// 通过索引获取三角形
		void GetFaceByIndex(unsigned int index, unsigned int& x, unsigned int& y, unsigned int& z) const
		{
			AssertFatal(index<m_FaceNum, "MeshElement::GetFaceByIndex() : Index out of boundary.");

			x = m_FaceArray[index*3];
			y = m_FaceArray[index*3+1];
			z = m_FaceArray[index*3+2];
		}

		// 通过索引获取顶点
		Vector3f GetVertexByIndex(unsigned int index) const
		{
			AssertFatal(index<m_VertexNum, "MeshElement::GetVertexByIndex() : Index out of boundary.");

			float px = m_VertexArray[index*3];
			float py = m_VertexArray[index*3+1];
			float pz = m_VertexArray[index*3+2];

			return Vector3f(px, py, pz);
		}

		unsigned int GetFaceNum() const { return m_FaceNum; }

		inline IVertexBuffer* GetVertexBuffer() const { return m_VertexBuffer; }

		OBB& GetOBB() { return m_OBB; }

	private:
		// 遍历顶点，更新模型的包围盒信息
		void UpdateOBB();

		// 使用当前数据构造顶点缓冲
		void BuildVertexBufferData();

	private:
		unsigned int*	m_FaceArray;		///< 三角形数组

		float*			m_VertexArray;		///< 顶点数组
		float*			m_NormalArray;		///< 法线数组
		float*			m_TexCoordArray;	///< 纹理坐标数组

		unsigned int	m_VertexNum;
		unsigned int	m_FaceNum;

		IVertexBuffer*	m_VertexBuffer;

		OBB				m_OBB;
	};


	// 模型元素构造器
	// 通过一个一个添加顶点的方式构造模型元素
	// 自动生成顶点索引
	//
	// 注意：这个东西生成索引缓冲的过程可能会很慢！！！
	class MeshElementBuilder
	{
		friend class MeshElement;
	public:
		MeshElementBuilder();
		~MeshElementBuilder();

		void AddVertex(float pos[3], float normal[3], float texcoord[2]);
	private:
		void AddIndex(unsigned int index);
	private:
		unsigned int*	m_Indices;
		float*			m_Vertices;
		float*			m_Normals;
		float*			m_Texcoords;

		int				m_VertexCaps;
		int				m_VertexCount;
		int				m_IndexCaps;
		int				m_IndexCount;

		OBB				m_OBB;
	};

}

#endif

