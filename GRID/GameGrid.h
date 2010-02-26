#ifndef _GAMEGRID_H
#define _GAMEGRID_H

// GRID需要实现的功能：
// 世界以三维格子为单位
// 每个障碍物摆在格子上

#include "Existence.h"
using namespace Gen;

#include "Point3.h"
#include <set>
#include <list>

using namespace std;

#pragma warning(disable : 4996)

#define WORLD_SIZE 50


class WorldGrid
{
public:
	Point3		pos;
	MeshObject* obj;

	unsigned int neighbourCount;
	Point3		 neighbourPos[12];
	float		 length[12];

	float		 priority;

	WorldGrid()
	{
		obj = NULL;
		neighbourCount = 0;

		for (int i=0; i<12; i++)
		{
			length[i] = 0.0f;
		}

		priority = 0.0f;
	}

	bool HasNeighbour(Point3 pos)
	{
		for (unsigned int i=0; i<this->neighbourCount; i++)
		{
			if (this->neighbourPos[i] == pos)
				return true;
		}

		return false;
	}

	void AddNeighbour(const Point3& pos, float len)
	{
		this->neighbourPos[neighbourCount] = pos;
		length[neighbourCount] = len;

		neighbourCount++;
	}

};

class WorldGridComparer : public less<WorldGrid>
{
public:
	bool operator()(WorldGrid* const lhs, WorldGrid* const rhs) const
	{
		return lhs->priority < rhs->priority;
	}
};

typedef set<WorldGrid*>		NodeList;
typedef set<WorldGrid*, WorldGridComparer>	OpenList;
typedef list<WorldGrid*>	PathList;

enum OperationMode
{
	MODE_ADD,
	MODE_REMOVE,
	//MODE_SHOW_NEIGHBOUR,
	MODE_SET_START,
	MODE_SET_GOAL,
	NUM_MODE,
};


class GameGrid : public IGame
{
public:
	GameGrid();
	~GameGrid();

	void StartGame();
	void Shutdown();

	bool OnNotifyQuitting();

	void OnKeyPressed(unsigned int key);
	void OnKeyReleased(unsigned int key) {}

	void OnMousePressed(unsigned int id);
	void OnMouseReleased(unsigned int id) {}

	void OnResizeWindow(unsigned int width, unsigned int height);

	void Update(unsigned long deltaTime);
	void RenderScene();

	bool GetIntersectionPoint(Vector3f& point);
	void AddBox(const Point3& pos);
	void RemoveBox(const Point3& pos);
	void BuildLinkingPath();
	void BuildNodePath(const Point3& pos);
	void AddGridNeighbour(WorldGrid*, const Point3& pos, float len);
	//void ShowNeighbour(const Point3& pos);
	void PathFinding(const Point3& start, const Point3& end);

	void MarkerGo(unsigned long deltaTime);

	bool SaveScene(const String& filename);
	bool LoadScene(const String& filename);

private:
	SceneGraph*		m_Scene;
	Camera*			m_Camera;
	TextUIControl*	m_UIFps;

	Light*			m_Sun;

	PrimitiveMesh*	m_MeshPlane;
	MeshObject*		m_BasePlane;

	PrimitiveMesh*	m_MeshBox;
	Material*		m_BoxMaterial;
	MeshObject*		m_IntersectionObject;

	MeshObject*		m_MarkerStart;
	MeshObject*		m_MarkerGoal;

	float			m_Rotation;

	bool			m_TestRun;
	unsigned int	m_TimeToRun;

	WorldGrid		m_World[WORLD_SIZE][WORLD_SIZE][WORLD_SIZE];
	int				m_Mode;
	String			m_ModeName;

	int				m_Layer;

	Point3			m_Start;
	Point3			m_Goal;

	OpenList		m_OpenList;
	NodeList		m_CloseList;

	PathList		m_Path;
	Vector3f		m_LastPos;
};

#endif
