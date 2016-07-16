// GPUFence.cpp : A Fence to synchronize with GPU
//

#include "stdafx.h"
#include "GPUFence.h"

GPUFence::GPUFence()
	:FenceEvent(nullptr)
{
}

GPUFence::~GPUFence()
{
	if (FenceEvent != nullptr)
	{
		CloseHandle(FenceEvent); 
		FenceEvent = nullptr;
	}
}

void GPUFence::Initialize(_In_ Microsoft::WRL::ComPtr<ID3D12Device>& Device)
{
	Utility::ThrowOnFail(Device->CreateFence(ReadyValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (Fence == nullptr)
	{
		Utility::Throw(L"Fence Event Creation failed!");
	}
}

void GPUFence::Set(_In_ Microsoft::WRL::ComPtr<ID3D12CommandQueue> & CommandQueue)
{
	Utility::ThrowOnFail(Fence->Signal(BusyValue));
	CommandQueue->Signal(Fence.Get(), ReadyValue);
}

void GPUFence::Wait()
{
	if (Fence->GetCompletedValue() == BusyValue)
	{
		Utility::ThrowOnFail(Fence->SetEventOnCompletion(ReadyValue, FenceEvent));
		WaitForSingleObjectEx(FenceEvent, INFINITE, FALSE);
	}
}

void GPUFence::SetAndWait(_In_ Microsoft::WRL::ComPtr<ID3D12CommandQueue> & CommandQueue)
{
	Set(CommandQueue);
	Wait();
}
