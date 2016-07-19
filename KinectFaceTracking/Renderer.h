#pragma once

#include "RenderTarget.h"
#include "GPUFence.h"

class Window;
class GraphicsContext;

class Renderer
{
public:
	Renderer(_In_ GraphicsContext & DeviceContext, _In_ Window & TargetWindow);

	void Initialize();

	void Render();

protected:
	typedef std::array<float, 4> DXColor;
	static constexpr DXColor BackgroundColor = { 0.0f, 0.2f, 0.4f, 1.0f };
	static constexpr UINT BufferFrameCount = 2;
	template<typename T> using BufferFrameArray = std::array<T, BufferFrameCount>;

	GraphicsContext & DeviceContext;
	Window & TargetWindow;

	Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;

	BufferFrameArray<RenderTarget> RenderTargets;
	GPUFence Fence;

	UINT RTVDescSize;
	UINT BufferFrameIndex;

	void CreateSwapChain();
	void CreateRTVHeap();
	void InitializeRenderTargets();

	void CreateCommandList();

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle();
};
