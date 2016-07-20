// DirectionalFoVCamera.cpp : A Camera that has a Focus direction and a normal FoV
//

#include "stdafx.h"
#include "DirectionalFoVCamera.h"


DirectionalFoVCamera::DirectionalFoVCamera(_In_ const Vector3 & Position, _In_ const Vector3 & LookDirection, _In_ float FoV)
	:Camera(Position)
	,LookDirection(LookDirection)
	,FoV(FoV)
{
}

void DirectionalFoVCamera::UpdateCamera()
{
	DirectX::XMStoreFloat4x4(&View, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToRH(Position, LookDirection, Up)));

	DirectX::XMMATRIX Perspective = DirectX::XMMatrixPerspectiveFovRH(DirectX::XMConvertToRadians(FoV), AspectRatio, 0.001f, 100.0f);
	DirectX::XMStoreFloat4x4(&Projection, DirectX::XMMatrixTranspose(Perspective));
}
