#include "InputManager.h"
#include "WindowEngine.h"
WindowEngine* wine;

InputManager::InputManager()
{
    cam = new Camera();
}

InputManager::~InputManager()
{
    delete cam;
}

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
    if ((btnState & MK_LBUTTON) != 0)
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));
        Transform& camtrans = cam->GetTransform();
        camtrans.rotateCamera(dy, dx);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;

}