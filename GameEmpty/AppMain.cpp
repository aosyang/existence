#include "Existence.h"

#include "EmptyGame.h"

#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(	HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR	lpCmdLine,
				   int		nCmdShow)
#endif
{
	Debug::EnableBreakOnAlloc();

	EmptyGame* game = new EmptyGame();
	Engine::Instance().SetGame(game);

	// 创建并初始化渲染窗口
	System::Instance().CreateRenderWindow("Empty", 640, 480, 32, false);
	renderer->Initialize(System::Instance().GetRenderWindowParameters());
	Input::Instance().Initialize();

	System::Instance().LoadResources("resources.cfg");

	Engine::Instance().Run();
	Engine::Instance().Shutdown();
	System::Instance().DestroyRenderWindow();

	delete game;
	return 0;
}
