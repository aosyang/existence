#ifndef _GAMEEDITOR_H
#define _GAMEEDITOR_H

#include "Existence.h"

class GameEditor : public IGame
{
public:
	GameEditor();
	~GameEditor();
	void StartGame();
	void Shutdown();

	bool OnNotifyQuitting();

	void OnKeyPressed(unsigned int key);
	void OnKeyReleased(unsigned int key);

	void Update(unsigned long deltaTime);
	void RenderScene();

private:
	SceneGraph*	m_Scene;

	Camera*		m_Camera;
	Mesh*		m_BoxMesh;
	MeshObject	m_Box[3];

	Material*	m_MatRed;
	Material*	m_MatGreen;
	Material*	m_MatBlue;

	float		m_Angle;

};

#endif
