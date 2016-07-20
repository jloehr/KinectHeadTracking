#pragma once

#include "Camera.h"

class DirectionalFoVCamera : public Camera
{
public:
	DirectionalFoVCamera(_In_ const Vector3 & Position, _In_ const Vector3 & LookDirection = Vector3(0.0f, 0.0f, -1.0f), _In_ float FoV = 90.0f);

	virtual void UpdateCamera();

private:
	Vector3 LookDirection;
	float FoV;
};

