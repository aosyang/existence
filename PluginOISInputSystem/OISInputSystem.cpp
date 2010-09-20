//-----------------------------------------------------------------------------------
/// OISInputSystem.cpp OIS输入系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#include "OISInputSystem.h"
#include "OIS/OIS.h"
#include <sstream>

namespace Gen
{
	OISInputSystem::OISInputSystem()
		: m_Initialized(false),
		  m_InputManager(0), m_Keyboard(0), m_Mouse(0),
		  m_DeadZone(0.1f),
		  m_MouseX(0), m_MouseY(0),
		  m_EventHandler(NULL)
	{
		for (unsigned int i=0; i<MAX_MB_NUM; i++)
			m_MouseButtonState[i] = false;

		for (unsigned int i=0; i<24; i++)
			m_JoyStickButton[i] = false;

		for (unsigned int i=0; i<6; i++)
			m_JoyStickAxis[i] = 0;
	}

	void OISInputSystem::Initialize(RenderWindowHandle rw)
	{
		OIS::ParamList pl;

		std::ostringstream wnd;
#if defined __PLATFORM_WIN32
		wnd << (size_t)rw;

		pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

		//Default mode is foreground exclusive..but, we want to show mouse - so nonexclusive
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined __PLATFORM_LINUX
		wnd << (size_t)rw.window;

		pl.insert(std::make_pair(std::string("WINDOW"), wnd.str()));

		// show mouse and do not grab (confine to window)
		pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
#endif

		m_InputManager = OIS::InputManager::createInputSystem(pl);

		m_Keyboard = (OIS::Keyboard*)m_InputManager->createInputObject(OIS::OISKeyboard, true);
		m_Keyboard->setEventCallback(this);

		m_Mouse = (OIS::Mouse*)m_InputManager->createInputObject(OIS::OISMouse, true);
		m_Mouse->setEventCallback(this);

		// 尝试初始化游戏杆，由于不是所有用户都有游戏杆，存在创建设备失败的可能
		try
		{
			m_JoyStick = (OIS::JoyStick*)m_InputManager->createInputObject(OIS::OISJoyStick, true);
		}
		catch(...)
		{
			m_JoyStick = NULL;
		}
		if (m_JoyStick)
			m_JoyStick->setEventCallback(this);

		m_Initialized = true;

		//ResizeWindow();
	}

	void OISInputSystem::Shutdown()
	{
		SAFE_DELETE(m_EventHandler);
		OIS::InputManager::destroyInputSystem(m_InputManager);
	}

	void OISInputSystem::CaptureDevice()
	{
		if (m_Keyboard) m_Keyboard->capture();
		if (m_Mouse) m_Mouse->capture();
		if (m_JoyStick) m_JoyStick->capture();
	}
	
	void OISInputSystem::ResizeWindow(unsigned int width, unsigned int height)
	{
		if (m_Initialized)
		{
			const OIS::MouseState& state = m_Mouse->getMouseState();
			state.width = width;
			state.height = height;
		}
	}
	
	void OISInputSystem::SetEventHandler(IInputEventHandler* handler)
	{
		m_EventHandler = handler;
	}

	bool OISInputSystem::GetKeyDown(KeyCode key)
	{
		return m_Keyboard->isKeyDown(ToOISKeyCode(key));
	}

	bool OISInputSystem::GetMouseButtonDown(MouseButtonID ButtonCode)
	{
		return m_Mouse->getMouseState().buttonDown((OIS::MouseButtonID)ButtonCode);
	}

	int OISInputSystem::GetJoyStickAbs(int axis)
	{
		return m_JoyStickAxis[axis];
	}

	KeyCode OISInputSystem::FromOISKeyCode(OIS::KeyCode key) const
	{
		return (KeyCode)key;
	}
	
	OIS::KeyCode OISInputSystem::ToOISKeyCode(KeyCode key) const
	{
		return (OIS::KeyCode)key;
	}

	bool OISInputSystem::keyPressed(const OIS::KeyEvent &arg)
	{
		if (m_EventHandler) m_EventHandler->OnKeyPressed((KeyCode)arg.key);

		return true;
	}

	bool OISInputSystem::keyReleased(const OIS::KeyEvent &arg)
	{
		if (m_EventHandler) m_EventHandler->OnKeyReleased((KeyCode)arg.key);

		return true;
	}

	bool OISInputSystem::mouseMoved( const OIS::MouseEvent &arg )
	{
		m_MouseX = arg.state.X.abs;
		m_MouseY = arg.state.Y.abs;

		if (m_EventHandler) m_EventHandler->OnMouseMoved(arg.state.X.abs, arg.state.Y.abs, arg.state.X.rel, arg.state.Y.rel);

		return true;
	}

	bool OISInputSystem::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (m_EventHandler) m_EventHandler->OnMouseButtonPressed((MouseButtonID)id);
		return true;
	}

	bool OISInputSystem::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (m_EventHandler) m_EventHandler->OnMouseButtonReleased((MouseButtonID)id);
		return true;
	}

	bool OISInputSystem::buttonPressed( const OIS::JoyStickEvent &arg, int button )
	{
		return true;
	}

	bool OISInputSystem::buttonReleased( const OIS::JoyStickEvent &arg, int button )
	{
		return true;
	}

	bool OISInputSystem::axisMoved( const OIS::JoyStickEvent &arg, int axis )
	{
		int val = arg.state.mAxes[axis].abs;
		if (abs(val) < m_DeadZone * 0x7FFF)
			val = 0;
		m_JoyStickAxis[axis] = val;

		return true;
	}

	IPlugin* CreatePluginInstance()
	{
		return new OISInputSystem;
	}

}
