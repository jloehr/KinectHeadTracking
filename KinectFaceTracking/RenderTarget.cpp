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
	InitializeTransitionBarrier(RenderTargetTransition, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	InitializeTransitionBarrier(PresentTransition, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	Fence.Initialize(Device);
}

void RenderTarget::Release()
{
}

void RenderTarget::BeginFrame(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList)
{
	Fence.Wait();
	CommandAllocator->Reset();
	Utility::ThrowOnFail(CommandList->Reset(CommandAllocator.Get(), nullptr));
	
	CommandList->ResourceBarrier(1, &RenderTargetTransition);

}

void RenderTarget::EndFrame(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ Microsoft::WRL::ComPtr<ID3D12CommandQueue> & CommandQueue)
{
	CommandList->ResourceBarrier(1, &PresentTransition);

	Utility::ThrowOnFail(CommandList->Close());

	ID3D12CommandList * CommandListPointer = CommandList.Get();
	CommandQueue->ExecuteCommandLists(1, &CommandListPointer);

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

void RenderTarget::InitializeTransitionBarrier(_Inout_ D3D12_RESOURCE_BARRIER & Barrier, _In_ D3D12_RESOURCE_STATES StateBefore, _In_ D3D12_RESOURCE_STATES StateAfter)
{
	ZeroMemory(&Barrier, sizeof(Barrier));
	Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	Barrier.Transition.pResource = RenderTargetView.Get();
	Barrier.Transition.StateBefore = StateBefore;
	Barrier.Transition.StateAfter = StateAfter;
	Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
}