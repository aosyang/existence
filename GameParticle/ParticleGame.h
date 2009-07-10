#ifndef _PARTICLEGAME_H
#define _PARTICLEGAME_H

#include "Existence.h"
#include <list>

using namespace std;

#define MAX_PARTICLE_COUNT 100

struct Particle
{
	Billboard*	bb;
	Vector3f	dir;
	long		life;
};

typedef list<Particle> ParticlePool;

class ParticleGame : public IGame
{
public:
	ParticleGame();
	~ParticleGame();

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
	TextUIControl*	m_UIFps;

	ParticlePool	m_ParticlePool;
	long			m_EmitTime;
};

void EmitParticle(const Vector3f& pos, SceneGraph* scene, ParticlePool& pool);


#endif
