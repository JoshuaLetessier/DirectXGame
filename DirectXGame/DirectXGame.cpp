#include <sal.h>
#include <wtypes.h>
// DirectXGame.cpp : D�finit le point d'entr�e de l'application.
//
#include "../DirectXMoteur/Window.h"
#include "Resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window win;
	win.Init();
	MessageBox(NULL, L"Bonjour, c'est une application Windows basique !", L"Hello", MB_OK);

	return 0;
}



