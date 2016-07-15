// Renderer.cpp : Does all the Directx stuff
//

#include "stdafx.h"
#include "Renderer.h"

#include "Window.h"

Renderer::Renderer(_In_ Window & TargetWindow)
	:TargetWindow(TargetWindow)
	,RTVDescSize(0), BufferFrameIndex(0)
{
}

void Renderer::Initialize()
{ 
	if (TargetWindow.GetHandle() == nullptr)
	{
		Utility::Throw(L"Window Handle is invalid!");
	}

	EnableDebugLayer();
	CreateFactory();
	CreateDevice();
	CreateCommandQueue();
	CreateSwapChain();
	CreateRTVHeap();
	InitializeRenderTargets();

	CreateCommandList();

	Fence.Initialize(Device);
}

void Renderer::Release()
{
	Fence.SetAndWait(CommandQueue);
}

void Renderer::EnableDebugLayer()
{
#if defined(_DEBUG)
	Microsoft::WRL::ComPtr<ID3D12Debug> DebugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController))))
	{
		DebugController->EnableDebugLayer();
	}
#endif
}

void Renderer::CreateFactory()
{
	Utility::ThrowOnFail(CreateDXGIFactory1(IID_PPV_ARGS(&Factory)));
}

void Renderer::CreateDevice()
{
	Microsoft::WRL::ComPtr<IDXGIAdapter1> HardwareAdapter;

	for (UINT AdapterIndex = 0; Factory->EnumAdapters1(AdapterIndex, &HardwareAdapter) != DXGI_ERROR_NOT_FOUND; ++AdapterIndex)
	{
		DXGI_ADAPTER_DESC1 Desc;
		HardwareAdapter->GetDesc1(&Desc);

		if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Skip the software adapter
			continue;
		}

		if (SUCCEEDED(D3D12CreateDevice(HardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&Device))))
		{
			break;
		}
	}

	if (Device == nullptr)
	{
		Utility::Throw(L"No Device was created!");
	}

}

void Renderer::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	Utility::ThrowOnFail(Device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&CommandQueue)));
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
	Utility::ThrowOnFail(Factory->CreateSwapChainForHwnd(CommandQueue.Get(), TargetWindow.GetHandle(), &SwapChainDesc, nullptr, nullptr, &HelperSwapChain));

	Utility::ThrowOnFail(HelperSwapChain.As(&SwapChain));
	BufferFrameIndex = SwapChain->GetCurrentBackBufferIndex();
}

void Renderer::CreateRTVHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
	RTVHeapDesc.NumDescriptors = BufferFrameCount;
	RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	Utility::ThrowOnFail(Device->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&RTVHeap)));

	RTVDescSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void Renderer::InitializeRenderTargets()
{
	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = RTVHeap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < BufferFrameCount; ++i)
	{
		RenderTargets[i].Initialize(i, Device, SwapChain, RTVHandle);
		RTVHandle.ptr += RTVDescSize;
	}
}

void Renderer::CreateCommandList()
{
	Utility::ThrowOnFail(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, RenderTargets[BufferFrameIndex].GetCommandAllocator().Get(), nullptr, IID_PPV_ARGS(&CommandList)));
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

	CurrentRenderTarget.EndFrame(CommandList, CommandQueue);
	Utility::ThrowOnFail(SwapChain->Present(0, 0));
	BufferFrameIndex = SwapChain->GetCurrentBackBufferIndex();
}
