#pragma once

namespace Utility
{
	void Throw(_In_opt_ LPCWSTR Message = nullptr);
	void ThrowOnFail(_In_ HRESULT hr, _In_opt_ LPCWSTR Message = nullptr);
	void ThrowOnFail(_In_ HRESULT hr, _In_ Microsoft::WRL::ComPtr<ID3DBlob> & Error);
}

struct Vector3 {
	DirectX::XMVECTOR Vector;

	Vector3()
		:Vector3(0.0f) {}

	Vector3(_In_ float Value)
		:Vector3(Value, Value, Value) {}

	Vector3(_In_ float X, _In_ float Y, _In_ float Z)
		:Vector(DirectX::XMVectorSet(X, Y, Z, 0.0f)) {}

	Vector3(_In_ const DirectX::XMVECTOR & Vector)
		:Vector(Vector) {}

	inline operator DirectX::XMVECTOR() const { return Vector; }
};

struct Quaternion {
	DirectX::XMVECTOR Value;

	Quaternion()
		:Value(DirectX::XMQuaternionIdentity()) {}

	Quaternion(_In_ float Roll, _In_ float Pitch, _In_ float Yaw)
		:Value(DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(Roll), DirectX::XMConvertToRadians(Pitch), DirectX::XMConvertToRadians(Yaw))) {}

	inline operator DirectX::XMVECTOR() const { return Value; }
};