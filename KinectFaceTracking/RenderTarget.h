#pragma once

#include "GPUFence.h"

class RenderTarget
{
public:
	RenderTarget();

	void Initialize(_In_ UINT FrameIndex, _In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device, _In_ Microsoft::WRL::ComPtr<IDXGISwapChain3> & SwapChain, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE & RTVHandle);
	void Release();

	void Reset();
	void SetBusy(_In_ Microsoft::WRL::ComPtr<ID3D12CommandQueue> & CommandQueue);

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> & GetCommandAllocator();

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator; 
	GPUFence Fence;

	void CreateRTV(_In_ UINT FrameIndex, _In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device, _In_ Microsoft::WRL::ComPtr<IDXGISwapChain3> & SwapChain, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE & RTVHandle);
	void CreateCommandAllocator(_In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device);
};
