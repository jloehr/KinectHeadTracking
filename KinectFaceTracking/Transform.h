#pragma once

class Transform;

typedef std::vector<Transform> TransformList;

class Transform
{
public:
	Transform(_In_ Vector3 Position = Vector3(), _In_ Quaternion Rotation = Quaternion(), _In_ Vector3 Scale = Vector3(1.f));
	
	const DirectX::XMFLOAT4X4 & GetMatrix() const;

private:
	DirectX::XMFLOAT4X4 Matrix;

	Vector3 Position;
	Quaternion Rotation;
	Vector3 Scale;

	void UpdateMatrix();
};


