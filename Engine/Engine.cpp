//-----------------------------------------------------------------------------------
/// Engine.cpp �������ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Engine.h"

#include <fstream>
#include "EString.h"

using namespace std;

#include "Input.h"
#include "System.h"
#include "NullAudioSystem.h"
#include "Mesh.h"

void LoadConfigFile(const String& filename, ConfigGroups& group, String groupname)
{

	// ��ȡһ�������ļ�����ʽ����
	//
	// # ��ȫע����
	// [plugins]								# ������
	// RenderSystem = PluginGLRenderSystem		# ���� = ��ֵ
	//
	// ע���ڵ�һ������������֮ǰ�ļ�����������common����

	ifstream fs;
	fs.open(filename.Data());

	if (!fs.is_open())
		return;

	// ���÷�������Ĭ��Ϊcommon
	//string groupname = "common";

	ConfigFileKeys list;
	group[groupname] = list;

	while(1)
	{
		char buf[1024];

		fs.getline(buf, sizeof(buf));

		string line(buf);

		// ��'#'��ʼ��������Ϊע�ͣ����账��
		size_t comment = line.find("#");
		if (comment!=string::npos)
			line = line.substr(0, comment);

		size_t header, tail;

		if (((header = line.find_first_not_of(" \t"))!=string::npos) &&
			((tail = line.find_last_not_of(" \t"))!=string::npos))
		{
			if (line[header]=='[' && line[tail]==']')
			{
				groupname = line.substr(header + 1, tail - header - 1);
				header = groupname.FindFirstNotOf(" \t");
				tail = groupname.FindLastNotOf(" \t");
				groupname = groupname.Substr(header, tail - header + 1);
				continue;
			}
		}

		size_t pos = line.find("=");
		if (pos != string::npos)
		{
			// ����
			string key_name = line.substr(0, pos);

			// ȥ���ַ���ǰ��Ŀո���Ʊ��
			header = key_name.find_first_not_of(" \t");
			tail = key_name.find_last_not_of(" \t");
			key_name = key_name.substr(header, tail - header + 1);

			// ��Ӧ���ļ�·����
			string key_value = line.substr(pos+1);

			// ȥ���ַ���ǰ��Ŀո���Ʊ��
			header = key_value.find_first_not_of(" \t");
			tail = key_value.find_last_not_of(" \t");
			key_value = key_value.substr(header, tail - header + 1);

			if (key_name.size() && key_value.size())
			{
				ConfigFileLine line;
				line.key = key_name;
				line.value = key_value;

				group[groupname].push_back(line);
			}
		}


		if (fs.eof())
			break;

	}

	fs.close();
}

void EngineMessageNotifier::OnMessageQuit()
{
	Engine::Instance().SetQuitting(true);
}

void EngineMessageNotifier::OnMessageActiveWindow(bool active)
{
	renderer->SetActive(active);									// Program Is Active
}

void EngineMessageNotifier::OnMessageResizeWindow(unsigned int width, unsigned int height)
{
	// TODO: ResizeWindow����̫�࣬��Ҫ���д���
	System::Instance().ResizeWindow(width, height);
	Input::Instance().ResizeWindow();
	Engine::Instance().ResizeWindow(width, height);
}

Engine::Engine()
: m_Renderer(NULL),
  m_AudioSystem(NULL),
  m_MessageNotifier(NULL),
  m_Game(NULL),
  m_Quit(false),
  m_DebugRender(false),
  m_FPS(0)
{
	Log.CreateLog("engine.log");

	Initialize();
}

void Engine::Initialize()
{
	Log.MsgLn("Initializing engine");
	m_MessageNotifier = new EngineMessageNotifier;
	Platform::SetMessageNotifier(m_MessageNotifier);

	LoadPlugins();

	AssertFatal(m_Renderer, "Engine::Initialize() : Failed to create render system from plugin.");

	// Use a null audio system if we don't find one from plugins
	if (!m_AudioSystem)
		m_AudioSystem = new NullAudioSystem();
	//AssertFatal(m_AudioSystem, "Engine::Initialize() : Failed to create audio system from plugin.");

	Log.MsgLn("Initializing audio system");
	m_AudioSystem->Initialize();

	FontManager::Instance().Initialize();
}

void Engine::Shutdown()
{
	FontManager::Instance().Shutdown();

	Platform::SetMessageNotifier(NULL);
	delete m_MessageNotifier;

	Log.MsgLn("Shutting down audio system");
	m_AudioSystem->Shutdown();
	delete m_AudioSystem;

	// ����Ⱦ���ݻ�ǰ�������Mesh
	ResourceManager<Mesh>::Instance().UnloadAllResources();

	Log.MsgLn("Shutitng down renderer");
	m_Renderer->Shutdown();
	delete m_Renderer;

	Input::Instance().Shutdown();
}

void Engine::Run()
{
	AssertFatal(m_Game, "Engine::Run() : NULL game found. Did you forget to call Engine::SetGame() method?");

	m_Game->StartGame();

	Timer* timer = new Timer();
	unsigned long lastTime = timer->GetMilliseconds();

	timer->Reset();

	while (!m_Quit)
	{
		unsigned long deltaTime = timer->GetMilliseconds() - lastTime;
		lastTime = timer->GetMilliseconds();
		ManualUpdate(deltaTime);

		// ���û��ʵ����Ⱦ�Ͳ�Ҫռ��CPU����
		Sleep(1);
	}

	m_Game->Shutdown();
	delete timer;
}

void Engine::ManualUpdate(unsigned long deltaTime)
{
	// �ۼ�֡��
	static unsigned long elapsedTime = 0;
	static unsigned int frameNumber = 0;
	elapsedTime += deltaTime;
	frameNumber++;

	// 1�����һ��FPS
	if (elapsedTime>1000)
	{
		m_FPS = frameNumber * 1000 / elapsedTime;

		elapsedTime = 0;
		frameNumber = 0;
	}

	Input::Instance().CaptureDevice();

	m_Game->Update(deltaTime);

	// ������Ƶϵͳ��ɾ�����õ���Դ
	Engine::Instance().AudioSystem()->Update();

	// TODO: SceneGraph updates here

	// ��Ӧϵͳ��Ϣ
	Platform::HandleWindowMessage();

	if (m_Quit)
		m_Quit = m_Game->OnNotifyQuitting();

	// TODO: SceneGraph renders here.
	if (renderer->GetActive())
		m_Game->RenderScene();

	//renderer->Render();
}
void Engine::ResizeWindow(unsigned int width, unsigned int height)
{
	m_Game->OnResizeWindow(width, height);
	renderer->ResizeRenderWindow(width, height);
}

void Engine::LoadPlugins()
{
	Log.MsgLn("Loading plugins");
	ConfigGroups group;
	ConfigFileKeys::iterator iter;
	LoadConfigFile("plugins.cfg", group, "plugins");

	if (group.find("plugins") == group.end())
		return;

	ConfigFileKeys* list = &group["plugins"];

	for (iter=list->begin(); iter!=list->end(); iter++)
	{
		if (iter->key == "RenderSystem")
		{
			HINSTANCE hDLL;
			hDLL = LoadLibrary(iter->value.Data());

			RenderSystemFactoryCreateFunc rendererCreator = (RenderSystemFactoryCreateFunc)GetProcAddress(hDLL, "CreateRenderSystem");
			if (rendererCreator == NULL)
			{
				String msg;
				msg.Format("Failed to load render system from %s, unable to find entrance func CreateRenderSystem", iter->value.Data());
				Log.Error(msg);
				continue;
			}

			m_Renderer = (*rendererCreator)();

			//FreeLibrary(hDLL);
		}
		else if (iter->key == "AudioSystem")
		{
			HINSTANCE hDLL;
			hDLL = LoadLibrary(iter->value.Data());

			AudioSystemFactoryCreateFunc audioCreator = (AudioSystemFactoryCreateFunc)GetProcAddress(hDLL, "CreateAudioSystem");
			if (audioCreator == NULL)
			{
				String msg;
				msg.Format("Failed to load audio system from %s, unable to find entrance func CreateAudioSystem", iter->value.Data());
				Log.Error(msg);

				continue;
			}

			m_AudioSystem = (*audioCreator)();
		}
	}
}
