#pragma once
#include "pch.h"

class InputManager
{
public:

	POINT mLastMousePos;
	HWND  mhMainWnd = nullptr; // main window handle

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

public:
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
};
