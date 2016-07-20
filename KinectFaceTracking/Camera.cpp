// Camera.cpp : Represents a Camera that'll provide a View and Projection Matrix
//

#include "stdafx.h"
#include "Camera.h"

const Vector3 Camera::Up(0.0f, 1.0f, 0.0f);

Camera::Camera(_In_ const Vector3 & Position)
	:Position(Position)
{
}

const DirectX::XMFLOAT4X4 & Camera::GetViewMatrix() const
{
	return View;
}

const DirectX::XMFLOAT4X4 & Camera::GetProjectionMatrix() const
{
	return Projection;
}
