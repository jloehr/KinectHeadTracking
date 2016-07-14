#pragma once

class Window;

class Renderer
{
public:
	Renderer(_In_ Window & TargetWindow);

	void Initialize();
	void Release();

	void Render();
	
protected:
	static const UINT BufferFrameCount = 2;

	Window & TargetWindow;

	Microsoft::WRL::ComPtr<IDXGIFactory4> Factory;
	Microsoft::WRL::ComPtr<ID3D12Device> Device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> RenderTargetViews[BufferFrameCount];
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocators[BufferFrameCount];
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;

	UINT RTVDescSize;
	UINT BufferFrameIndex;

	void EnableDebugLayer();
	void CreateFactory();
	void CreateDevice();
	void CreateCommandQueue();
	void CreateSwapChain();
	void CreateRTVHeap();
	void CreateRTVs();
	void CreateCommandAllocator();

	void CreateCommandList();
};
