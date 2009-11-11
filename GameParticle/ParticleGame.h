#ifndef _particleGAME_H
#define _particleGAME_H

#include "Existence.h"
using namespace Gen;

#include <list>

using namespace std;

#define MAX_particle_COUNT 100

struct particle
{
	Billboard*	bb;
	Vector3f	dir;
	long		life;
	float		scale;
	float		scale_inc;
	float		rot;
	float		rot_inc;
};

typedef list<particle> particlePool;

class particleGame : public IGame
{
public:
	particleGame();
	~particleGame();

	void StartGame();
	void Shutdown();

	bool OnNotifyQuitting();

	void OnKeyPressed(unsigned int key);
	void OnKeyReleased(unsigned int key) {}

	void OnResizeWindow(unsigned int width, unsigned int height);

	void Update(unsigned long deltaTime);
	void RenderScene();

private:
	SceneGraph*		m_Scene;
	Camera*			m_Camera;
	AudioListener*	m_AudioListener;
	TextUIControl*	m_UIFps;

	particlePool	m_particlePool;
	long			m_EmitTime;
};

void Emitparticle(const Vector3f& pos, SceneGraph* scene, particlePool& pool);


#endif
