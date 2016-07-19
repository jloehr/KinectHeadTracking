// Camera.cpp : Represents a Camera that'll provide a View and Projection Matrix
//

#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
}

void Camera::UpdateCamera(Window::WindowSize Size)
{
	float AspectRatio = static_cast<float>(Size.first) / static_cast<float>(Size.second);

	DirectX::XMMATRIX Perspective = DirectX::XMMatrixPerspectiveFovRH(
		DirectX::XMConvertToRadians(90.f),
		AspectRatio,
		0.01f,
		100.0f
	);

	DirectX::XMStoreFloat4x4(&Projection, DirectX::XMMatrixTranspose(Perspective));

	DirectX::XMVECTORF32 Eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	DirectX::XMVECTORF32 At = { 0.0f, -0.1f, 0.0f, 0.0f };
	DirectX::XMVECTORF32 Up = { 0.0f, 1.0f, 0.0f, 0.0f };

	DirectX::XMStoreFloat4x4(&View, DirectX::XMMatrixTranspose(XMMatrixLookAtRH(Eye, At, Up)));
}

const DirectX::XMFLOAT4X4 & Camera::GetViewMatrix()
{
	return View;
}

const DirectX::XMFLOAT4X4 & Camera::GetProjectionMatrix()
{
	return Projection;
}
