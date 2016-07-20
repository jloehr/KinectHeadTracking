// Transform.cpp : An objects position, rotation and scale in 3D space
//

#include "stdafx.h"
#include "Transform.h"

Transform::Transform(_In_ Vector3 Position, _In_ Quaternion Rotation, _In_ Vector3 Scale)
	:Position(Position), Rotation(Rotation), Scale(Scale)
{
	UpdateMatrix();
}

const DirectX::XMFLOAT4X4 & Transform::GetMatrix() const
{
	return Matrix;
}

void Transform::UpdateMatrix()
{
	DirectX::XMStoreFloat4x4(&Matrix, DirectX::XMMatrixTranspose(DirectX::XMMatrixScalingFromVector(Scale) * DirectX::XMMatrixRotationQuaternion(Rotation) * DirectX::XMMatrixTranslationFromVector(Position)));
}
