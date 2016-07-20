// FrameCamera.cpp : A Camera whose frustum will always exactly pass a frame around origin
//

#include "stdafx.h"
#include "FrameCamera.h"


FrameCamera::FrameCamera(_In_ const Vector3 & Position, _In_ float FrameHeight)
	:Camera(Position)
	,FrameHalfHeight(FrameHeight/2.f)
{
}

void FrameCamera::UpdateCamera()
{
	DirectX::XMStoreFloat4x4(&View, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToRH(Position, Vector3(0.0f, 0.0f, -1.0f), Up)));

	Vector3 InvertedPositon = DirectX::XMVectorNegate(Position);
	float FrameHalfWidth = AspectRatio * FrameHalfHeight;
	float NearZ = (Position.Z >= 0.0f) ? Position.Z : InvertedPositon.Z;

	DirectX::XMMATRIX Perspective = DirectX::XMMatrixPerspectiveOffCenterRH(InvertedPositon.X - FrameHalfWidth, InvertedPositon.X + FrameHalfWidth, InvertedPositon.Y - FrameHalfHeight, InvertedPositon.Y + FrameHalfHeight, NearZ, 1000.f);
	DirectX::XMStoreFloat4x4(&Projection, DirectX::XMMatrixTranspose(Perspective));
}

