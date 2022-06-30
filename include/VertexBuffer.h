#pragma once

#include "IFManager.h"
#include "ComPtr.h"
#include "GraphicManager.h"
#include "ErrorMassage.h"

class VertexBuffer
{
public:
	VertexBuffer(size_t size, size_t stride, const void* pInitData, const GraphicManager* gEngine);

	D3D12_VERTEX_BUFFER_VIEW View() const {
		return view;
	}

	bool IsValid() {
		return isValid;
	}
private:
	bool isValid = false;
	ComPtr<ID3D12Resource> pBuffer;
	D3D12_VERTEX_BUFFER_VIEW view = {};
};

