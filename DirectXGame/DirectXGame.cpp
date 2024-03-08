#include <sal.h>
#include <wtypes.h>
// DirectXGame.cpp : Définit le point d'entrée de l'application.
#include "..\DirectXMoteur\RenderEngine.h"

#include "Resource.h"
#include "LunchGame.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

LunchGame lunch;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtMemState memStateInit;
	_CrtMemCheckpoint(&memStateInit);
#endif

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		RenderEngine renderEngine(hInstance);
		if (!renderEngine.Initialize())
			return 0;

		return renderEngine.Run();
		lunch.run();

	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
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
