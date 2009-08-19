//-----------------------------------------------------------------------------------
/// Engine.cpp 引擎类的实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Engine.h"

#include <fstream>
#include "EString.h"

using namespace std;

#include "Plugin.h"
#include "Input.h"
#include "System.h"
#include "NullAudioSystem.h"
#include "Mesh.h"
#include "EGUIManager.h"

void LoadConfigFile(const String& filename, ConfigGroups& group, String groupname)
{

	// 读取一个配置文件，格式如下
	//
	// # 完全注释行
	// [plugins]								# 分组名
	// RenderSystem = PluginGLRenderSystem		# 键名 = 键值
	//
	// 注：在第一个分组名出现之前的键将被分配在common组中

	ifstream fs;
	fs.open(filename.Data());

	if (!fs.is_open())
		return;

	// 配置分组名，默认为common
	//string groupname = "common";

	ConfigFileKeys list;
	group[groupname] = list;

	while(1)
	{
		char buf[1024];

		fs.getline(buf, sizeof(buf));

		string line(buf);

		// 从'#'开始的内容作为注释，不予处理
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
			// 键名
			string key_name = line.substr(0, pos);

			// 去除字符串前后的空格和制表符
			header = key_name.find_first_not_of(" \t");
			tail = key_name.find_last_not_of(" \t");
			key_name = key_name.substr(header, tail - header + 1);

			// 对应的文件路径名
			string key_value = line.substr(pos+1);

			// 去除字符串前后的空格和制表符
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

	Log.MsgLn("Initializing engine");

	// 窗体消息通知
	m_MessageNotifier = new EngineMessageNotifier;
	Platform::SetMessageNotifier(m_MessageNotifier);

	LoadModules();
}

// 初始化各个系统，必须在渲染窗体创建成功后调用
// 参数：
//      bool input 是否初始化输入系统
//      使用第三方界面工具时，输入系统会发生冲突，遇到这样情况将这个参数设为false
void Engine::Initialize(bool input)
{
	// 如果窗体句柄为空，报错
	AssertFatal(System::Instance().GetRenderWindowParameters()->handle, "Engine::Initialize(): Render window handle is never expected to be null.");

	Log.MsgLn("Initializing RenderSystem");
	renderer->Initialize(System::Instance().GetRenderWindowParameters());

	Log.MsgLn("Initializing AudioSystem");
	m_AudioSystem->Initialize();

	FontManager::Instance().Initialize();

	if (input)
	{
		Input::Instance().Initialize();
	}

	EGUIManager::Instance().Initialize();

	// 创建默认的空白纹理
	// 需要在渲染器初始化以后调用
	Material* matWhite = ResourceManager<Material>::Instance().Create("#white");
	matWhite->SetLighting(false);
	unsigned int white_data = 0xFFFFFFFF;
	matWhite->SetTexture(renderer->BuildTexture("#white", 1, 1, 32, (unsigned char*)&white_data));
}

void Engine::Shutdown()
{
	EGUIManager::Instance().Shutdown();
	FontManager::Instance().Shutdown();

	Platform::SetMessageNotifier(NULL);
	delete m_MessageNotifier;

	Log.MsgLn("Shutting down audio system");
	m_AudioSystem->Shutdown();
	delete m_AudioSystem;

	// 在渲染器摧毁前清除所有Mesh
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

		// 如果没有实际渲染就不要占用CPU周期
#ifdef __PLATFORM_WIN32
		Sleep(1);
#endif	// #ifdef __PLATFORM_WIN32
	}

	m_Game->Shutdown();
	delete timer;
}

void Engine::ManualUpdate(unsigned long deltaTime)
{
	// 累计帧数
	static unsigned long elapsedTime = 0;
	static unsigned int frameNumber = 0;
	elapsedTime += deltaTime;
	frameNumber++;

	// 1秒计算一次FPS
	if (elapsedTime>1000)
	{
		m_FPS = frameNumber * 1000 / elapsedTime;

		elapsedTime = 0;
		frameNumber = 0;
	}

	Input::Instance().CaptureDevice();

	m_Game->Update(deltaTime);
	EGUIManager::Instance().Update(deltaTime);

	// 更新音频系统，删除无用的音源
	Engine::Instance().AudioSystem()->Update();

	// TODO: SceneGraph updates here

	// 响应系统消息
	Platform::HandleWindowMessage();

	if (m_Quit)
		m_Quit = m_Game->OnNotifyQuitting();

	// TODO: SceneGraph renders here.
	if (renderer->GetActive())
	{
		m_Game->RenderScene();

		// 渲染EGUI
		EGUIManager::Instance().RenderUI();

		renderer->SwapBuffer();
	}
}
void Engine::ResizeWindow(unsigned int width, unsigned int height)
{
	System::Instance().ResizeWindow(width, height);
	Input::Instance().ResizeWindow();
	m_Game->OnResizeWindow(width, height);
	renderer->ResizeRenderWindow(width, height);
}

// 从插件中读入音频及视频渲染系统
void Engine::LoadModules()
{
	Log.MsgLn("Loading plugins");
	ConfigGroups group;
	ConfigFileKeys::iterator iter;
	LoadConfigFile("plugins.cfg", group, "plugins");

	if (group.find("plugins") == group.end())
		return;

	ConfigFileKeys* list = &group["plugins"];

	// 渲染器插件名称，如OpenGL使用的CG插件等
	String RenderSystemPluginName;

	for (iter=list->begin(); iter!=list->end(); iter++)
	{
		if (iter->key == "RenderSystem")
		{
			Module_t hDLL;
			hDLL = LoadModule(iter->value.Data());

			RenderSystemFactoryCreateFunc rendererCreator = (RenderSystemFactoryCreateFunc)GetFunction(hDLL, "CreateRenderSystem");
			if (rendererCreator == NULL)
			{
				String msg;
				msg.Format("Failed to load render system from %s, unable to find entrance func CreateRenderSystem", iter->value.Data());
				Log.Error(msg);
				continue;
			}

			m_Renderer = (*rendererCreator)();

			//UnloadModule(hDLL);
		}
		else if (iter->key == "AudioSystem")
		{
			Module_t hDLL;
			hDLL = LoadModule(iter->value.Data());

			AudioSystemFactoryCreateFunc audioCreator = (AudioSystemFactoryCreateFunc)GetFunction(hDLL, "CreateAudioSystem");
			if (audioCreator == NULL)
			{
				String msg;
				msg.Format("Failed to load audio system from %s, unable to find entrance func CreateAudioSystem", iter->value.Data());
				Log.Error(msg);

				continue;
			}

			m_AudioSystem = (*audioCreator)();
		}
		else if (iter->key == "RenderSystemPlugin")
		{
			RenderSystemPluginName = iter->value;
		}
	}

	AssertFatal(m_Renderer, "Engine::LoadModules() : Failed to create render system from plugin.");
	m_Renderer->SetGpuPluginName(RenderSystemPluginName);

	// Use a null audio system if we don't find one from plugins
	if (!m_AudioSystem)
		m_AudioSystem = new NullAudioSystem();

}
