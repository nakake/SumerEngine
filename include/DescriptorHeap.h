#pragma once

#include "IFManager.h"
#include "ComPtr.h"
#include "GraphicManager.h"
#include "Texture2D.h"

class ConstantBuffer;
class Texture2D;

class DescriptorHandle
{
public:
	D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;
};

class DescriptorHeap
{
public:
	DescriptorHeap(const GraphicManager* _gEngine);
	void Register(Texture2D* texture);
	
	ID3D12DescriptorHeap* GetHeap() {
		return pHeap.Get();
	}
	std::vector<std::unique_ptr<DescriptorHandle>> pHandles;
private:
	bool isValid = false;
	UINT incrementSize = 0;
	ComPtr<ID3D12DescriptorHeap> pHeap = nullptr;
	const GraphicManager* gEngine;
};

