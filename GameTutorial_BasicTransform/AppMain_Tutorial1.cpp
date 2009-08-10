//-----------------------------------------------------------------------------------
/// AppMain_Tutorial1.cpp 引擎教程1，基本变换的实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Existence.h"
#include "GameBasicTransform.h"

#include <windows.h>

int WINAPI WinMain(	HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR	lpCmdLine,
				   int		nCmdShow)
{
	Debug::EnableBreakOnAlloc();

	GameBasicTransform* game = new GameBasicTransform();
	Engine::Instance().SetGame(game);

	System::Instance().CreateRenderWindow("Basic Transform Sample", 640, 480, 16, false);
	Engine::Instance().Initialize();

	System::Instance().LoadResources("resources.cfg");


	Engine::Instance().Run();
	Engine::Instance().Shutdown();

	delete game;

	return 0;
}