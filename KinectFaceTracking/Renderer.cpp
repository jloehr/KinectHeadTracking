// Renderer.cpp : Does all the Directx stuff
//

#include "stdafx.h"
#include "Renderer.h"

#include "GraphicsContext.h"
#include "Window.h"

Renderer::Renderer(_In_ GraphicsContext & DeviceContext, _In_ Window & TargetWindow)
	:DeviceContext(DeviceContext), TargetWindow(TargetWindow)
	,RTVDescSize(0), BufferFrameIndex(0)
{
}

void Renderer::Initialize()
{ 
	if (TargetWindow.GetHandle() == nullptr)
	{
		Utility::Throw(L"Window Handle is invalid!");
	}

	CreateSwapChain();
	CreateRTVHeap();
	InitializeRenderTargets();

	CreateCommandList();

	Fence.Initialize(DeviceContext.GetDevice());
}

void Renderer::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
	SwapChainDesc.BufferCount = BufferFrameCount;
	SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.SampleDesc.Count = 1;

	Window::WindowSize Size = TargetWindow.GetWindowSize();
	SwapChainDesc.Width = Size.first;
	SwapChainDesc.Height = Size.second;

	Microsoft::WRL::ComPtr<IDXGISwapChain1> HelperSwapChain;
	Utility::ThrowOnFail(DeviceContext.GetFactory()->CreateSwapChainForHwnd(DeviceContext.GetCommandQueue().Get(), TargetWindow.GetHandle(), &SwapChainDesc, nullptr, nullptr, &HelperSwapChain));

	Utility::ThrowOnFail(HelperSwapChain.As(&SwapChain));
	BufferFrameIndex = SwapChain->GetCurrentBackBufferIndex();
}

void Renderer::CreateRTVHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
	RTVHeapDesc.NumDescriptors = BufferFrameCount;
	RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&RTVHeap)));

	RTVDescSize = DeviceContext.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void Renderer::InitializeRenderTargets()
{
	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = RTVHeap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < BufferFrameCount; ++i)
	{
		RenderTargets[i].Initialize(i, DeviceContext.GetDevice(), SwapChain, RTVHandle);
		RTVHandle.ptr += RTVDescSize;
	}
}

void Renderer::CreateCommandList()
{
	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, RenderTargets[BufferFrameIndex].GetCommandAllocator().Get(), nullptr, IID_PPV_ARGS(&CommandList)));
	Utility::ThrowOnFail(CommandList->Close());
}

D3D12_CPU_DESCRIPTOR_HANDLE Renderer::GetRTVCPUHandle()
{
	D3D12_CPU_DESCRIPTOR_HANDLE Result = RTVHeap->GetCPUDescriptorHandleForHeapStart();
	Result.ptr += BufferFrameIndex * RTVDescSize;

	return Result;
}

void Renderer::Render()
{
	RenderTarget & CurrentRenderTarget = RenderTargets[BufferFrameIndex];
	CurrentRenderTarget.BeginFrame(CommandList);

	CommandList->ClearRenderTargetView(GetRTVCPUHandle(), BackgroundColor.data(), 0, nullptr);

	CurrentRenderTarget.EndFrame(CommandList, DeviceContext.GetCommandQueue());
	Utility::ThrowOnFail(SwapChain->Present(0, 0));
	BufferFrameIndex = SwapChain->GetCurrentBackBufferIndex();
}
