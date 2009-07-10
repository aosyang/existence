// Name: minimal.cpp
// Purpose: Minimal wxWidgets sample
// Author: Julian Smart

#include "wx/wx.h"
#include "Existence.h"
#include "GameEditor.h"

// ����Ӧ�ó�����
class AppWxEngine : public wxApp
{
public:
	// ������������ڳ���������ʱ�򱻵���
	virtual bool OnInit();

};

// ������������
class FrameEngine : public wxFrame
{
public:
	// ��������Ĺ��캯��
	FrameEngine(const wxString& title);
	~FrameEngine();

	// �¼�������
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	void OnIdle(wxIdleEvent& event);

	void Notify();

	void OnTimer(wxTimerEvent& event);

	//void OnCloseWindow(wxCloseEvent& event);
	void OnSize(wxSizeEvent& event);

private :
	// �����¼���
	DECLARE_EVENT_TABLE()
protected:
	GameEditor*			m_GameEditor;

	Timer*				m_ExistTimer;
	unsigned long		m_LastTime;

	wxTimer				m_Timer;
#define TIMER_ID	1000
};

// ������һ�оͿ���ʹ�� AppWxEngine& wxGetApp()��
DECLARE_APP(AppWxEngine)

IMPLEMENT_APP(AppWxEngine)

bool AppWxEngine::OnInit()
{
	FrameEngine *frame = new FrameEngine(wxT("Minimal wxWidgets App"));
	//dxFrame *frame = new dxFrame(NULL, wxID_ANY, wxT("Test"));

	frame->Show(true);

	return true;
}

BEGIN_EVENT_TABLE(FrameEngine, wxFrame)
EVT_MENU(wxID_ABOUT, FrameEngine::OnAbout)
EVT_MENU(wxID_EXIT, FrameEngine::OnQuit)
EVT_IDLE(FrameEngine::OnIdle)
EVT_TIMER(TIMER_ID, FrameEngine::OnTimer)
//EVT_CLOSE(FrameEngine::OnCloseWindow)
EVT_SIZE(FrameEngine::OnSize)
END_EVENT_TABLE()
void FrameEngine::OnAbout(wxCommandEvent& event)
{
	wxString msg;
	msg.Printf(wxT("Hello and welcome to %s"),
		wxVERSION_STRING);
	wxMessageBox(msg, wxT("About Minimal"),
		wxOK | wxICON_INFORMATION, this);
}

void FrameEngine::OnQuit(wxCommandEvent& event)
{
	Close() ;
}

void FrameEngine::OnIdle(wxIdleEvent& event)
{
}

// ʹ��Timer������Ⱦ���ĸ���
void FrameEngine::OnTimer(wxTimerEvent& event)
{

	unsigned long deltaTime = m_ExistTimer->GetMilliseconds() - m_LastTime;
	m_LastTime = m_ExistTimer->GetMilliseconds();

	Engine::Instance().ManualUpdate(deltaTime);
	//m_Timer.get
	//int time = event.GetInterval();
	//int timerInterval = m_Timer.GetInterval();

	WXHWND hWnd = GetHWND();

	RECT rect;
	GetWindowRect(static_cast<HWND>(hWnd), &rect);

	wxString status;

	status = wxString::Format(wxT("left: %i top: %i right: %i bottom: %i"), rect.left, rect.top, rect.right, rect.bottom);
	//status = wxString::Format(wxT("Hello!"));
	//status = wxString::Format(wxT("time: %d"), time);

	SetStatusText(status);

}
//
//void FrameEngine::OnCloseWindow(wxCloseEvent &event)
//{
//	//m_Timer.Stop();
//	//Engine::Instance().Shutdown();
//	//System::Instance().DestroyRenderWindow();
//}

void FrameEngine::OnSize(wxSizeEvent& event)
{
	WXHWND hWnd = GetHWND();

	RECT rect;
	GetWindowRect(static_cast<HWND>(hWnd), &rect);

	renderer->ResizeRenderWindow(rect.right - rect.left, rect.bottom - rect.top);
}

//#include "mondrian.xpm"

FrameEngine::FrameEngine(const wxString& title)
: wxFrame(NULL, wxID_ANY, title), m_GameEditor(NULL), m_ExistTimer(NULL), m_Timer(this, TIMER_ID)
{
	//SetIcon(wxIcon(mondrian_xpm));

	// ���������ʱ��
	m_ExistTimer = new Timer();

	m_LastTime = m_ExistTimer->GetMilliseconds();
	m_ExistTimer->Reset();

	wxMenu *fileMenu = new wxMenu;

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
		wxT("Show about dialog"));
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"),
		wxT("Quit this program"));

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(helpMenu, wxT("&Help"));

	SetMenuBar(menuBar);

	CreateStatusBar(2);
	SetStatusText(wxT("Welcome to wxWidgets!"));

	m_Timer.Start(1);

	//mRenderer = new D3DRenderer();
	WXHWND hWnd = GetHWND();

	m_GameEditor = new GameEditor();
	Engine::Instance().SetGame(m_GameEditor);

	//mRenderer->Initialize(static_cast<HWND>(hWnd));
	System::Instance().SetRenderWindowHandle(static_cast<RenderWindowHandle>(hWnd));
	renderer->Initialize(System::Instance().GetRenderWindowParameters());
	Input::Instance().Initialize();

	System::Instance().LoadResources("resources.cfg");

	// ���ȫ����Դ������ٿ�ʼ��Ϸ
	m_GameEditor->StartGame();
}

FrameEngine::~FrameEngine()
{
	//m_Timer.Stop();
	m_GameEditor->Shutdown();
	Engine::Instance().Shutdown();

	SAFE_DELETE(m_GameEditor);
	SAFE_DELETE(m_ExistTimer);
}
