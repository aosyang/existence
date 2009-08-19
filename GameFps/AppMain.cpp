#include "Existence.h"

#include "GameFps.h"


int WINAPI WinMain(	HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR	lpCmdLine,
				   int		nCmdShow)
{
	Debug::EnableBreakOnAlloc();

	GameFps* game = new GameFps();
	Engine::Instance().SetGame(game);

	// ��������ʼ����Ⱦ����
	//System::Instance().CreateRenderWindow("Test window", 100, 100, 32, false);	// �ര�崴������

	System::Instance().CreateRenderWindow("Existence FPS", 640, 480, 32, false);
	Engine::Instance().Initialize();

	System::Instance().LoadResources("resources.cfg");

	Engine::Instance().Run();
	Engine::Instance().Shutdown();
	System::Instance().DestroyRenderWindow();

	delete game;
	return 1;
}
