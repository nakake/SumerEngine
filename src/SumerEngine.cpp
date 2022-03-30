#include "SumerEngine.h"

bool SumerEngine::InitEngine() {
	if (!wndManager.InitWnd()) {
		return false;
	}

	return true;

}

void SumerEngine::TermEngine() {
	wndManager.TermWnd();
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