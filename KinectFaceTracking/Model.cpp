// Model.cpp : Represents a Mesh with its VertexBuffer, VertexIndices, RootSignature and Shader
//

#include "stdafx.h"
#include "Model.h"

#include "Resource.h"
#include "GraphicsContext.h"
#include "Camera.h"

Model::Model(_In_ GraphicsContext & DeviceContext)
	:DeviceContext(DeviceContext)
{
}

void Model::Create()
{
	CreateRootSignature();
	CreatePipelineState();

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexUploadResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexUploadResource;
	GPUFence Fence;

	Fence.Initialize(DeviceContext.GetDevice());
	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocator)));
	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&CommandList)));
	
	UploadVertices(CommandList, VertexUploadResource);
	UploadIndices(CommandList, IndexUploadResource);

	Utility::ThrowOnFail(CommandList->Close());

	ID3D12CommandList * CommandListPointer = CommandList.Get();
	DeviceContext.GetCommandQueue()->ExecuteCommandLists(1, &CommandListPointer);

	Fence.SetAndWait(DeviceContext.GetCommandQueue());
}

void Model::Render(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ Camera & Camera)
{
	CommandList->SetPipelineState(PipelineState.Get());
	CommandList->SetGraphicsRootSignature(RootSignature.Get());

	CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
	CommandList->IASetIndexBuffer(&IndexBufferView);

	// Set Object Constant
	const DirectX::XMFLOAT4X4 & ViewMatrix = Camera.GetViewMatrix();
	const DirectX::XMFLOAT4X4 & ProjectionMatrix = Camera.GetProjectionMatrix();

	CommandList->SetGraphicsRoot32BitConstants(0, Num32BitPerMatrix, &ViewMatrix, 0);
	CommandList->SetGraphicsRoot32BitConstants(0, Num32BitPerMatrix, &ProjectionMatrix, Num32BitPerMatrix);

	DirectX::XMMATRIX Object = DirectX::XMMatrixIdentity() * DirectX::XMMatrixRotationRollPitchYaw(33.f, 5.f, 47.f);
	DirectX::XMFLOAT4X4 ObjectMatrix;
	DirectX::XMStoreFloat4x4(&ObjectMatrix, Object);

	CommandList->SetGraphicsRoot32BitConstants(1, Num32BitPerMatrix, &ObjectMatrix, 0);

	CommandList->DrawIndexedInstanced(IndexCount, 1, 0, 0, 0);
}

void Model::CreateRootSignature()
{
	std::array<CD3DX12_ROOT_PARAMETER, 2> RootParameters;
	RootParameters[0].InitAsConstants(2 * Num32BitPerMatrix, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	RootParameters[1].InitAsConstants(1 * Num32BitPerMatrix, 1, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	D3D12_ROOT_SIGNATURE_FLAGS RootSignatureFlags = 
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(static_cast<UINT>(RootParameters.size()), RootParameters.data(), 0, nullptr, RootSignatureFlags);

	Microsoft::WRL::ComPtr<ID3DBlob> SerializedSignature;
	Microsoft::WRL::ComPtr<ID3DBlob> Error;

	Utility::ThrowOnFail(D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &SerializedSignature, &Error), Error);
	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateRootSignature(0, SerializedSignature->GetBufferPointer(), SerializedSignature->GetBufferSize(), IID_PPV_ARGS(&RootSignature)));
}

void Model::CreatePipelineState()
{
	Microsoft::WRL::ComPtr<ID3DBlob> VertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> PixelShader;

	LoadAndCompileShader(VertexShader, PixelShader);

	std::array<D3D12_INPUT_ELEMENT_DESC, 2> InputElementDesc
	{ {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	} };

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};
	PipelineStateDesc.InputLayout = { InputElementDesc.data(), static_cast<UINT>(InputElementDesc.size()) };
	PipelineStateDesc.pRootSignature = RootSignature.Get();
	PipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(VertexShader.Get());
	PipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(PixelShader.Get());
	PipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	PipelineStateDesc.SampleMask = UINT_MAX;
	PipelineStateDesc.SampleDesc.Count = 1;
	PipelineStateDesc.NumRenderTargets = 1;
	PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateGraphicsPipelineState(&PipelineStateDesc, IID_PPV_ARGS(&PipelineState)));
}

void Model::UploadVertices(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource)
{
	std::array<Vertex, 8> Cube =
	{ {
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	} };

	constexpr size_t SizeOfCube = Cube.size() * sizeof(Vertex);

	UploadData(CommandList, VertexBuffer, UploadResource, Cube.data(), SizeOfCube);

	VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
	VertexBufferView.StrideInBytes = sizeof(Vertex);
	VertexBufferView.SizeInBytes = SizeOfCube;
}

void Model::UploadIndices(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource)
{
	using IndexSize = uint16_t;
	std::array<IndexSize, 36> Indices =
	{
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5,
	};

	constexpr size_t SizeOfIndices = Indices.size() * sizeof(IndexSize);
	IndexCount = Indices.size();

	UploadData(CommandList, IndexBuffer, UploadResource, Indices.data(), SizeOfIndices);

	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.SizeInBytes = SizeOfIndices;
	IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Model::UploadData(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & Resource, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource, _In_reads_bytes_(DataSize) const void * Data, _In_ size_t  DataSize)
{
	CD3DX12_HEAP_PROPERTIES DefaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_HEAP_PROPERTIES UploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(DataSize);

	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommittedResource(
		&DefaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&Resource)));

	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommittedResource(
		&UploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&UploadResource)));

	D3D12_SUBRESOURCE_DATA SubresourceData = {};
	SubresourceData.pData = Data;
	SubresourceData.RowPitch = DataSize;
	SubresourceData.SlicePitch = SubresourceData.RowPitch;

	UpdateSubresources(CommandList.Get(), Resource.Get(), UploadResource.Get(), 0, 0, 1, &SubresourceData);

	CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	CommandList->ResourceBarrier(1, &ResourceBarrier);
}

void Model::LoadAndCompileShader(_Out_ Microsoft::WRL::ComPtr<ID3DBlob>& VertexShader, _Out_ Microsoft::WRL::ComPtr<ID3DBlob>& PixelShader)
{
	Microsoft::WRL::ComPtr<ID3DBlob> Error;

	HRSRC ShaderResource = FindResource(nullptr, MAKEINTRESOURCE(IDR_SHADER), RT_RCDATA);
	HGLOBAL ShaderResourceHandle = LoadResource(nullptr, ShaderResource);
	DWORD ContentSize = SizeofResource(nullptr, ShaderResource);
	LPVOID Content = LockResource(ShaderResourceHandle);

#if defined(_DEBUG)
	UINT CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT CompileFlags = 0;
#endif

	Utility::ThrowOnFail(D3DCompile(Content, ContentSize, nullptr, nullptr, nullptr, "VShader", "vs_5_1", CompileFlags, 0, &VertexShader, &Error), Error);
	Utility::ThrowOnFail(D3DCompile(Content, ContentSize, nullptr, nullptr, nullptr, "PShader", "ps_5_1", CompileFlags, 0, &PixelShader, &Error), Error);
}
