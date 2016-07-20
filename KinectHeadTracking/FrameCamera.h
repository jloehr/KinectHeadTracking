#pragma once
#include "Camera.h"
class FrameCamera : public Camera
{
public:
	FrameCamera(_In_ const Vector3 & Position, _In_ float FrameHeight);

	virtual void UpdateCamera();

private:
	float FrameHalfHeight;
};

