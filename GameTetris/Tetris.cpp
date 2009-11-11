#include "Existence.h"
using namespace Gen;

#include "TetrisGame.h"

#if defined __PLATFORM_WIN32
int WINAPI WinMain(	HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR	lpCmdLine,
				   int		nCmdShow)
#elif defined __PLATFORM_LINUX
int main()
#endif
{
	Debug::EnableBreakOnAlloc();

	TetrisGame* game = new TetrisGame();
	Engine::Instance().SetGame(game);

	// 创建并初始化渲染窗口
	System::Instance().CreateRenderWindow("Tetris", 640, 480, 16, false);
	Engine::Instance().Initialize();

	// TODO: 读取材质必须位于渲染器初始化之后进行，故需要作出调整
	System::Instance().LoadResources("resources.cfg");

	Engine::Instance().Run();
	Engine::Instance().Shutdown();
	System::Instance().DestroyRenderWindow();

	delete game;
	return 1;
}
