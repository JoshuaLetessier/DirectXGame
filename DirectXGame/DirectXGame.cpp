//#include <sal.h>
//#include <wtypes.h>
// DirectXGame.cpp : Définit le point d'entrée de l'application.
#include "..\DirectXMoteur\WindowEngine.h"
#include"DirectXGame.h"
#include "Resource.h"
#include "LunchGame.h"


WindowEngine* WindowEngine::mApp;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#ifdef _DEBUG
	_CrtMemState memStateInit;
	_CrtMemCheckpoint(&memStateInit);
#endif
	try
	{
		WindowEngine::mApp = nullptr;
		WindowEngine window(hInstance);

		if (window.Initialize())
		{
			window.Run(&window);
			//start
			// Draw
			// 
			//ennemies.spawnEnnemies();

		}

	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		//return 0;
	}
#ifdef _DEBUG
	_CrtMemState memStateEnd, memStateDiff;
	_CrtMemCheckpoint(&memStateEnd);
	if (_CrtMemDifference(&memStateDiff, &memStateInit, &memStateEnd))
	{
		MessageBoxA(NULL, "MEMORY LEAKS", "DISCLAIMER", 0);
	}
#endif 
}