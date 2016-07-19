#pragma once

#include "RenderTarget.h"
#include "GPUFence.h"
#include "Camera.h"
#include "Window.h"

class GraphicsContext;
class Model;

class Renderer
{
public:
	Renderer(_In_ GraphicsContext & DeviceContext, _In_ Window & TargetWindow);

	void Initialize();

	void Render(_In_ Model Cube);

	void OnWindowSizeChange(_In_ const Window::WindowSize & NewSize);

protected:
	typedef std::array<float, 4> DXColor;
	static constexpr DXColor BackgroundColor = { 0.0f, 0.2f, 0.4f, 1.0f };
	static constexpr UINT BufferFrameCount = 2;
	template<typename T> using BufferFrameArray = std::array<T, BufferFrameCount>;

	GraphicsContext & DeviceContext;
	Window & TargetWindow;

	Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSVHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> DepthStencelView;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;

	Camera Camera;
	D3D12_VIEWPORT Viewport;
	D3D12_RECT ScissorRect;

	BufferFrameArray<RenderTarget> RenderTargets;
	GPUFence Fence;

	UINT RTVDescSize;
	UINT BufferFrameIndex;

	void CreateSwapChain();
	void CreateRTVHeap();
	void CreateDSVHeap();
	void CreateDepthStencilView();
	void InitializeRenderTargets();

	void CreateCommandList();

	void ResizeBuffers(_In_ const Window::WindowSize & NewSize);
	void ReleaseSizeDependentBuffers();
	void ResizeSwapChain(_In_ const Window::WindowSize & NewSize);
	void RecreateSizeDependentBuffers();
	void UpdateViewportAndScissorRect(_In_ const Window::WindowSize & Size);

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle();
};
