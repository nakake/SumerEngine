#include "SumerEngine.h"

void SumerEngine::StartEngine() {
	MainLoop();
}

bool SumerEngine::Init() {
#if defined(DEBUG) || defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> debug;
		auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));

		if (SUCCEEDED(hr)) {
			
			debug->EnableDebugLayer();
		}

	}
#endif
	wndManager.reset(new WndManager(width, height, appName));
	if (!wndManager->Init()) {
		return false;
	}

	d3dManager.reset(new D3DManager());
	if (!d3dManager->Init(wndManager.get(), frameCount)) {
		return false;
	}

	m_Scene.reset(new Scene(d3dManager->GetGraphicManager(), frameCount));
	if (!m_Scene->Init()) {
		return false;
	}


	return true;

}

void SumerEngine::SetClearColor(float red, float green, float blue, float alpha) {
	d3dManager->SetClearColor(red, green, blue, alpha);
}

void SumerEngine::MainLoop() {
	MSG msg = {};

	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			m_Scene->Update();
			StartRender();
			m_Scene->Draw();
			EndRender();
		}
	}
}

void SumerEngine::StartRender() {
	d3dManager->StartRender();
}

void SumerEngine::EndRender() {
	d3dManager->EndRender();
}