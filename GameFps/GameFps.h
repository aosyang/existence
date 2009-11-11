#ifndef _EMPTYGAME_H
#define _EMPTYGAME_H

#include "Existence.h"
using namespace Gen;

#define MAX_DECAL_NUM 100
#define MAX_SHELL_NUM 100

typedef struct
{
	bool needUpdate;
	Vector3f vel;
	MeshObject* obj;

	float yawSpeed;
	float yawStart;

} shell_t;

class GameFps : public IGame
{
public:
	GameFps();
	~GameFps();

	void StartGame();
	void Shutdown();

	bool OnNotifyQuitting();

	void OnKeyPressed(unsigned int key);
	void OnKeyReleased(unsigned int key) {}

	void OnResizeWindow(unsigned int width, unsigned int height);

	void Update(unsigned long deltaTime);
	void RenderScene();

	void FireWeapon(unsigned long deltaTime);
	void EjectShell(const Vector3f& pos, const Vector3f& dir);

private:
	SceneGraph*	m_Scene;
	SceneGraph* m_ScreenQuadScene;

	Camera*		m_Camera;

	AudioListener*	m_AudioListener;

	// ע�� ����SceneObject������ʹ��new����Ҫʹ�÷����ڶ�ջ�ϵķ�������ֹ��ʼ��˳��������
	// TODO: �����û�ʹ�ö�ջ����ķ���
	BspObject*	m_BspScene;
	MeshObject*	m_ViewGun;
	MeshObject*	m_ViewGunFlare;
	Decal*		m_PlayerShadow;

	DistantViewObject*	m_Sky;

	Billboard*	m_Billboard;

	Light*		m_Sun;

	float		m_Angle;

	Decal*		m_Decals[MAX_DECAL_NUM];
	shell_t		m_Shells[MAX_SHELL_NUM];

	BaseUIObject*	m_UIControl;
	TextUIControl*	m_UIFps;
};

#endif
