#pragma once

#include "Window.h"

class Camera
{
public:
	Camera();

	void UpdateCamera(_In_ const Window::WindowSize & Size);

	const DirectX::XMFLOAT4X4 & GetViewMatrix();
	const DirectX::XMFLOAT4X4 & GetProjectionMatrix();

private:
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Projection;
};

