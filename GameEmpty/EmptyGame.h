#ifndef _EMPTYGAME_H
#define _EMPTYGAME_H

#include "Existence.h"

class EmptyGame : public IGame
{
public:
	EmptyGame();
	~EmptyGame();

	void StartGame();
	void Shutdown();

	bool OnNotifyQuitting();

	void OnKeyPressed(unsigned int key);
	void OnKeyReleased(unsigned int key) {}

	void OnResizeWindow(unsigned int width, unsigned int height);

	void Update(unsigned long deltaTime);
	void RenderScene();

private:
	SceneGraph*	m_Scene;

	Camera*		m_Camera1;
	Camera*		m_Camera2;
	Camera*		m_CurrentCamera;

	AudioListener*	m_AudioListener;

	BspObject*	m_BspScene;
	Mesh*		m_BoxMesh;
	Mesh*		m_CharacterMesh;
	MeshObject*	m_Box[3];
	MeshObject*	m_CameraAttachedBox;

	Billboard*	m_Billboard;

	Light*		m_Sun;
	Light*		m_Light;

	float		m_Angle;

	BaseUIObject*	m_UIControl;
	TextUIControl*	m_UIFps;
};

#endif
