#include "Existence.h"
using namespace Gen;

#include "ParticleGame.h"


//int WINAPI WinMain(	HINSTANCE hInstance,
//				   HINSTANCE hPrevInstance,
//				   LPSTR	lpCmdLine,
//				   int		nCmdShow)
int main()
{
	Debug::EnableBreakOnAlloc();

	particleGame* game = new particleGame();
	Engine::Instance().SetGame(game);

	// 创建并初始化渲染窗口
	System::Instance().CreateRenderWindow("particle", 640, 480, 32, false);
	Engine::Instance().Initialize();

	System::Instance().LoadResources("resources.cfg");

	Engine::Instance().Run();
	Engine::Instance().Shutdown();
	System::Instance().DestroyRenderWindow();

	delete game;
	return 1;
}
