// Renderer.cpp : Does all the Directx stuff
//

#include "stdafx.h"
#include "Renderer.h"

#include "GraphicsContext.h"
#include "Window.h"

#include "Model.h"

Renderer::Renderer(_In_ GraphicsContext & DeviceContext, _In_ Window & TargetWindow)
	:DeviceContext(DeviceContext), TargetWindow(TargetWindow)
	,Viewport(), ScissorRect()
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
	CreateDSVHeap();
	CreateDepthStencilView();
	InitializeRenderTargets();

	CreateCommandList();

	Camera.UpdateCamera(TargetWindow.GetWindowSize());
	UpdateViewportAndScissorRect();

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

void Renderer::CreateDSVHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc = {};
	DSVHeapDesc.NumDescriptors = 1;
	DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(&DSVHeap)));
}

void Renderer::CreateDepthStencilView()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
	DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE DSVClearValue = {};
	DSVClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	DSVClearValue.DepthStencil.Depth = 1.0f;
	DSVClearValue.DepthStencil.Stencil = 0;

	Window::WindowSize Size = TargetWindow.GetWindowSize();
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, Size.first, Size.second, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &DSVClearValue, IID_PPV_ARGS(&DepthStencelView)));

	DeviceContext.GetDevice()->CreateDepthStencilView(DepthStencelView.Get(), &DepthStencilViewDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());
}

void Renderer::InitializeRenderTargets()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());

	for (UINT i = 0; i < BufferFrameCount; ++i)
	{
		RenderTargets[i].Initialize(i, DeviceContext.GetDevice(), SwapChain, RTVHandle);
		RTVHandle.Offset(RTVDescSize);
	}
}

void Renderer::CreateCommandList()
{
	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, RenderTargets[BufferFrameIndex].GetCommandAllocator().Get(), nullptr, IID_PPV_ARGS(&CommandList)));
	Utility::ThrowOnFail(CommandList->Close());
}

void Renderer::UpdateViewportAndScissorRect()
{
	Window::WindowSize Size = TargetWindow.GetWindowSize();

	Viewport.Width = static_cast<float>(Size.first);
	Viewport.Height = static_cast<float>(Size.second);
	Viewport.MaxDepth = 1.0f;

	ScissorRect.right = static_cast<LONG>(Size.first);
	ScissorRect.bottom = static_cast<LONG>(Size.second);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE Renderer::GetRTVCPUHandle()
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(RTVHeap->GetCPUDescriptorHandleForHeapStart(), BufferFrameIndex, RTVDescSize);
}

void Renderer::Render(_In_ Model Cube)
{
	RenderTarget & CurrentRenderTarget = RenderTargets[BufferFrameIndex];
	CurrentRenderTarget.BeginFrame(CommandList);

	CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(GetRTVCPUHandle());
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVHandle(DSVHeap->GetCPUDescriptorHandleForHeapStart());

	CommandList->OMSetRenderTargets(1, &RTVHandle, FALSE, &DSVHandle);
	CommandList->ClearRenderTargetView(GetRTVCPUHandle(), BackgroundColor.data(), 0, nullptr);
	CommandList->ClearDepthStencilView(DSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	CommandList->RSSetViewports(1, &Viewport);
	CommandList->RSSetScissorRects(1, &ScissorRect);

	// Render Objects
	Cube.Render(CommandList, Camera);

	CurrentRenderTarget.EndFrame(CommandList, DeviceContext.GetCommandQueue());
	Utility::ThrowOnFail(SwapChain->Present(0, 0));
	BufferFrameIndex = SwapChain->GetCurrentBackBufferIndex();
}
