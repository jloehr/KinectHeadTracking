#pragma once

#include "GPUFence.h"

class GraphicsContext
{
public:
	void Initialize();
	void Release();

	Microsoft::WRL::ComPtr<IDXGIFactory4> & GetFactory();
	Microsoft::WRL::ComPtr<ID3D12Device> & GetDevice();
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> & GetCommandQueue();

private:
	Microsoft::WRL::ComPtr<IDXGIFactory4> Factory;
	Microsoft::WRL::ComPtr<ID3D12Device> Device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue;

	GPUFence Fence;

	void EnableDebugLayer();
	void CreateFactory();
	void CreateDevice();
	void CreateCommandQueue();
};

