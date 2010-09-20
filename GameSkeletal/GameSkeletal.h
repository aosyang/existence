#ifndef _GAMESKELETAL_H
#define _GAMESKELETAL_H

#include "Existence.h"
using namespace Gen;

class GameSkeletal : public IGame
{
public:
	GameSkeletal();
	~GameSkeletal();

	void StartGame();
	void Shutdown();

	bool OnNotifyQuitting();

	void OnKeyPressed(KeyCode key);
	void OnMouseMoved(int x, int y, int rel_x, int rel_y);

	void OnResizeWindow(unsigned int width, unsigned int height);

	void Update(unsigned long deltaTime);
	void RenderScene();

private:
	SceneGraph*			m_Scene;

	Camera*				m_Camera;

	DistantViewObject*	m_Sky;

	TextUIControl*		m_UIFps;

	SkeletalMeshObject*	m_SkelMesh1;
	SkeletalMeshObject*	m_SkelMesh2;
};

#endif
