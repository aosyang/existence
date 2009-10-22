//-----------------------------------------------------------------------------------
/// Engine.h �����࣬���Ȳ��ģ������Ϸ�߼�����
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ENGINE_H
#define _ENGINE_H

#include "Singleton.h"
#include "IRenderer.h"
#include "IAudioSystem.h"
#include "Platform.h"
#include "IGame.h"
#include "Debug.h"
#include "Timer.h"
#include "Font.h"

#include <map>
#include <vector>
#include "EString.h"

using namespace std;

// ��Ⱦ���궨��
#define renderer Engine::Instance().Renderer()


struct ConfigFileLine
{
	String key;
	String value;
};

typedef vector<ConfigFileLine> ConfigFileKeys;
typedef map<const String, ConfigFileKeys> ConfigGroups;

// ���������ļ�
void LoadConfigFile(const String& filename, ConfigGroups& group, String groupname = "common");

class EngineMessageNotifier : public PlatformMessageNotifier
{
public:
	~EngineMessageNotifier() {}

	void OnMessageQuit();

	void OnMessageActiveWindow(bool active);

	void OnMessageResizeWindow(unsigned int width, unsigned int height);
};


class Engine : public Singleton<Engine>
{
	friend class Singleton<Engine>;
public:
	void Initialize(bool input=true);
	void Shutdown();

	void Run();
	void ManualUpdate(unsigned long deltaTime);

	void ResizeWindow(unsigned int width, unsigned int height);

	unsigned int GetFPS() const { return m_FPS; }

	void ToggleDebugRender(bool debugRender) { m_DebugRender = debugRender; }
	bool GetDebugRender() const { return m_DebugRender; }

	//void LoadTextures();

	//-----------------------------------------------------------------------------------
	/// \brief
	/// �����Ⱦ���ӿ�
	///
	/// \returns
	/// ������ʹ�õ���Ⱦ���ӿ�
	///
	/// �û�ͨ������ӿڿ���ֱ�Ӳ�����Ⱦ��
	//-----------------------------------------------------------------------------------
	inline IRenderer* Renderer() { return m_Renderer; }

	//-----------------------------------------------------------------------------------
	/// \brief
	/// �����Ƶϵͳ�ӿ�
	///
	/// \returns
	/// ������ʹ�õ���Ƶϵͳ�ӿ�
	///
	/// �û�ͨ������ӿڿ���ֱ�Ӳ�����Ƶϵͳ
	//-----------------------------------------------------------------------------------
	inline IAudioSystem* AudioSystem() { return m_AudioSystem; }

	inline void SetRenderBatchCount(unsigned int count) { m_RenderBatchCount = count; }
	inline unsigned int GetRenderBatchCount() const { return m_RenderBatchCount; }

	inline void SetQuitting(bool quit) { m_Quit = quit; }

	inline void SetGame(IGame* game) { m_Game = game; }
	inline IGame* GetGame() { return m_Game; }

private:
	void LoadModules();

private:
	Engine();

	IRenderer*		m_Renderer;
	IAudioSystem*	m_AudioSystem;

	unsigned int	m_RenderBatchCount;

	EngineMessageNotifier* m_MessageNotifier;

	IGame*			m_Game;
	bool			m_Quit;

	bool			m_DebugRender;

	// FPS���
	unsigned int	m_FPS;
};

#endif