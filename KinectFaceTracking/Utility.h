#pragma once

namespace Utility
{
	void Throw(_In_opt_ LPCWSTR Message = nullptr);
	void ThrowOnFail(_In_ HRESULT hr, _In_opt_ LPCWSTR Message = nullptr);
	void ThrowOnFail(_In_ HRESULT hr, _In_ Microsoft::WRL::ComPtr<ID3DBlob> & Error);
}
