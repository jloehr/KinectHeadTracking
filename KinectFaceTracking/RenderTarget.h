#pragma once

#include "GPUFence.h"

class RenderTarget
{
public:
	RenderTarget();

	void Initialize(_In_ UINT FrameIndex, _In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device, _In_ Microsoft::WRL::ComPtr<IDXGISwapChain3> & SwapChain, _In_ const CD3DX12_CPU_DESCRIPTOR_HANDLE & RTVHandle);
	void Release();

	void BeginFrame(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList);
	void EndFrame(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ Microsoft::WRL::ComPtr<ID3D12CommandQueue> & CommandQueue);

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> & GetCommandAllocator();

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator; 
	CD3DX12_RESOURCE_BARRIER RenderTargetTransition;
	CD3DX12_RESOURCE_BARRIER PresentTransition;
	GPUFence Fence;

	void CreateRTV(_In_ UINT FrameIndex, _In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device, _In_ Microsoft::WRL::ComPtr<IDXGISwapChain3> & SwapChain, _In_ const CD3DX12_CPU_DESCRIPTOR_HANDLE & RTVHandle);
	void CreateCommandAllocator(_In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device);

	void InitializeTransitionBarrier(_Inout_ CD3DX12_RESOURCE_BARRIER & Barrier, _In_ D3D12_RESOURCE_STATES StateBefore, _In_ D3D12_RESOURCE_STATES StateAfter);
};
