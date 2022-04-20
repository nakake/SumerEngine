#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(size_t size, const GraphicManager* gEngine) {
	size_t align = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	UINT64 sizeAligned = (size + (align - 1)) & ~(align - 1);

	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(sizeAligned);

	auto hr = gEngine->GetDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(pBuffer.GetAddressOf())
	);

	if (FAILED(hr)) {
		ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("定数バッファリソースの生成に失敗"));
		return;
	}

	hr = pBuffer->Map(0, nullptr, &pMappedPtr);

	if (FAILED(hr)) {
		ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("定数バッファのマッピングに失敗"));
		return;
	}

	Desc				= {};
	Desc.BufferLocation = pBuffer->GetGPUVirtualAddress();
	Desc.SizeInBytes	= UINT(sizeAligned);

	isValid = true;
}