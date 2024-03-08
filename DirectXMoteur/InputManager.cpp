#include "InputManager.h"

void InputManager::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void InputManager::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void InputManager::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	//else if ((btnState & MK_LBUTTON) != 0)
	//{
	//}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

POINT InputManager::mousePointeur()
{
	POINT mousePoint;
	if (GetCursorPos(&mousePoint)) {
		return mousePoint;
	}
}