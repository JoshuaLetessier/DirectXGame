#include <sal.h>
#include <wtypes.h>
// DirectXGame.cpp : Définit le point d'entrée de l'application.

#include "../DirectXMoteur/Window.h"
#include "Resource.h"
#include "LunchGame.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//MessageBox(NULL, L"Bonjour, c'est une application Windows !", L"Hello", MB_OK);
	Window win;
    LunchGame lunchGame;
	win.Init();

    try
    {
        return lunchGame.run();
    }
    catch (DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}
