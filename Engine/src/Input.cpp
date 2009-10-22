//-----------------------------------------------------------------------------------
/// Input.cpp 输入系统
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Input.h"
#include "System.h"

#include <sstream>


//using namespace OIS;

bool InputHandler::keyPressed( const OIS::KeyEvent &arg )
{
	Input::Instance().SetKeyDown((KeyCode)arg.key, true);
	Engine::Instance().GetGame()->OnKeyPressed(arg.key);

	return true;
}

bool InputHandler::keyReleased( const OIS::KeyEvent &arg )
{
	Input::Instance().SetKeyDown((KeyCode)arg.key, false);
	Engine::Instance().GetGame()->OnKeyReleased(arg.key);

	return true;

}

bool InputHandler::mouseMoved( const OIS::MouseEvent &arg )
{
	Input::Instance().SetMouseRelX(arg.state.X.rel);
	Input::Instance().SetMouseRelY(arg.state.Y.rel);

	Input::Instance().SetMouseAbsX(arg.state.X.abs);
	Input::Instance().SetMouseAbsY(arg.state.Y.abs);

	return true;
}

bool InputHandler::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	Input::Instance().SetMouseButtonDown((MouseButtonID)id, true);
	Engine::Instance().GetGame()->OnMousePressed(id);

	return true;
}

bool InputHandler::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	Input::Instance().SetMouseButtonDown((MouseButtonID)id, false);
	Engine::Instance().GetGame()->OnMouseReleased(id);

	return true;
}

bool InputHandler::buttonPressed( const OIS::JoyStickEvent &arg, int button )
{
	Input::Instance().SetJoyStickButtonDown(button, true);

	return true;
}

bool InputHandler::buttonReleased( const OIS::JoyStickEvent &arg, int button )
{
	Input::Instance().SetJoyStickButtonDown(button, false);

	return true;
}

bool InputHandler::axisMoved( const OIS::JoyStickEvent &arg, int axis )
{
	int val = arg.state.mAxes[axis].abs;
	if (abs(val) < Input::Instance().GetJoyStickDeadZone() * 0x7FFF)
		val = 0;
	Input::Instance().SetJoyStickAbs(axis, val);
	return true;
}


//Input* Singleton<Input>::m_Singleton = 0;

Input::Input()
: m_Initialized(false),
  m_InputManager(0), m_Keyboard(0), m_Mouse(0), 
  m_DeadZone(0.1f),
  m_MouseRelX(0), m_MouseRelY(0),
  m_MouseAbsX(0), m_MouseAbsY(0)
{
	for (unsigned int i=0; i<MAX_KC_NUM; i++)
		m_KeyState[i] = false;

	for (unsigned int i=0; i<MAX_MB_NUM; i++)
		m_MouseButtonState[i] = false;

	for (unsigned int i=0; i<24; i++)
		m_JoyStickButton[i] = false;

	for (unsigned int i=0; i<6; i++)
		m_JoyStickAxis[i] = 0;
}

// 输入系统初始化
//     初始化行为依赖RenderWindowHandle
void Input::Initialize()
{
	Log.MsgLn("Initializing input system");
	OIS::ParamList pl;

	std::ostringstream wnd;
#if defined __PLATFORM_WIN32
	wnd << (size_t)System::Instance().GetRenderWindowParameters()->handle;

	pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

	//Default mode is foreground exclusive..but, we want to show mouse - so nonexclusive
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined __PLATFORM_LINUX
	wnd << (size_t)System::Instance().GetRenderWindowParameters()->handle.window;

	pl.insert(std::make_pair(std::string("WINDOW"), wnd.str()));

	// show mouse and do not grab (confine to window)
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
#endif

	m_InputManager = OIS::InputManager::createInputSystem(pl);

	m_Keyboard = (OIS::Keyboard*)m_InputManager->createInputObject(OIS::OISKeyboard, true);
	m_Keyboard->setEventCallback( &m_Handler );

	m_Mouse = (OIS::Mouse*)m_InputManager->createInputObject(OIS::OISMouse, true);
	m_Mouse->setEventCallback( &m_Handler );

	try
	{
		m_JoyStick = (OIS::JoyStick*)m_InputManager->createInputObject(OIS::OISJoyStick, true);
	}
	catch(...)
	{
		m_JoyStick = NULL;
	}
	if (m_JoyStick)
		m_JoyStick->setEventCallback(&m_Handler);

	m_Initialized = true;

	ResizeWindow();
}

void Input::Shutdown()
{
	Log.MsgLn("Shutting down input system");
	OIS::InputManager::destroyInputSystem(m_InputManager);
}

void Input::CaptureDevice()
{
	m_MouseRelX = m_MouseRelY = 0;
	if (m_Keyboard) m_Keyboard->capture();
	if (m_Mouse) m_Mouse->capture();
	if (m_JoyStick) m_JoyStick->capture();
}

void Input::ResizeWindow()
{
	if (m_Initialized)
	{
		RenderWindowParam* param = System::Instance().GetRenderWindowParameters();
		const OIS::MouseState& state = m_Mouse->getMouseState();
		state.width = param->width;
		state.height = param->height;
	}
}

