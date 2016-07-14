// RenderTarget.cpp : A Render Target is a drame of a SwapChain
//

#include "stdafx.h"
#include "RenderTarget.h"

#include "Utility.h"

RenderTarget::RenderTarget()
{
}

void RenderTarget::Initialize(_In_ UINT FrameIndex, _In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device, _In_ Microsoft::WRL::ComPtr<IDXGISwapChain3> & SwapChain, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE & RTVHandle)
{
	CreateRTV(FrameIndex, Device, SwapChain, RTVHandle);
	CreateCommandAllocator(Device);

	Fence.Initialize(Device);
}

void RenderTarget::Release()
{
}

void RenderTarget::Reset()
{
	Fence.Wait();
	CommandAllocator->Reset();
}

void RenderTarget::SetBusy(_In_ Microsoft::WRL::ComPtr<ID3D12CommandQueue> & CommandQueue)
{
	Fence.Set(CommandQueue);
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& RenderTarget::GetCommandAllocator()
{
	return CommandAllocator;
}

void RenderTarget::CreateRTV(_In_ UINT FrameIndex, _In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device, _In_ Microsoft::WRL::ComPtr<IDXGISwapChain3> & SwapChain, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE & RTVHandle)
{
	Utility::ThrowOnFail(SwapChain->GetBuffer(FrameIndex, IID_PPV_ARGS(&RenderTargetView)));
	Device->CreateRenderTargetView(RenderTargetView.Get(), nullptr, RTVHandle);
}

void RenderTarget::CreateCommandAllocator(_In_ Microsoft::WRL::ComPtr<ID3D12Device> & Device)
{
	Utility::ThrowOnFail(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocator)));
}
