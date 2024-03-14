//***************************************************************************************
// Camera.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Simple first person style camera class that lets the viewer explore the 3D scene.
//   -It keeps track of the camera coordinate system relative to the world space
//    so that the view matrix can be constructed.  
//   -It keeps track of the viewing frustum of the camera so that the projection
//    matrix can be obtained.
//***************************************************************************************

#ifndef CAMERA_H
#define CAMERA_H

#include "d3dUtil.h"
#include "Component.h"
#include "Transform.h"

#define VSCPrint(buffer, msg, ...) \
    do{ \
        _snprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, __VA_ARGS__); \
        OutputDebugStringA(buffer); \
    } while(0)
static char buff[200]; // Global within the class (in main.cpp, it's a member to avoid problems)

//class Transform;

class Camera : public Component
{
public:

	Camera();
	~Camera();

	virtual bool Initialize() override;
	virtual void Update() override;

	void UpdateCam(int ARGiNewPosX, int ARGiNewPosY);

	Transform GetTransform();

	// Getters to construct View Matrix with the Camera
	float GetTheta();
	float GetPhi();
	float GetRadius();

private:
	Transform mtrans;
	// Variables needed to rotate the Camera around
	// Phi is the angle for Up & Down, Theta is the angle for Left & Right
	float _fTheta;
	float _fPhi;
	float _fRadius = 5.0f;

	// Mouse pos stuff to do cool calculus
	POINT MousePose;

	// Limits that both the angles can reach, restricting the player's view
	float _fLIMThetaMin;
	float _fLIMThetaMax;
	float _fLIMPhiMin;
	float _fLIMPhiMax;
};

#endif // CAMERA_H