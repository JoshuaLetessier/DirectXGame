#include "InputManager.h"
#include "WindowEngine.h"

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



//void InputManager::UpdateCamera(float deltaTime) 
//{
//    const float dt = deltaTime;
//
//    if (GetAsyncKeyState('W') & 0x8000)
//        cam->Walk(10.0f * dt);
//
//    if (GetAsyncKeyState('S') & 0x8000)
//        cam->Walk(-10.0f * dt);
//
//    if (GetAsyncKeyState('A') & 0x8000)
//        cam->Strafe(-10.0f * dt);
//
//    if (GetAsyncKeyState('D') & 0x8000)
//        cam->Strafe(10.0f * dt);
//
//    cam->UpdateViewMatrix();
//}    if (GetCursorPos(&mousePoint)) {