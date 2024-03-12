#pragma once
#include "pch.h"

class InputManager
{
private:


public:

	POINT mLastMousePos;
	HWND  mhMainWnd = nullptr; // main window handle

protected:


public:
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
};
