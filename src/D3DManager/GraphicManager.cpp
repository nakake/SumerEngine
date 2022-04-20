#include "GraphicManager.h"


void GraphicManager::StartRender() {
	pvCmdAllocator[FrameIndex]->Reset();
	pCmdList->Reset(pvCmdAllocator[FrameIndex].Get(), nullptr);
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource	= pvColorBuffer[FrameIndex].Get();
	barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pCmdList->RSSetViewports(1, &Viewport);
	pCmdList->RSSetScissorRects(1, &Scissor);

	pCmdList->ResourceBarrier(1, &barrier);

	pCmdList->OMSetRenderTargets(1, &vHandleRTV[FrameIndex], FALSE, &HandleDSV);

	pCmdList->ClearRenderTargetView(vHandleRTV[FrameIndex], clearColor.data(), 0, nullptr);

	pCmdList->ClearDepthStencilView(HandleDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	
}

void GraphicManager::EndRender() {
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource	= pvColorBuffer[FrameIndex].Get();
	barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pCmdList->ResourceBarrier(1, &barrier);

	auto hr = pCmdList->Close();


	ID3D12CommandList* ppCmdLists[] = { pCmdList.Get() };
	pQueue->ExecuteCommandLists(1, ppCmdLists);

	Present(1);
}

//描画が終了するまで待ち、終了処理を行う
void GraphicManager::Present(uint32_t interval) {
	pSwapChain->Present(interval, 0);

	const auto currentValue = vFenceCounter[FrameIndex];
	pQueue->Signal(pFence.Get(), currentValue);

	FrameIndex = pSwapChain->GetCurrentBackBufferIndex();

	//次のフレームの描画準備がまだであれば待機する
	if (pFence->GetCompletedValue() < vFenceCounter[FrameIndex]) {
		pFence->SetEventOnCompletion(vFenceCounter[FrameIndex], FenceEvent);
		WaitForSingleObjectEx(FenceEvent, INFINITE, FALSE);
	}

	vFenceCounter[FrameIndex] = currentValue + 1;
}

void GraphicManager::WaitGPU() {
	assert(pQueue != nullptr);
	assert(pFence != nullptr);
	assert(FenceEvent != nullptr);

	pQueue->Signal(pFence.Get(), vFenceCounter[FrameIndex]);
	pFence->SetEventOnCompletion(vFenceCounter[FrameIndex], FenceEvent);
	WaitForSingleObjectEx(FenceEvent, INFINITE, FALSE);
	vFenceCounter[FrameIndex]++;
}

void GraphicManager::TermGraphicManager() {
	WaitGPU();

	if (FenceEvent != nullptr) {
		CloseHandle(FenceEvent);
		FenceEvent = nullptr;
	}

	pFence.Reset();

	pHeapRTV.Reset();

	pHeapDSV.Reset();

	pDepthBuffer.Reset();

	for (auto i = 0u; i < FrameCount; ++i) {
		pvColorBuffer[i].Reset();
	}

	pCmdList.Reset();

	for (auto i = 0u; i < FrameCount; ++i) {
		pvCmdAllocator[i].Reset();
	}

	pSwapChain.Reset();

	pQueue.Reset();

	pDevice.Reset();

}

bool GraphicManager::Init(const WndManager* wnd, uint32_t frameCount) {
	wManager.reset(new WndManager);
	wManager->Copy(wnd);
	this->FrameCount	= frameCount;

	pvCmdAllocator.resize(this->FrameCount);
	pvColorBuffer.resize(this->FrameCount);
	vFenceCounter.resize(this->FrameCount);
	vHandleRTV.resize(this->FrameCount);

	clearColor.resize(4);
	clearColor[0] = 0.25f;
	clearColor[1] = 0.25f;
	clearColor[2] = 0.25f;
	clearColor[3] = 1.0f;

	//ファクトリーの生成
	ComPtr<IDXGIFactory4> pFactory = nullptr;
	auto hr = CreateDXGIFactory1(IID_PPV_ARGS(pFactory.GetAddressOf()));
	if (FAILED(hr)) {
		return false;
	}

	//デバイスの生成
	if (!CreateDevice()) {
		ErrorMessage(wManager->getHandleWnd(), TEXT("Error : GraphicManager デバイスの生成に失敗しました"));
		return false;
	}

	if (!CreateCmdQueue()) {
		ErrorMessage(wManager->getHandleWnd(), TEXT("Error : GraphicManager コマンドキューの生成に失敗しました"));
		return false;
	}
	if (!CreateSwapChain(pFactory)) {
		ErrorMessage(wManager->getHandleWnd(), TEXT("Error : GraphicManager スワップチェインの生成に失敗しました"));
		return false;
	}
	if (!CreateCmdAllocator()) {
		ErrorMessage(wManager->getHandleWnd(), TEXT("Error : GraphicManager コマンドアローケータの生成に失敗しました"));
		return false;
	}
	if (!CreateCmdList()) {
		ErrorMessage(wManager->getHandleWnd(), TEXT("Error : GraphicManager コマンドリストの生成に失敗しました"));
		return false;
	}
	if (!CreateRTV()) {
		ErrorMessage(wManager->getHandleWnd(), TEXT("Error : GraphicManager レンダーターゲットビューの生成に失敗しました"));
		return false;
	}
	if (!CreateDSV()) {
		ErrorMessage(wManager->getHandleWnd(), TEXT("Error : GraphicManager 深度ステンシルビューの生成に失敗しました"));
		return false;
	}
	if (!CreateFence()) {
		ErrorMessage(wManager->getHandleWnd(), TEXT("Error : GraphicManager フェンスの生成に失敗しました"));
		return false;
	}

	CreateViewPort();
	CreateScissorRect();

	pFactory.Reset();
	return true;
}

bool GraphicManager::CreateDevice() {
	auto hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(pDevice.GetAddressOf())
	);

	if (FAILED(hr)) {
		return false;
	}
	return true;
}

bool GraphicManager::CreateCmdQueue() {
	D3D12_COMMAND_QUEUE_DESC desc	= {};
	desc.Type						= D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority					= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags						= D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask					= 0;

	auto hr = pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(pQueue.GetAddressOf()));
	if (FAILED(hr)) {
		return false;
	}
	return true;
}

bool GraphicManager::CreateSwapChain(ComPtr<IDXGIFactory4> pFactory) {
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width						= wManager->GetWidth();
	desc.BufferDesc.Height						= wManager->GetHeight();
	desc.BufferDesc.RefreshRate.Numerator		= 60;
	desc.BufferDesc.RefreshRate.Denominator		= 1;
	desc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling						= DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count						= 1;
	desc.SampleDesc.Quality						= 0;
	desc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount							= FrameCount;
	desc.OutputWindow							= wManager->getHandleWnd();
	desc.Windowed								= TRUE;
	desc.SwapEffect								= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain> pTempSwapChain;
	auto hr = pFactory->CreateSwapChain(pQueue.Get(), &desc, pTempSwapChain.GetAddressOf());
	if (FAILED(hr)) {
		return false;
	}

	hr = pTempSwapChain.As(&this->pSwapChain);
	if (FAILED(hr)) {
		return false;
	}

	FrameIndex = this->pSwapChain->GetCurrentBackBufferIndex();

	pTempSwapChain.Reset();

	return true;
}

bool GraphicManager::CreateCmdAllocator() {
	for (auto i = 0u; i < FrameCount; ++i) {
		auto hr = pDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(pvCmdAllocator[i].GetAddressOf())
		);
		if (FAILED(hr)) {
			return false;
		}
	}

	return true;
}
bool GraphicManager::CreateCmdList() {
	auto hr = pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		pvCmdAllocator[FrameIndex].Get(),
		nullptr,
		IID_PPV_ARGS(pCmdList.GetAddressOf())
	);
	if (FAILED(hr)) {
		return false;
	}

	pCmdList->Close();

	return true;
}
bool GraphicManager::CreateRTV() {
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors		= FrameCount;
	desc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask			= 0;

	auto hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(pHeapRTV.GetAddressOf()));
	if (FAILED(hr)) {
		return false;
	}

	auto handle = pHeapRTV->GetCPUDescriptorHandleForHeapStart();
	auto incrementSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (auto i = 0u; i < FrameCount; ++i) {
		hr = pSwapChain->GetBuffer(i, IID_PPV_ARGS(pvColorBuffer[i].GetAddressOf()));
		if (FAILED(hr)) {
			return false;
		}

		D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
		viewDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		viewDesc.ViewDimension			= D3D12_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice		= 0;
		viewDesc.Texture2D.PlaneSlice	= 0;

		pDevice->CreateRenderTargetView(pvColorBuffer[i].Get(), &viewDesc, handle);

		vHandleRTV[i] = handle;
		handle.ptr += incrementSize;
	}

	return true;
}

bool GraphicManager::CreateDSV() {
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type					= D3D12_HEAP_TYPE_DEFAULT;
	prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask		= 1;
	prop.VisibleNodeMask		= 1;
	
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Alignment			= 0;
	resDesc.Width				= wManager->GetWidth();
	resDesc.Height				= wManager->GetHeight();
	resDesc.DepthOrArraySize	= 1;
	resDesc.MipLevels			= 1;
	resDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	resDesc.SampleDesc.Count	= 1;
	resDesc.SampleDesc.Quality	= 0;
	resDesc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Flags				= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format					= DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth		= 1.0;
	clearValue.DepthStencil.Stencil		= 0;

	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(pDepthBuffer.GetAddressOf())
	);
	if (FAILED(hr)) {
		return false;
	}

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors		= 1;
	desc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	desc.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask			= 0;

	hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(pHeapDSV.GetAddressOf()));
	if (FAILED(hr)) {
		return false;
	}

	auto handle = pHeapDSV->GetCPUDescriptorHandleForHeapStart();
	auto incrementSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
	viewDesc.Format					= DXGI_FORMAT_D32_FLOAT;
	viewDesc.ViewDimension			= D3D12_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice		= 0;
	viewDesc.Flags					= D3D12_DSV_FLAG_NONE;

	pDevice->CreateDepthStencilView(pDepthBuffer.Get(), &viewDesc, handle);

	HandleDSV = handle;

	return true;
}

bool GraphicManager::CreateFence() {
	for (auto i = 0u; i < FrameCount; ++i) {
		vFenceCounter[i] = 0;
	}

	auto hr = pDevice->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(pFence.GetAddressOf())
	);
	if (FAILED(hr)) {
		return false;
	}

	vFenceCounter[FrameIndex]++;

	FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (FenceEvent == nullptr) {
		return false;
	}

	return true;
}

void GraphicManager::CreateViewPort() {
	Viewport.TopLeftX	= 0;
	Viewport.TopLeftY	= 0;
	Viewport.Width		= static_cast<float>(wManager->GetWidth());
	Viewport.Height		= static_cast<float>(wManager->GetHeight());
	Viewport.MinDepth	= 0.0f;
	Viewport.MaxDepth	= 1.0f;
}

void GraphicManager::CreateScissorRect() {
	Scissor.left	= 0;
	Scissor.right	= wManager->GetWidth();
	Scissor.top		= 0;
	Scissor.bottom	= wManager->GetHeight();
}
