// Camera.cpp : Represents a Camera that'll provide a View and Projection Matrix
//

#include "stdafx.h"
#include "Camera.h"

const Vector3 Camera::Up(0.0f, 1.0f, 0.0f);

Camera::Camera(_In_ const Vector3 & Position)
	:Position(Position), AspectRatio(1.f)
{
}

void Camera::UpdateCamera(_In_ const Window::WindowSize & Size)
{
	AspectRatio = static_cast<float>(Size.first) / static_cast<float>(Size.second);

	UpdateCamera();
}

void Camera::UpdateCamera(_In_ const Vector3 & Position)
{
	this->Position = Position;

	UpdateCamera();
}

const DirectX::XMFLOAT4X4 & Camera::GetViewMatrix() const
{
	return View;
}

const DirectX::XMFLOAT4X4 & Camera::GetProjectionMatrix() const
{
	return Projection;
}
