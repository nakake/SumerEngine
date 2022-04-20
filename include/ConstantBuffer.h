#pragma once

#include "IFManager.h"
#include "ComPtr.h"
#include "GraphicManager.h"

class ConstantBuffer
{
public:
	ConstantBuffer(size_t size, const GraphicManager* gEngine);

	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const {
		return Desc.BufferLocation;
	}
	
	D3D12_CONSTANT_BUFFER_VIEW_DESC ViewDesc() {
		return Desc;
	}

	void* GetPtr() const {
		return pMappedPtr;
	}

	template<typename T>
	T* GetPtr() {
		return reinterpret_cast<T*>(GetPtr());
	}
	
	bool IsValid() {
		return isValid;
	}

private:
	bool isValid = false;
	ComPtr<ID3D12Resource> pBuffer;
	D3D12_CONSTANT_BUFFER_VIEW_DESC Desc;
	void* pMappedPtr = nullptr;

	ConstantBuffer(const ConstantBuffer&) = delete;
	void operator = (const ConstantBuffer&) = delete;
};

