#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(size_t size, const uint32_t* pInitData, const GraphicManager* gEngine) {
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	auto hr = gEngine->GetDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(pBuffer.GetAddressOf())
	);

	if (FAILED(hr)) {
		ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("インデックスバッファリソースの生成に失敗"));
		return;
	}

	view					= {};
	view.BufferLocation		= pBuffer->GetGPUVirtualAddress();
	view.Format				= DXGI_FORMAT_R32_UINT;
	view.SizeInBytes		= static_cast<UINT>(size);

	if (pInitData != nullptr) {
		void* ptr = nullptr;
		hr = pBuffer->Map(0, nullptr, &ptr);
		if (FAILED(hr)) {
			ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("インデックスバッファマッピングに失敗"));
			return;
		}
		memcpy(ptr, pInitData, size);

		pBuffer->Unmap(0, nullptr);
	}

	isValid = true;
}
