#pragma once

class RenderTarget
{
public:
	RenderTarget();

	void Initialize(_In_ UINT FrameIndex, _In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device, _In_ Microsoft::WRL::ComPtr<IDXGISwapChain3> & SwapChain, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE & RTVHandle);
	void Release();

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> & GetCommandAllocator();

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator; 

	void CreateRTV(_In_ UINT FrameIndex, _In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device, _In_ Microsoft::WRL::ComPtr<IDXGISwapChain3> & SwapChain, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE & RTVHandle);
	void CreateCommandAllocator(_In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device);
};


