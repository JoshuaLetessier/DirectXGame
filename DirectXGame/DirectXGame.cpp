#include <sal.h>
#include <wtypes.h>
// DirectXGame.cpp : Définit le point d'entrée de l'application.
//
#include "../DirectXMoteur/Window.h"
#include "Resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window win;
	win.Init();

	return 0;
}



