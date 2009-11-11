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

	// ��������ʼ����Ⱦ����
	System::Instance().CreateRenderWindow("Tetris", 640, 480, 16, false);
	Engine::Instance().Initialize();

	// TODO: ��ȡ���ʱ���λ����Ⱦ����ʼ��֮����У�����Ҫ��������
	System::Instance().LoadResources("resources.cfg");

	Engine::Instance().Run();
	Engine::Instance().Shutdown();
	System::Instance().DestroyRenderWindow();

	delete game;
	return 1;
}
