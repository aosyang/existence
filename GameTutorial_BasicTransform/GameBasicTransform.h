#ifndef _GAMEBASICTRANSFORM_H
#define _GAMEBASICTRANSFORM_H

#include "Existence.h"

class GameBasicTransform : public IGame
{
public:
	GameBasicTransform();
	~GameBasicTransform();

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

	Mesh*		m_BoardMesh;
	MeshObject*	m_TutorialBoard;
	Material*	m_BoardMaterial;

	Mesh*		m_BoxMesh;
	MeshObject*	m_TutorialBox;
	Material*	m_BoxMaterial;
};

#endif
