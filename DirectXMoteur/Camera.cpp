// Include needed classes
#include "Camera.h"
#include <DirectXMath.h>
#include <windows.h>



Camera::Camera()
{
	_fLIMThetaMin = 0.0f;
	_fLIMThetaMax = 0.0f;
	_fLIMPhiMin = 0.1f;
	_fLIMPhiMax = DirectX::XM_PI - 0.1f;
}

Camera::~Camera()
{

}

bool Camera::Initialize()
{
	return false;
}

void Camera::Update()
{
}

// This method needs to be called every time the player moves the mouse (or every frame if we go cave-man mode)
void Camera::UpdateCam(int ARGiNewPosX, int ARGiNewPosY)
{
	VSCPrint(buff, "=>UpdateCam(int ARGiNewPosX = %d, int ARGiNewPosY = %d)\n", ARGiNewPosX, ARGiNewPosY);
	// Calculates the difference in movement of the mouse
	int iDx = ARGiNewPosX - MousePose.x;
	int iDy = ARGiNewPosY - MousePose.y;

	// Applies a factor on the movement of the mouse. You can think of it as the sensibility of the mouse movements.
	float fDegAngleX = 0.25f * static_cast<float>(iDx);
	float fDegAngleY = 0.25f * static_cast<float>(iDy);
	VSCPrint(buff, "\tfDegAngleX = %.5f ||| fDegAngleY = %.5f\n", fDegAngleX, fDegAngleY);

	// Converts both distances in radians, so we interpret the "magnitude" of the mouse move by actual Camera movement
	float fRadAngleX = DirectX::XMConvertToRadians(fDegAngleX);
	float fRadAngleY = DirectX::XMConvertToRadians(fDegAngleY);
	VSCPrint(buff, "\tfRadAngleX = %.5f ||| fRadAngleY = %.5f\n", fRadAngleX, fRadAngleY);

	// Updates both the angles
	_fTheta += fRadAngleX;
	_fPhi += fRadAngleY;

	// Finally, restrict angles to limit where the player can see
	//_fPhi = _fPhi < _fLIMPhiMin ? _fLIMPhiMin : (_fPhi > _fLIMPhiMax ? _fLIMPhiMax : _fPhi);

	// Updates the mouse position
	MousePose.x = ARGiNewPosX;
	MousePose.y = ARGiNewPosY;

}

Transform Camera::GetTransform()
{
	return mtrans;
}

float Camera::GetTheta()
{
	return _fTheta;
}

float Camera::GetPhi()
{
	return _fPhi;
}

float Camera::GetRadius()
{
	return _fRadius;
}
