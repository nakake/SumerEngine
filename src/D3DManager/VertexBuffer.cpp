#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(size_t size, size_t stride, const void* pInitData, const GraphicManager* gEngine) {
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	auto hr = gEngine->GetDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(pBuffer.GetAddressOf())
	);

	if (FAILED(hr)) {
		ErrorMessage(gEngine->GetWnd()->getHandleWnd() , TEXT("頂点バッファリソースの生成に失敗"));
		return;
	}

	view.BufferLocation = pBuffer->GetGPUVirtualAddress();
	view.SizeInBytes	= static_cast<UINT>(size);
	view.StrideInBytes	= static_cast<UINT>(stride);

	if (pInitData != nullptr) {
		void* ptr = nullptr;
		hr = pBuffer->Map(0, nullptr, &ptr);
		if (FAILED(hr)) {
			ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("頂点バッファマッピングに失敗"));
			return;
		}

		memcpy(ptr, pInitData, size);

		pBuffer->Unmap(0, nullptr);
	}

	isValid = true;
}
