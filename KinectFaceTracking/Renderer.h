#pragma once

#include "RenderTarget.h"

class Window;

class Renderer
{
public:
	Renderer(_In_ Window & TargetWindow);

	void Initialize();
	void Release();

	void Render();

protected:
	typedef std::array<float, 4> DXColor;
	static constexpr DXColor BackgroundColor = { 0.0f, 0.2f, 0.4f, 1.0f };
	static constexpr UINT BufferFrameCount = 2;
	template<typename T> using BufferFrameArray = std::array<T, BufferFrameCount>;

	Window & TargetWindow;

	Microsoft::WRL::ComPtr<IDXGIFactory4> Factory;
	Microsoft::WRL::ComPtr<ID3D12Device> Device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;

	BufferFrameArray<RenderTarget> RenderTargets;

	UINT RTVDescSize;
	UINT BufferFrameIndex;

	void EnableDebugLayer();
	void CreateFactory();
	void CreateDevice();
	void CreateCommandQueue();
	void CreateSwapChain();
	void CreateRTVHeap();
	void InitializeRenderTargets();

	void CreateCommandList();

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle();
};
