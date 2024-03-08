#pragma once
#include "pch.h"
#include "Camera.h"

static Camera* cam;

class InputManager
{
public:
	InputManager();
	~InputManager();

	POINT mLastMousePos;
	HWND  mhMainWnd = nullptr; // main window handle

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

public:
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void UpdateCamera(float deltaTime);
};
