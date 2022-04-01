#pragma once

#include "IFManager.h"
#include "WndManager.h"

using std::vector;

class WndManager;

class GraphicManager
{

public:
	GraphicManager(){}
	~GraphicManager(){
		TermGraphicManager();
	}

	bool InitGraphicManager(const WndManager* wnd, uint32_t frameCount);
	void StartRender();
	void EndRender();


	void SetClearColor(float red, float green, float blue, float alpha) {
		this->clearColor.clear();
		clearColor.resize(4);
		clearColor[0] = red;
		clearColor[1] = green;
		clearColor[2] = blue;
		clearColor[3] = alpha;
	}

private:
	std::unique_ptr<WndManager>				wManager;
	uint32_t								FrameCount;
	vector<float>							clearColor;

	ComPtr<ID3D12Device>					pDevice;
	ComPtr<ID3D12CommandQueue>				pQueue;
	ComPtr<IDXGISwapChain3>					pSwapChain;
	vector<ComPtr<ID3D12CommandAllocator>>	pvCmdAllocator;
	ComPtr<ID3D12GraphicsCommandList>		pCmdList;
	ComPtr<ID3D12DescriptorHeap>			pHeapDSV;
	ComPtr<ID3D12DescriptorHeap>			pHeapRTV;
	vector<ComPtr<ID3D12Resource>>			pvColorBuffer;
	ComPtr<ID3D12Resource>					pDepthBuffer;
	ComPtr<ID3D12Fence>						pFence;

	HANDLE									FenceEvent;
	vector<uint64_t>						vFenceCounter;
	uint32_t								FrameIndex;
	vector<D3D12_CPU_DESCRIPTOR_HANDLE>		vHandleRTV;
	D3D12_CPU_DESCRIPTOR_HANDLE				HandleDSV;

	bool CreateDevice();
	bool CreateCmdQueue();
	bool CreateSwapChain(ComPtr<IDXGIFactory4> pFactory);
	bool CreateCmdAllocator();
	bool CreateCmdList();
	bool CreateRTV();
	bool CreateDSV();
	bool CreateFence();

	void Present(uint32_t interval);
	void WaitGPU();
	void TermGraphicManager();
};

