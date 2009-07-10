#include "Existence.h"

#include <windows.h>

#include "TetrisGame.h"

#include <crtdbg.h>

//extern RendererCreator s_RendererCreator;

//IAudioSystem* Engine::Instance().AudioSystem() = 0;

int WINAPI WinMain(	HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR	lpCmdLine,
				   int		nCmdShow)
{
	Debug::EnableBreakOnAlloc();

	TetrisGame* game = new TetrisGame();
	Engine::Instance().SetGame(game);

	// ��������ʼ����Ⱦ����
	System::Instance().CreateRenderWindow("Tetris", 640, 480, 16, false);
	renderer->Initialize(System::Instance().GetRenderWindowParameters());
	//renderer->Initialize();
	Input::Instance().Initialize();

	// TODO: ��ȡ���ʱ���λ����Ⱦ����ʼ��֮����У�����Ҫ��������
	System::Instance().LoadResources("resources.cfg");

	Engine::Instance().Run();
	Engine::Instance().Shutdown();
	System::Instance().DestroyRenderWindow();

	delete game;
	return 1;
}
