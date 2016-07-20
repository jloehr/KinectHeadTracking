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
	static constexpr UINT64 BusyValue = 0;
	static constexpr UINT64 ReadyValue = 1;

	Microsoft::WRL::ComPtr<ID3D12Fence> Fence;
	HANDLE FenceEvent;
};
