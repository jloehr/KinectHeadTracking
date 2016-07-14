#pragma once

class GPUFence
{
public:
	GPUFence();
	~GPUFence();

	void Initialize(_In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device);

	void Set(_In_ Microsoft::WRL::ComPtr<ID3D12CommandQueue> & CommandQueue);
	void Wait();
	void SetAndWait(_In_ Microsoft::WRL::ComPtr<ID3D12CommandQueue> & CommandQueue);

private:
	static const UINT64 UnsetValue = 0;
	static const UINT64 SetValue = 1;

	Microsoft::WRL::ComPtr<ID3D12Fence> Fence;
	HANDLE FenceEvent;
};

