#include "GraphicsDevice.h"
#include "CommandQueue.h"
#include "CommandList.h"
#include "GPUBuffer.h"

namespace Venus::Private::Direct3D12
{
	VEGraphicsDevice* CreateGraphicsDevice()
	{
		return new GraphicsDevice();
	}
}

using namespace Venus;
using namespace Venus::Private::Direct3D12;

GraphicsDevice::GraphicsDevice()
{
    HRESULT hardwareResult = S_OK;
    // 먼저 Factory를 만든다. 넘버링이 올라갈 수록 기능을 더 제공해준다.
    hardwareResult = CreateDXGIFactory1(IID_PPV_ARGS(&factory));

    // 그래픽 카드에 대한 정보들을 loop 돈다. (책 코드에는 loop 사용 없이 가장 첫 번째 그래픽 카드로 설정)
    // Try to create hardware device.
    hardwareResult = D3D12CreateDevice(
        nullptr,             // default adapter
        D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(&device));

    // Fallback to WARP device.
    if (FAILED(hardwareResult))
    {
        IDXGIAdapter* pWarpAdapter;
      
        // 그래픽 카드가 없는 경우 CPU로 GPU처럼 행동하게 만들어준다.
        hardwareResult = factory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter));

        hardwareResult = D3D12CreateDevice(
            pWarpAdapter,
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&device));
    }
}

VEObject<VECommandQueue> GraphicsDevice::CreateCommandQueue()
{
    ComPtr<ID3D12CommandQueue> queue;
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue)));

    return new CommandQueue(this, queue.Get());
}

VEObject<VECommandList> GraphicsDevice::CreateCommandList()
{
    ComPtr<ID3D12CommandAllocator> commandAllocator;

    ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf())));

    ComPtr<ID3D12GraphicsCommandList> commandList;
    ThrowIfFailed(device->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        commandAllocator.Get(), // Associated command allocator
        nullptr,                   // Initial PipelineStateObject
        IID_PPV_ARGS(commandList.GetAddressOf())));

    return new CommandList(commandAllocator.Get(), commandList.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
}

// Ref. DX12 : CreateDefaultBuffer Function
VEObject<VEGPUBuffer> GraphicsDevice::CreateGPUBuffer(size_t size, VEGPUBuffer::CPUCacheMode mode)
{
    D3D12_HEAP_PROPERTIES heapProperty{};
    D3D12_RESOURCE_STATES initialResourceState{};
    switch (mode)
    {
    case GPUBuffer::CPUCacheMode::DEFAULT:
        heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        initialResourceState = D3D12_RESOURCE_STATE_COMMON;
        break;
    case GPUBuffer::CPUCacheMode::UPLOAD:
        heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        initialResourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
        break;
    case GPUBuffer::CPUCacheMode::READBACK:
        heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);
        initialResourceState = D3D12_RESOURCE_STATE_COPY_DEST;
        break;
    }

    D3D12_RESOURCE_DESC bufferDesc;
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Alignment = 0;
    bufferDesc.Width = AlignGPUBufferSize(size);
    bufferDesc.Height = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.MipLevels = 1;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.SampleDesc.Quality = 0;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    ComPtr<ID3D12Resource> buffer;
    ThrowIfFailed(device->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        initialResourceState,
        nullptr,
        IID_PPV_ARGS(buffer.GetAddressOf())));

    return new GPUBuffer(buffer.Get(), mode, initialResourceState);
}