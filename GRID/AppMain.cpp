//
// GRID, which represents for Grid-based Real-time Interactive Deployment.
//

#include "Existence.h"

#include "GameGrid.h"

#if defined __PLATFORM_WIN32
int WINAPI WinMain(HINSTANCE	hInstance,
				   HINSTANCE	hPrevInstance,
				   LPSTR		lpCmdLine,
				   int			nCmdShow)
#elif defined __PLATFORM_LINUX
int main()
#endif	// #if defined __PLATFORM_WIN32
{
	Debug::EnableBreakOnAlloc();

	IGame* game = new GameGrid();
	Engine::Instance().SetGame(game);

	System::Instance().CreateRenderWindow("Grid-based Real-time Interactive Deployment", 640, 480, 32, false);
	Engine::Instance().Initialize();

	System::Instance().LoadResources("resources.cfg");

	Engine::Instance().Run();
	Engine::Instance().Shutdown();
	System::Instance().DestroyRenderWindow();

	delete game;

	return 1;
}
