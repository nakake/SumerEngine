#include "SumerEngine.h"

bool SumerEngine::InitEngine() {
#if defined(DEBUG) || defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> debug;
		auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));

		if (SUCCEEDED(hr)) {
			
			debug->EnableDebugLayer();
		}
	}
#endif
	wndManager.reset(new WndManager(width, height));
	if (!wndManager->InitWnd()) {
		return false;
	}

	d3dManager.reset(new D3DManager());
	if (!d3dManager->InitD3DManager(wndManager.get(), frameCount)) {
		return false;
	}


	return true;

}

void SumerEngine::SetClearColor(float red, float green, float blue, float alpha) {
	d3dManager->SetClearColor(red, green, blue, alpha);
}

bool SumerEngine::LoopSetUp() {
	MSG msg = {};

	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			break;
		}
	}

	return msg.message != WM_QUIT;
}

void SumerEngine::StartRender() {
	d3dManager->StartRender();
}

void SumerEngine::EndRender() {
	d3dManager->EndRender();
}