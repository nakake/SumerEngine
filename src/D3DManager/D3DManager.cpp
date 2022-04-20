#include "D3DManager.h"

bool D3DManager::Init(const WndManager* wnd, uint32_t _frameCount) {
	wManager.reset(new WndManager());
	wManager->Copy(wnd);

	frameCount = _frameCount;
	
	gManager.reset(new GraphicManager());
	gManager->Init(wManager.get(), frameCount);
	return true;
}

void D3DManager::StartRender() {
	gManager->StartRender();
}

void D3DManager::EndRender() {
	gManager->EndRender();
}

void D3DManager::SetClearColor(float red, float green, float blue, float alpha) {
	gManager->SetClearColor(red, green, blue, alpha);
}