#pragma once

#include "Window.h"

class Camera
{
public:
	Camera(_In_ const Vector3 & Position);

	 virtual void UpdateCamera(_In_ const Window::WindowSize & Size) = 0;

	const DirectX::XMFLOAT4X4 & GetViewMatrix() const;
	const DirectX::XMFLOAT4X4 & GetProjectionMatrix() const;

protected:
	static const Vector3 Up;

	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Projection;

	Vector3 Position;
};

