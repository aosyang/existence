#ifndef _GAMECOLLISIONTEST_H
#define _GAMECOLLISIONTEST_H

#include "Existence.h"

class GameCollisionTest : public IGame
{
public:
	GameCollisionTest();
	~GameCollisionTest();

	void StartGame();
	void Shutdown();

	bool OnNotifyQuitting();

	void OnKeyPressed(unsigned int key);
	void OnKeyReleased(unsigned int key) {}

	void OnMousePressed(unsigned int id);

	void OnResizeWindow(unsigned int width, unsigned int height);

	void Update(unsigned long deltaTime);
	void RenderScene();

	Vector3f MoveAloneRay(Ray& ray, int depth=0);
private:
	SceneGraph*	m_Scene;
	Camera*		m_Camera;
	MeshObject*	m_SceneObject;

	Light*		m_Sun;

	bool		m_ApplyGravity;

	TextUIControl*	m_UIFps;
};

#endif
