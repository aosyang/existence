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



#include "Plugin.h"
#include "Input.h"
#include "System.h"
#include "NullAudioSystem.h"
#include "EmdMesh.h"
#include "EGUIManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "SkeletonManager.h"
#include "AudioManager.h"
#include "DebugRenderer.h"
#include "RenderTarget.h"

namespace Gen
{
	void LoadConfigFile(const String& filename, ConfigGroups& group, String groupName)
	{

		// ��ȡһ�������ļ�����ʽ����
		//
		// # ��ȫע����
		// [plugins]								# ������
		// RenderSystem = PluginGLRenderSystem		# ���� = ��ֵ
		//
		// ע���ڵ�һ������������֮ǰ�ļ�����������common����

		std::ifstream fs;
		fs.open(filename.Data());

		if (!fs.is_open())
			return;

		// ���÷�������Ĭ��Ϊcommon
		//string groupName = "common";

		ConfigFileKeys list;
		group[groupName] = list;

		while(1)
		{
			char buf[1024];

			fs.getline(buf, sizeof(buf));

			String line(buf);

			// ��'#'��ʼ��������Ϊע�ͣ����账��
			size_t comment = line.Find("#");
			if (comment!=String::npos)
				line = line.Substr(0, comment);

			size_t header, tail;

			if (((header = line.FindFirstNotOf(" \t\r"))!=String::npos) &&
				((tail = line.FindLastNotOf(" \t\r"))!=String::npos))
			{
				if (line[header]=='[' && line[tail]==']')
				{
					groupName = line.Substr(header + 1, tail - header - 1);
					groupName.Trim();
					continue;
				}
			}

			size_t pos = line.Find("=");
			if (pos != String::npos)
			{
				// ����
				String keyName = line.Substr(0, pos);
				keyName.Trim();

				// ��Ӧ���ļ�·����
				String keyValue = line.Substr(pos+1);
				keyValue.Trim();

				if (keyName.Size() && keyValue.Size())
				{
					ConfigFileLine line;
					line.key = keyName;
					line.value = keyValue;

					group[groupName].push_back(line);
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
		Renderer::Instance().SetActive(active);									// Program Is Active
	}

	void EngineMessageNotifier::OnMessageResizeWindow(unsigned int width, unsigned int height)
	{
		Engine::Instance().ResizeWindow(width, height);
	}

	Engine::Engine()
	: m_AudioSystem(NULL),
	  m_InputSystem(NULL),
	  m_RenderBatchCount(0),
	  m_MessageNotifier(NULL),
	  m_Game(NULL),
	  m_Quit(false),
	  m_FPS(0)
	{
		// �������ԣ���ȷ��ȡ�����������ļ�
		std::locale   langLocale("");
		setlocale(LC_ALL, langLocale.name().data());

		IOLogReciever* logReciever = new IOLogReciever();
		logReciever->OpenFileForOutput("Engine.log");
		Log.AddReciever(logReciever);

		Log.MsgLn("Initializing engine");

		// ������Ϣ֪ͨ
		m_MessageNotifier = new EngineMessageNotifier;
		Platform::SetMessageNotifier(m_MessageNotifier);

		LoadModules();
	}

	// ��ʼ������ϵͳ����������Ⱦ���崴���ɹ������
	// ������
	//      bool input �Ƿ��ʼ������ϵͳ
	//      ʹ�õ��������湤��ʱ������ϵͳ�ᷢ����ͻ������������������������Ϊfalse
	void Engine::Initialize(bool input)
	{
		// ���������Ϊ�գ�����

		//AssertFatal(System::Instance().GetRenderWindowParameters()->handle, "Engine::Initialize(): Render window handle is never expected to be null.");

		RenderWindowParam* param = System::Instance().GetRenderWindowParameters();

		Renderer::Instance().Initialize(param);
		DebugRenderer::Instance().Initialize();

		Log.MsgLn("Initializing AudioSystem");
		m_AudioSystem->Initialize();

		SceneObjectFactory::RegisterDefaultFactories();

		FontManager::Instance().Initialize();

		if (input && m_InputSystem)
		{
			m_InputSystem->Initialize(param->handle);
		}

		EGUIManager::Instance().Initialize();

		// ���´��ڳߴ�
		ResizeWindow(param->width, param->height);

		// ����Ĭ�ϵĿհ�����
		// ��Ҫ����Ⱦ����ʼ���Ժ����
		Material* matWhite = MaterialManager::Instance().Create("#white");
		matWhite->SetLighting(false);
		unsigned int white_data = 0xFFFFFFFF;

		Texture2D* whiteTex = TextureManager::Instance().CreateTexture2D("#white");
		whiteTex->Create(1, 1, 32, (unsigned char*)&white_data);
		matWhite->SetTexture(whiteTex);
	}

	void Engine::Shutdown()
	{
		// ����ȫ����Դ
		MeshManager::Instance().UnloadAllResources();
		TextureManager::Instance().UnloadAllResources();
		MaterialManager::Instance().UnloadAllResources();
		FontManager::Instance().UnloadAllResources();
		SkeletonManager::Instance().UnloadAllResources();
		AudioManager::Instance().UnloadAllResources();

		RenderTarget::DestroyAllObjects();

		// ж��ϵͳ
		EGUIManager::Instance().Shutdown();
		FontManager::Instance().Shutdown();

		Platform::SetMessageNotifier(NULL);
		delete m_MessageNotifier;

		Log.MsgLn("Shutting down audio system");
		m_AudioSystem->Shutdown();
		delete m_AudioSystem;

		DebugRenderer::Instance().Shutdown();
		Renderer::Instance().Shutdown();
		
		if (m_InputSystem)
		{
			m_InputSystem->Shutdown();
			SAFE_DELETE(m_InputSystem);
		}

		// ɾ��ȫ�����
		std::vector<Plugin*>::iterator iter;
		for (iter=m_Plugins.begin(); iter!=m_Plugins.end(); iter++)
		{
			delete (*iter);
		}
		m_Plugins.clear();
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
	#ifdef __PLATFORM_WIN32
			Sleep(1);
	#elif defined __PLATFORM_LINUX
			usleep(1000);	// ��λ��΢��us
	#endif	// #ifdef __PLATFORM_WIN32
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

		if (m_InputSystem)
		{
			m_InputSystem->CaptureDevice();
		}

		m_Game->Update(deltaTime);
		EGUIManager::Instance().Update(deltaTime);

		// ������Ƶϵͳ��ɾ�����õ���Դ
		//Engine::Instance().AudioSystem()->Update();

		// TODO: SceneGraph updates here

		// ��Ӧϵͳ��Ϣ
		Platform::HandleWindowMessage();

		if (m_Quit)
			m_Quit = m_Game->OnNotifyQuitting();

		// TODO: SceneGraph renders here.
		//if (Renderer::Instance().GetActive())
		{
			Renderer::Instance().ClearBuffer();

			m_Game->RenderScene();

			// ��ȾEGUI
			EGUIManager::Instance().RenderUI();

			DebugRenderer::Instance().ClearBuffer();

			Renderer::Instance().SwapBuffer();
		}
	}
	void Engine::ResizeWindow(unsigned int width, unsigned int height)
	{
		System::Instance().ResizeWindow(width, height);
		if (m_InputSystem)
		{
			m_InputSystem->ResizeWindow(width, height);
		}
		m_Game->OnResizeWindow(width, height);
		Renderer::Instance().ResizeRenderWindow(width, height);
	}

	// �Ӳ���ж�����Ƶ����Ƶ��Ⱦϵͳ
	void Engine::LoadModules()
	{
		Log.MsgLn("Loading plugins");
		ConfigGroups group;
		ConfigFileKeys::iterator iter;
		LoadConfigFile("plugins.cfg", group, "plugins");
		
		AssertFatal(group.find("plugins")!=group.end(), "Engine::LoadModules(): Unable to find sector \"plugins\".");

		ConfigFileKeys* list = &group["plugins"];

		// ��Ⱦ��������ƣ���OpenGLʹ�õ�CG�����
		String renderSystemPluginName;

		for (iter=list->begin(); iter!=list->end(); iter++)
		{
			String msg;
			Plugin* plugin = new Plugin;
			if (plugin->LoadModule(iter->value))
			{
				IPlugin* iplugin = plugin->CreatePluginInstance();
				if (iplugin)
				{
					switch (iplugin->GetPluginType())
					{
					case PLUGIN_TYPE_RENDER_SYSTEM:
						Renderer::Instance().SetRenderDevice(static_cast<IRenderDevice*>(iplugin));
						break;
					case PLUGIN_TYPE_AUDIO_SYSTEM:
						m_AudioSystem = static_cast<IAudioSystem*>(iplugin);
						break;
					case PLUGIN_TYPE_INPUT_SYSTEM:
						m_InputSystem = static_cast<IInputSystem*>(iplugin);
						break;
					default:
						{
							msg.Format("Ignored unrecognized plugin type in %s.", iter->value.Data());
							Log.Warning(msg);

							SAFE_DELETE(plugin);

							continue;
						}
						break;
					}

					m_Plugins.push_back(plugin);
				}
				else
				{
					msg.Format("Unable to create plugin instance from %s.", iter->value.Data());
					Log.Warning(msg);
					SAFE_DELETE(plugin);
				}
			}
			else
			{
				msg.Format("Unable to open plugin from file in %s.", iter->value.Data());
				Log.Warning(msg);
				SAFE_DELETE(plugin);
			}
		}

		AssertFatal(Renderer::Instance().GetRenderDevice(), "Engine::LoadModules() : Failed to create render system from plugin.");

		// Use a null audio system if we don't find one from plugins
		if (!m_AudioSystem)
			m_AudioSystem = new NullAudioSystem();

	}
}
