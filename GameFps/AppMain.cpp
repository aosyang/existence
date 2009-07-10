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

	// 创建并初始化渲染窗口
	System::Instance().CreateRenderWindow("Existence FPS", 640, 480, 32, false);
	renderer->Initialize(System::Instance().GetRenderWindowParameters());
	Input::Instance().Initialize();

	System::Instance().LoadResources("resources.cfg");

	Engine::Instance().Run();
	Engine::Instance().Shutdown();
	System::Instance().DestroyRenderWindow();

	delete game;
	return 1;
}
