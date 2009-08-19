//-----------------------------------------------------------------------------------
/// MeshElement.h 网格元素类，保存具有单一纹理的子网格
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MESHELEMENT_H
#define _MESHELEMENT_H

#include "Vector3f.h"
#include "Debug.h"
#include "MeshObject.h"
#include "Material.h"
#include "OBB.h"
#include "Mesh.h"
#include "IVertexBuffer.h"

#include <fstream>

class Mesh;

using namespace std;

class MeshElement
{
	friend bool LoadMesh_NoVersion(Mesh* mesh, ifstream& fin);
	friend bool LoadMeshElement_NoVersion(MeshElement* mesh, ifstream& fin, float& boundingRadius);

public:
	MeshElement();
	~MeshElement();

	void CreateBox(float side);

	void CreatePositiveYPlane(float side);
	void CreatePositiveZPlane(float side);

	// 通过索引获取三角形
	void GetFaceByIndex(unsigned int index, unsigned int& x, unsigned int& y, unsigned int& z)
	{
		AssertFatal(index<m_FaceNum, "Mesh::GetFaceByIndex() : Index out of boundary.");

		x = m_FaceArray[index*3];
		y = m_FaceArray[index*3+1];
		z = m_FaceArray[index*3+2];
	}

	// 通过索引获取顶点
	const Vector3f GetVertexByIndex(unsigned int index)
	{
		AssertFatal(index<m_VertexNum, "Mesh::GetVertexByIndex() : Index out of boundary.");

		float px = m_VertexArray[index*3];
		float py = m_VertexArray[index*3+1];
		float pz = m_VertexArray[index*3+2];

		return Vector3f(px, py, pz);
	}

	unsigned int GetFaceNum() const { return m_FaceNum; }

	void UpdateOBB();
	OBB& GetOBB() { return m_OBB; }

	void BuildVertexBufferData();

	unsigned int*	m_FaceArray;		///< 三角形数组
	
	float*			m_VertexArray;		///< 顶点数组
	float*			m_NormalArray;		///< 法线数组
	float*			m_TexCoordArray;	///< 纹理坐标数组

	unsigned int	m_VertexNum;
	unsigned int	m_FaceNum;

	IVertexBuffer*	m_VertexBuffer;

	Material*		m_Material;

private:
	OBB				m_OBB;
};


bool LoadMeshElement_NoVersion(MeshElement* mesh, ifstream& fin, float& boundingRadius);
bool LoadMeshElement_010(MeshElement* mesh, ifstream& fin);


#endif
