
#include "Existence.h"

#include "GameCollisionTest.h"

int WINAPI WinMain(HINSTANCE	hInstance,
				   HINSTANCE	hPrevInstance,
				   LPSTR		lpCmdLine,
				   int			nCmdShow)
{
	Debug::EnableBreakOnAlloc();

	IGame* game = new GameCollisionTest();
	Engine::Instance().SetGame(game);

	System::Instance().CreateRenderWindow("Collision Test", 640, 480, 32, false);
	renderer->Initialize(System::Instance().GetRenderWindowParameters());
	Input::Instance().Initialize();

	System::Instance().LoadResources("resources.cfg");

	Engine::Instance().Run();
	Engine::Instance().Shutdown();
	System::Instance().DestroyRenderWindow();

	delete game;

	return 1;
}
