#include <sal.h>
#include <wtypes.h>
// DirectXGame.cpp : Définit le point d'entrée de l'application.

#include "../DirectXMoteur/Window.h"
#include "Resource.h"
#include "../Chapter6/Box/BoxApp.cpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//MessageBox(NULL, L"Bonjour, c'est une application Windows !", L"Hello", MB_OK);
	Window win;
	win.Init();

	return 0;
}