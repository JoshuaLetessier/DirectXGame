#include "Transform.h"


void Transform::identity()
{
	vPos.x = 0.0f;
	vPos.y = 0.0f;
	vPos.z = 0.0f;

	mPos._11 = mPos._22 = mPos._33 = mPos._44 = 1.0f;
	mPos._12 = mPos._13 = mPos._14 = 0.0f;
	mPos._21 = mPos._23 = mPos._24 = 0.0f;
	mPos._31 = mPos._32 = mPos._34 = 0.0f;
	mPos._41 = mPos._42 = mPos._43 = 0.0f;


}
