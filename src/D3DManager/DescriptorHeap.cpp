#include "DescriptorHeap.h"

const UINT HANDLE_MAX = 512;

DescriptorHeap::DescriptorHeap(const GraphicManager* _gEngine) {
	gEngine = _gEngine;
	pHandles.clear();
	pHandles.reserve(HANDLE_MAX);

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NodeMask			= 1;
	desc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors		= HANDLE_MAX;
	desc.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	auto device = gEngine->GetDevice();

	auto hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(pHeap.ReleaseAndGetAddressOf()));

	if (FAILED(hr)) {
		isValid = false;
		return;
	}

	incrementSize = device->GetDescriptorHandleIncrementSize(desc.Type);
	isValid = true;
}

void DescriptorHeap::Register(Texture2D* texture) {
	auto count = pHandles.size();
	if (HANDLE_MAX <= count) {
		return;
	}

	std::unique_ptr<DescriptorHandle> pHandle;
	pHandle.reset(new DescriptorHandle());

	auto handleCPU = pHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += incrementSize * count;

	auto handleGPU = pHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += incrementSize * count;

	pHandle->HandleCPU = handleCPU;
	pHandle->HandleGPU = handleGPU;

	auto device = gEngine->GetDevice();
	auto resouce = texture->Resouce();
	auto desc = texture->ViewDesc();
	device->CreateShaderResourceView(resouce, &desc, pHandle->HandleCPU);

	pHandles.push_back(std::move(pHandle));
}