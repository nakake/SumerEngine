#pragma once

#include "IFManager.h"
#include "GraphicManager.h"
#include "ComPtr.h"
#include "ErrorMassage.h"

class IndexBuffer
{
public:
	IndexBuffer(size_t size, const uint32_t* pInitData, const GraphicManager* gEngine);

	bool IsValid() {
		return isValid;
	}

	D3D12_INDEX_BUFFER_VIEW View() const {
		return view;
	}

private:
	bool isValid = false;
	ComPtr<ID3D12Resource> pBuffer;
	D3D12_INDEX_BUFFER_VIEW view;

	IndexBuffer(const IndexBuffer&) = delete;
	void operator = (const IndexBuffer&) = delete;
};

